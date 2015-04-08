/*
 * Skeleton of the analysis algorithm, defines the way we parse the CFG 
 */

#include <ctime>
#include <elm/io/Output.h>
#include <elm/genstruct/SLList.h>
#include <otawa/cfg/Edge.h>
#include <otawa/cfg/BasicBlock.h>
#include <otawa/cfg/features.h>

#include "analysis.h"
#include "smt.h"
#include "debug.h"

using namespace elm::genstruct;
using namespace elm::io;

Analysis::State::State() : dfa_state(NULL), sp(0), constants(0, 0) { }

Analysis::State::State(BasicBlock* entrybb, const dfa::State* state, const OperandVar& sp, unsigned int max_tempvars, unsigned int max_registers, bool init)
	: dfa_state(state), sp(sp), constants(max_tempvars, max_registers)
{
	generated_preds.clear(); // generated_preds := [[]]
	labelled_preds.clear(); // labelled_preds := [[]]
	if(init)
	{
		BasicBlock::OutIterator outs(entrybb);
		ASSERT(outs);
		path += *outs;
		constants.set(sp, SP, false); // set that ?13==SP (since SP is the value of ?13 at the beginning of the program)
	}
}

Analysis::State::State(Edge* entry_edge, const dfa::State* state, const OperandVar& sp, unsigned int max_tempvars, unsigned int max_registers, bool init)
	: dfa_state(state), sp(sp), constants(max_tempvars, max_registers)
{
	generated_preds.clear(); // generated_preds := [[]]
	labelled_preds.clear(); // labelled_preds := [[]]
	if(init)
	{
		path += entry_edge;
		constants.set(sp, SP, false); // set that ?13==SP (since SP is the value of ?13 at the beginning of the program)
	}
}

Analysis::State::State(const State& s)
	: dfa_state(s.dfa_state), sp(s.sp), path(s.path), constants(s.constants), labelled_preds(s.labelled_preds), generated_preds(s.generated_preds), generated_preds_taken(s.generated_preds_taken)
	{ }

void Analysis::State::appendEdge(Edge* e, bool is_conditional)
{
	// add edge to the end of the path
	this->path.addLast(e);
	// we now need to label the correct list of predicates
	const SLList<LabelledPredicate> &relevant_preds = (is_conditional && e->kind() == Edge::TAKEN) ?
		generated_preds_taken : // conditional TAKEN
		generated_preds;  // NOT TAKEN, VIRTUAL, VIRTUAL RETURN, non-conditional TAKEN

	// label our list of predicates with the current edge then append it
	SLList<LabelledPredicate> labelled_analysis_result = labelPredicateList(relevant_preds, e);
	labelled_preds += labelled_analysis_result;
	// label the constants as well
	constants.label(e);
}

/**
 * @fn void Analysis::processCFG(CFG* cfg);
 * Runs the analysis on the CFG cfg
 * *WARNING*: atm, this function assumes we have NO LOOPS!
*/
void Analysis::processCFG(CFG* cfg)
{
	DBG(color::Whi() << "Processing CFG " << cfg)
	paths_count = infeasible_paths_count = total_paths = loop_header_count = 0;
	int processed_bbs = 0;
	placeboProcessCFG(cfg);
	std::time_t timestamp = clock(); // Timestamp before analysis

	/* begin */
	/* wl <- {ɛ} */
	wl.push(cfg->firstBB());
	/* lock[] <- {{}}; */
	Identifier<SLList<Analysis::State> > PROCESSED_EDGES("Analyis::IP analysis processed incoming edges"); //, SLList<State>::null);
	BasicBlock::OutIterator entry_outs(cfg->entry());
	SLList<Analysis::State> entry_annotation;
	entry_annotation += Analysis::State(cfg->entry(), dfa_state, sp, max_tempvars, max_registers);
	PROCESSED_EDGES(*entry_outs) = entry_annotation;

	/* While wl != ∅ */
	while(!wl.isEmpty())
	{
		DBG("wl=" << wlToString())
		/* bb::wl <- wl; */
		BasicBlock *bb = wl.pop();
		SLList<Analysis::State> sl;
		const bool is_loop_header = LOOP_HEADER(bb);

		/*  If lock[bb].count() < bb.ins.count
				continue;
			End If
		*/
		if(!allIncomingNonBackEdgesAreAnnotated(bb, PROCESSED_EDGES))
			continue;
		
		if(dbg_flags&DBG_NO_DEBUG && !(flags&SUPERSILENT))
			cout << "[" << ++processed_bbs*100/bb_count << "%] Processing BB #" << bb->number() << " of " << bb_count << " " << (is_loop_header?" (loop header)":"") << endl;
		/* sl <- mergeIntoOneList(lock[bb]); */
		for(BasicBlock::InIterator bb_ins(bb); bb_ins; bb_ins++)
			sl.addAll(*PROCESSED_EDGES(*bb_ins));
		// at this point of the algorithm, all incoming edges of bb have been processed
		purgeStateList(sl);

		// in case of loop, merge the state list into a single state
		if(is_loop_header)
		{
			State s((Edge*)NULL, dfa_state, sp, max_tempvars, max_registers, false); // entry is cleared anyway
			BasicBlock::OutIterator bb_outs(bb);
			// look for the loop exit edge
			while(LOOP_EXIT_EDGE(*bb_outs) != bb)
				bb_outs++;
			s.merge(sl, *bb_outs);
			sl.clear();
			sl += s; // sl <- {s}
		}

		// merge into a single empty state
		/*if(is_loop_header && sl)
		{
			State s(sl.first());
			sl.clear();
			s.throwInfo();
			sl += s;
		}*/

		/* For s in sl */
		for(SLList<Analysis::State>::MutableIterator sl_iter(sl); sl_iter; )
		{
			// if(is_loop_header) sl_iter.item().throwInfo(); // dump predicates info
			DBG(color::Whi() << "Processing path " << (*sl_iter).getPathString())
			/* processBB(s, bb); */
			if(processBB(sl_iter.item(), bb) > 0)
				sl.remove(sl_iter); // path ended
			else sl_iter++;
		}
		/* End For */

		/*	For e in bb.outs */
		for(BasicBlock::OutIterator bb_outs(bb); bb_outs; bb_outs++)
		{
			if(isAHandledEdgeKind(bb_outs->kind())) // filter out calls etc
			{
				if(BACK_EDGE(*bb_outs))
					DBG(color::Whi() << "End of loop reached.")
				// else 
				{
					// adds to PROCESSED_EDGE
					processOutEdge(*bb_outs, PROCESSED_EDGES, sl, isConditional(bb)); // annotate regardless of returned new_sl being empty or not
					if(!wl.contains(bb_outs->target()))
						wl.push(bb_outs->target());
				}
			}
		}
		/* End For */
	}
	/* End While */
	/* end */

	// analysis complete, print infeasible paths
	removeDuplicateInfeasiblePaths();
	printResults((clock()-timestamp)*1000/CLOCKS_PER_SEC);
}

/*
	@fn int Analysis::processBB(State& s, BasicBlock* bb);
	@param s Analysis state to update
	@param bb BasicBlock to process
	@return 0: continue, 1: stop analysis for this path
*/
int Analysis::processBB(State& s, BasicBlock* bb)
{
	if(bb->isExit())
	{
		onPathEnd();
		return 1;
	}
	
	DBG(color::Whi() << "Processing " << bb)
	s.processBB(bb); // generates lists of predicates in generated_preds and generated_preds_taken	
	return 0;
}

/**
 * @fn SLList<Analysis::State> Analysis::processOutEdge(Edge* e, const SLList<Analysis::State>& sl);
 * Processes outgoing Edge e from a BasicBlock for each element of sl
*/
// SLList<Analysis::State> Analysis::processOutEdge(Edge* e, const SLList<Analysis::State>& sl, bool is_conditional)
void Analysis::processOutEdge(Edge* e, const Identifier<SLList<Analysis::State> >& processed_edges_id, const SLList<Analysis::State>& sl, bool is_conditional)
{
	/* wl <- sl ⊙ e; */
	processed_edges_id.remove(e); // clearing: useless?
	if(sl.isEmpty())
	{	// just propagate the {bottom}
		State invalid_state;
		processed_edges_id.ref(e).addFirst(invalid_state);
		return;
	}
	SLList<Option<Path> > sl_paths;
	for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++)
	{
		State s = *sl_iter;
		s.appendEdge(e, is_conditional);

		// SMT call
		SMT smt;
		const Option<Path>& infeasible_path = smt.seekInfeasiblePaths(s);
		sl_paths.addLast(infeasible_path);
		if(infeasible_path)
		{
			// we need to add some sort of {} to say this is an infeasible path
			State invalid_state;
			processed_edges_id.ref(e).addFirst(invalid_state);
			ASSERT((*infeasible_path).contains(s.lastEdge())) // make sure the last edge was relevant in this path
		}
		else
			processed_edges_id.ref(e).addFirst(s);
	}
	SLList<Option<Path> >::Iterator sl_paths_iter(sl_paths);
	for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++, sl_paths_iter++)
	{
		const State& s = *sl_iter;
		if(*sl_paths_iter) // is infeasible?
		{
			const Path& infeasible_path = **sl_paths_iter;
			elm::String counterexample;
			DBG("Path " << s.getPathString() << "->" << e->target()->number() << " minimized to " << pathToString(infeasible_path))
			bool valid = checkInfeasiblePathValidity(sl, sl_paths, e, infeasible_path, counterexample);
			DBG(color::BIWhi() << "B)" << color::RCol() << " Verifying minimized path validity... " << (valid?color::IGre():color::IRed()) << (valid?"SUCCESS!":"FAILED!"))
			if(valid)
			{
				addDisorderedPath(infeasible_path, (*sl_iter).getPath(), e); // infeasible_paths += order(infeasible_path); to output proprer ffx!
				DBG(color::On_IRed() << "Inf. path found: " << pathToString(infeasible_path))
			}
			else // we found a counterexample, e.g. a feasible path that is included in the set of paths we marked as infeasible
			{
				DBG("   counterexample: " << counterexample)
				 // falling back on full path (not as useful as a result, but still something)
				OrderedPath original_full_path = (*sl_iter).getPath();
				original_full_path.addLast(e); // need to add e
				infeasible_paths.add(original_full_path);
				// TODO! clean that mess below...
				{
					Path ofp;
					for(OrderedPath::Iterator original_full_orderedpath_iter(original_full_path); original_full_orderedpath_iter; original_full_orderedpath_iter++)
						ofp += *original_full_orderedpath_iter;
					DBG(color::On_IRed() << "Inf. path found: " << pathToString(ofp) << color::RCol() << " (unrefined)")
				}
				// TODO: do a C) where we still try to refine this infeasible path?
			}
			onAnyInfeasiblePath();
		}
	}
}

/**
 * @fn bool Analysis::checkInfeasiblePathValidity(const SLList<Analysis::State>& sl, const SLList<Option<Path> >& sl_paths, const Edge* e, const Path& infeasible_path, elm::String& counterexample);
 * Checks if the minimized list of edges we found 'infeasible_path' is valid,
 * that is if all the paths it represents (all the 'sl[i]->e') are infeasible ('sl_paths[i]' is not 'elm::option::none')
 * If invalid, returns a counter-example in counterexample.
 * @return true if valid
*/
bool Analysis::checkInfeasiblePathValidity(const SLList<Analysis::State>& sl, const SLList<Option<Path> >& sl_paths, const Edge* e, const Path& infeasible_path, elm::String& counterexample) const
{
	bool valid = true;
	// check that all the paths going to the current BB are sound with the minimized inf. path we just found
	SLList<Option<Path> >::Iterator sl_paths_subiter(sl_paths);
	for(SLList<Analysis::State>::Iterator sl_subiter(sl); sl_subiter; sl_subiter++, sl_paths_subiter++)
	{
		// if feasible path && contained in the minimized inf. path
		if(!*sl_paths_subiter && isSubPath((*sl_subiter).getPath(), e, infeasible_path))
		{
			valid = false;
			counterexample = _ << (*sl_subiter).getPathString() << "+" << e->source()->number() << "->" << e->target()->number();
			// DBG("isSubPath(" << (*sl_subiter).getPath() << ", " << e->source()->number()<<">"<<e->target()->number() << ", " 
			// 	<< pathToString(infeasible_path) << ") = " << isSubPath((*sl_subiter).getPath(), e, infeasible_path))
			break;
		}
	}
	return valid;
}

// reorder the path then add it
void Analysis::addDisorderedPath(const Path& infeasible_path, const OrderedPath& full_path, Edge* last_edge)
{
	OrderedPath ordered_ip;
	for(OrderedPath::Iterator full_path_iter(full_path); full_path_iter; full_path_iter++)
	{
		if(infeasible_path.contains(*full_path_iter))
			ordered_ip.addLast(*full_path_iter);
	}
	if(infeasible_path.contains(last_edge))
		ordered_ip.addLast(last_edge);
	infeasible_paths.add(ordered_ip);
}

// remove all invalid states
void Analysis::purgeStateList(SLList<Analysis::State>& sl) const
{
	for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; )
	{
		if(sl_iter->isValid())
			sl_iter++;
		else
			sl.remove(sl_iter);
	}
}

// figures properties on the CFG without doing any actual analysis
void Analysis::placeboProcessCFG(CFG* cfg)
{
	if(dbg_flags&DBG_NO_DEBUG && !(flags&SUPERSILENT))
		cout << bb_count << " BBs found." << endl;
	else
		DBG(color::Whi() << total_paths << " paths found.")
	if(dbg_flags&DBG_NO_PREANALYSIS)
	{
		total_paths = 777;
		cout << bb_count << " BBs found." << endl;
		return;
	}
	if(dbg_flags&DBG_NO_DEBUG && !(flags&SUPERSILENT))
	{
		cout << "Running pre-analysis... ";
		placeboProcessBB(cfg->firstBB());
		cout << total_paths << " paths found." << endl;
	}
	else
	{
		DBG(color::Whi() << "Running pre-analysis... ")
		placeboProcessBB(cfg->firstBB());
		if(loop_header_count)
			DBG(color::Whi() << loop_header_count << " loop headers found.")
	}
}

void Analysis::placeboProcessBB(BasicBlock* bb)
{
	if(bb->isExit())
	{
		total_paths++;
		return;
	}
	if(LOOP_HEADER(bb)) // TODO: fix to merge paths into 1 at loop headers (?)
		loop_header_count++;
	for(BasicBlock::OutIterator outs(bb); outs; outs++)
		if(!BACK_EDGE(*outs) && isAHandledEdgeKind(outs->kind())) // Filter out irrelevant edges (calls...)
			placeboProcessBB((*outs)->target());
}

// print result of a whole CFG analysis
void Analysis::printResults(int exec_time_ms) const
{
	if(flags&SUPERSILENT)
		return;
	int infeasible_paths_count = infeasible_paths.count();
	if(dbg_flags&DBG_NO_TIME)
		DBG(color::BIGre() << infeasible_paths_count << " infeasible path" << (infeasible_paths_count == 1 ? "" : "s") << " found: ")
	else
		DBG(color::BIGre() << infeasible_paths_count << " infeasible path" << (infeasible_paths_count == 1 ? "" : "s") << " found: "
			<< "(" << (exec_time_ms>=1000 ? ((float)exec_time_ms)/(float(100)) : exec_time_ms) << (exec_time_ms>=1000 ? "s" : "ms") << ")")
	for(Vector<OrderedPath>::Iterator iter(infeasible_paths); iter; iter++)
	{
		const OrderedPath& l = *iter;
		bool first = true;
		elm::String str = "    * [";
		for(OrderedPath::Iterator subiter(l); subiter; subiter++)
		{
			if(first) first = false; else // do nothing at first iteration
				str = _ << str << ", ";
			str = str.concat(_ << (*subiter)->source()->number() << "->" << (*subiter)->target()->number());
		}
		str = _ << str << "]";
		DBG(color::IGre() << str)
		if(dbg_flags&DBG_NO_DEBUG)
			cout << str << endl;
	}
	if(dbg_flags&DBG_NO_DEBUG)
	{
		cout << total_paths-feasible_paths_count << "/" << total_paths << " infeasible paths (" << (total_paths-feasible_paths_count)*100/total_paths << "%)." << endl;
		cout << infeasible_paths_count << " infeasible path(s) found.";
		if(!(dbg_flags&DBG_NO_TIME))
		{
		    std::ios_base::fmtflags oldflags = std::cout.flags();
		    std::streamsize oldprecision = std::cout.precision();
			std::cout << std::fixed << std::setprecision(3) << " (" << ((float)exec_time_ms)/1000.f << "s)" << std::endl;
		    std::cout.flags(oldflags);
		    std::cout.precision (oldprecision);
		}
	}
}

// TODO: do something prettier here, maybe with a operator== on OrderedPath to use contains.... or just use Sets with a Comparator...
void Analysis::removeDuplicateInfeasiblePaths()
{
	Vector<OrderedPath> new_ips;
	for(Vector<OrderedPath>::Iterator ips_iter(infeasible_paths); ips_iter; )
	{
		OrderedPath op = *ips_iter;
		infeasible_paths.remove(ips_iter);
		bool contains = false;
		{
			for(Vector<OrderedPath>::Iterator ip_iter(infeasible_paths); ip_iter; ip_iter++)
			{
				bool equals = true;
				// check *ip_iter == op
				if(ip_iter.item().count() == op.count())
				{
					OrderedPath::Iterator iter1(ip_iter.item());
					for(OrderedPath::Iterator iter2(op); iter2; iter1++, iter2++)
					{
						if(!(iter1.item() == iter2.item()))
						{
							equals = false;
							break;
						}
					}
				}
				else
					equals = false;
				//if(*ip_iter == op)
				if(equals)
				{
					contains = true;
					break;
				}
			}
		}
		//if(!infeasible_paths.contains(op))
		if(!contains)
			new_ips.add(op);
	}
	infeasible_paths.clear();
	infeasible_paths = new_ips;
}

// debugs to do on path end
void Analysis::onPathEnd()
{
	feasible_paths_count++;
	if(flags&SUPERSILENT)
		return;
	if(dbg_flags&DBG_NO_DEBUG && !(dbg_flags&DBG_NO_PREANALYSIS))
	{
		if((++paths_count % 100) == 0 || (total_paths <= 1000 && paths_count <= 1000))
		{
			// cout << "(" << paths_count << "/" << total_paths << ")";
			// if(infeasible_paths_count)
			// 	cout << " !*" << infeasible_paths_count % 100;
			// cout << endl;
		}
	}
	else DBG(color::BBla() << color::On_Yel() << "EXIT block reached")
}

// debugs to do when we find an infeasible path
void Analysis::onAnyInfeasiblePath()
{
	if(flags&SUPERSILENT)
		return;
	if(dbg_flags&DBG_NO_DEBUG && !(dbg_flags&DBG_NO_PREANALYSIS))
	{
		infeasible_paths_count++;
		if((++paths_count % 100) == 0 || (total_paths <= 1000 && paths_count <= 1000))
		{
			// cout << "(" << paths_count << "/" << total_paths << ") !*" << infeasible_paths_count%100 << endl;
		}
	}
	else DBG(color::BIYel() << "Stopping current path analysis")
}

bool Analysis::isAHandledEdgeKind(Edge::kind_t kind) const
{
	// return (kind == Edge::TAKEN) || (kind == Edge::NOT_TAKEN) || (kind == Edge::VIRTUAL) || (kind == Edge::VIRTUAL_RETURN);
	switch(kind)
	{
		case Edge::VIRTUAL_CALL:
			return flags&FOLLOW_CALLS;
		case Edge::TAKEN:
		case Edge::NOT_TAKEN:
		case Edge::VIRTUAL:
		case Edge::VIRTUAL_RETURN:
			return true;
		case Edge::NONE:
		case Edge::CALL:
		case Edge::EXN_CALL:
		case Edge::EXN_RETURN:
		default:
			return false;
	}
}

bool Analysis::isConditional(BasicBlock* bb) const
{
	int count = 0;
	bool atLeastOneTaken = false;
	for(BasicBlock::OutIterator outs(bb); outs; outs++)
	{
		if(isAHandledEdgeKind(outs->kind()))
		{
			count++;
			atLeastOneTaken |= (outs->kind() == Edge::TAKEN);
		}
	}
	return atLeastOneTaken && (count > 1);
}

/**
	@fn int Analysis::allIncomingNonBackEdgesAreAnnotated(BasicBlock* bb, const Identifier<SLList<Analysis::State> >& annotation_identifier) const;
	returns true when all incoming non-back edges (look edges) are annotated with the given annotation
	@param bb BasicBlock to check
	@param annotation_identifier the annotation id we test on the incoming edges
*/
bool Analysis::allIncomingNonBackEdgesAreAnnotated(BasicBlock* bb, const Identifier<SLList<Analysis::State> >& annotation_identifier) const
{
	for(BasicBlock::InIterator bb_ins(bb); bb_ins; bb_ins++)
		if(!BACK_EDGE(*bb_ins))
			if(!annotation_identifier.get(*bb_ins))
				return false;
	return true;
}

/**
 * @fn bool Analysis::isSubPath(const OrderedPath& included_path, const Edge* e, const Path& path_set) const;
 * Checks if 'included_path->e' is a part of the set of paths "path_set",
 * that is if 'included_path' includes all the edges in the Edge set of path_set, except for e
 * @return true if it is a subpath
*/
bool Analysis::isSubPath(const OrderedPath& included_path, const Edge* e, const Path& path_set) const
{
	for(Path::Iterator iter(path_set); iter; iter++)
		if(*iter != e && !included_path.contains(*iter))
			return false;
	return true;
}

elm::String Analysis::wlToString() const
{
	elm::String rtn = "[";
	bool first = true;
	for(Vector<BasicBlock*>::Iterator iter(wl); iter; iter++)
	{
		if(first) first = false; else
			rtn = rtn.concat((CString)", ");
		rtn = _ << rtn << (*iter)->number();
	}
	rtn = rtn.concat((CString)"]");
	return rtn;
}

elm::String Analysis::pathToString(const Path& path)
{
	elm::String str = "[";
	bool first = true;
	for(Analysis::Path::Iterator iter(path); iter; iter++)
	{
		if(first)
			first = false;
		else
			str = str.concat(_ << ", ");
		str = str.concat(_ << (*iter)->source()->number() << "->" << (*iter)->target()->number());
	}
	str = str.concat(_ << "]");
	return str;
}
