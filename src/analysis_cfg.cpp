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

/*
void Analysis::initializeAnalysis()
{
	wl.add(Analysis::State(cfg->entry(), sp)); // wl <- {ɛ}
}
*/
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

void Analysis::State::appendEdge(Edge* e)
{
	// add edge to the end of the path
	this->path.addLast(e);
	// we now need to label the correct list of predicates
	const SLList<LabelledPredicate> &relevant_preds = (e->kind() == Edge::TAKEN) ?
		generated_preds_taken : // TAKEN
		generated_preds;  // NOT TAKEN, VIRTUAL, VIRTUAL RETURN

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
	placeboProcessCFG(cfg);
	std::time_t timestamp = clock(); // Timestamp before analysis
	
	/* *Algorithm*
	begin
		wl <- {ɛ}; // init to root
		lock[] <- {{}}; // not exactly an array struct

		While wl != ∅
			sl::wl <- wl; // pop
			bb <- sl[0].last.target(); // whichever element of il will do

			If bb.ins.count() > 1 // if we have to join several paths
				lock[bb] += sl; // adds new element sl

				If lock[bb].count() < bb.ins.count // we haven't explored all the incoming paths
		 			continue;
				End If
				sl <- mergeIntoOneList(lock[bb]); // for instance {{1,2},{3},{4}} becomes {1,2,3,4}
			End If

			For s in sl
				processBB(s, bb); // update s predicates
			End For

			For e in bb.outs
				wl <- sl ⊙ e; // ⊙ = une sorte de concaténation de langages
				// smt check here
			End For
		End While
	end
	*/

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
		DBG(color::IWhi() << "wl=" << wlToString())
		/* bb::wl <- wl; */
		BasicBlock *bb = wl.pop();
		SLList<Analysis::State> sl;
		bool is_loop_header = LOOP_HEADER(bb);
		
		if(dbg_flags&DBG_NO_DEBUG)
			cout << "Processing " << bb << (is_loop_header?" (loop header)":"") << endl;

		/*  If lock[bb].count() < bb.ins.count
				continue;
			End If
		*/
		if(!allIncomingNonBackEdgesAreAnnotated(bb, PROCESSED_EDGES))
			continue;
		/* sl <- mergeIntoOneList(lock[bb]); */
		for(BasicBlock::InIterator bb_ins(bb); bb_ins; bb_ins++)
			sl.addAll(*PROCESSED_EDGES(*bb_ins));
		// at this point of the algorithm, all incoming edges of bb have been processed

		// TODO! clean up below
		if(is_loop_header)
		{
			State s((Edge*)NULL, dfa_state, sp, max_tempvars, max_registers, false); // entry is cleared anyway
			// sl.remove(sl_iter);
			BasicBlock::OutIterator bb_outs(bb);
			while(BACK_EDGE(*bb_outs))
				bb_outs++;
			s.merge(sl, *bb_outs); // TODO!! is the back edge always the taken one? NO!!!
			sl.clear();
			sl += s; // sl <- {s}
		}

		/* For s in sl */
		for(SLList<Analysis::State>::MutableIterator sl_iter(sl); sl_iter; )
		{
			// if(is_loop_header) sl_iter.item().throwInfo();
			DBG(color::Whi() << "Processing path " << sl_iter.item().getPathString())
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
				else 
				{
					PROCESSED_EDGES(*bb_outs) = processOutEdge(*bb_outs, sl); // annotate regardless of returned new_sl being empty or not
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
SLList<Analysis::State> Analysis::processOutEdge(Edge* e, const SLList<Analysis::State>& sl)
{
	/* wl <- sl ⊙ e; */
	SLList<Analysis::State> new_sl; // this is what we return
	Vector<Option<Path> > sl_paths;
	for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++)
	{
		State s = *sl_iter;
		s.appendEdge(e);

		// SMT call
		SMT smt;
		Option<Path> infeasible_path = smt.seekInfeasiblePaths(s);
		sl_paths.push(infeasible_path);
		if(infeasible_path)
			ASSERT((*infeasible_path).contains(s.lastEdge())) // make sure the last edge was relevant in this path
		else
			new_sl += s;
	}
	Vector<Option<Path> >::Iterator sl_paths_iter(sl_paths);
	for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++, sl_paths_iter++)
	{
		const State& s = *sl_iter;
		// s.appendEdge(e);
		if(*sl_paths_iter) // is infeasible?
		{
			const Path& infeasible_path = **sl_paths_iter;
			DBG("Path " << s.getPathString() << "->" << e->target()->number() << " minimized to " << pathToString(infeasible_path))
			elm::String counterexample;
			bool valid = checkInfeasiblePathValidity(sl, sl_paths, e, infeasible_path, counterexample);
			DBG(color::BIWhi() << "B)" << color::RCol() << " Verifying minimized path validity... " << (valid?color::IGre():color::IRed()) << (valid?"SUCCESS!":"FAILED!"))
			if(valid)
			{
				infeasible_paths += infeasible_path;
				DBG(color::On_IRed() << "Inf. path found: " << pathToString(infeasible_path))
			}
			else // we found a counterexample, e.g. a feasible path that is included in the set of paths we marked as infeasible
			{
				DBG("   counterexample: " << counterexample)
				 // falling back on full path (not as useful as a result, but still something)
				OrderedPath original_full_orderedpath = (*sl_iter).getPath();
				Path original_full_path;
				for(OrderedPath::Iterator original_full_orderedpath_iter(original_full_orderedpath); original_full_orderedpath_iter; original_full_orderedpath_iter++)
					original_full_path += *original_full_orderedpath_iter;
				original_full_path += e; // need to add e
				infeasible_paths += original_full_path;
				DBG(color::On_IRed() << "Inf. path found: " << pathToString(original_full_path) << color::RCol() << " (unrefined)")
				// TODO: do a C) where we still try to refine this infeasible path
			}
			onAnyInfeasiblePath();
		}
	}
	return new_sl;
}

/**
 * @fn bool Analysis::checkInfeasiblePathValidity(const SLList<Analysis::State>& sl, const Vector<Option<Path> >& sl_paths, const Edge* e, const Path& infeasible_path, elm::String& counterexample);
 * Checks if the minimized list of edges we found 'infeasible_path' is valid,
 * that is if all the paths it represents (all the 'sl[i]->e') are infeasible ('sl_paths[i]' is not 'elm::option::none')
 * If invalid, returns a counter-example in counterexample.
 * @return true if valid
*/
bool Analysis::checkInfeasiblePathValidity(const SLList<Analysis::State>& sl, const Vector<Option<Path> >& sl_paths, const Edge* e, const Path& infeasible_path, elm::String& counterexample)
{
	bool valid = true;
	// check that all the paths going to the current BB are sound with the minimized inf. path we just found
	Vector<Option<Path> >::Iterator sl_paths_subiter(sl_paths);
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

// figures properties on the CFG without doing any actual analysis
void Analysis::placeboProcessCFG(CFG* cfg)
{
	if(dbg_flags&DBG_NO_DEBUG)
	{
		cout << "Running pre-analysis... ";
		placeboProcessBB(cfg->firstBB());
		cout << total_paths << " paths found." << endl;
	}
	else
	{
		DBG(color::Whi() << "Running pre-analysis... ")
		placeboProcessBB(cfg->firstBB());
		DBG(color::Whi() << total_paths << " paths found.")
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
		if(!BACK_EDGE(outs) && isAHandledEdgeKind(outs->kind())) // Filter out irrelevant edges (calls...)
			placeboProcessBB((*outs)->target());
}

// print result of a whole CFG analysis
void Analysis::printResults(int exec_time_ms) const
{
	int infeasible_paths_count = infeasible_paths.count();
	if(dbg_flags&DBG_NO_TIME)
		DBG(color::BIGre() << infeasible_paths_count << " infeasible path" << (infeasible_paths_count == 1 ? "" : "s") << " found: ")
	else
		DBG(color::BIGre() << infeasible_paths_count << " infeasible path" << (infeasible_paths_count == 1 ? "" : "s") << " found: "
			<< "(" << (exec_time_ms>=1000 ? ((float)exec_time_ms)/(float(100)) : exec_time_ms) << (exec_time_ms>=1000 ? "s" : "ms") << ")")
	for(Set<Path>::Iterator iter(infeasible_paths); iter; iter++)
	{
		Path l = *iter;
		bool first = true;
		elm::String str = "    * [";
		for(Path::Iterator subiter(l); subiter; subiter++)
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

// debugs to do on path end
void Analysis::onPathEnd()
{
	if(dbg_flags&DBG_NO_DEBUG)
	{
		if((++paths_count % 100) == 0 || total_paths <= 1000)
		{
			cout << "(" << paths_count << "/" << total_paths << ")";
			if(infeasible_paths_count)
				cout << " !*" << infeasible_paths_count;
			cout << endl;
			infeasible_paths_count = 0;
		}
	}
	else DBG(color::BBla() << color::On_Yel() << "EXIT block reached")
}

// debugs to do when we find an infeasible path
void Analysis::onAnyInfeasiblePath()
{
	if(dbg_flags&DBG_NO_DEBUG)
	{
		infeasible_paths_count++;
		if((++paths_count % 100) == 0 || total_paths <= 1000)
		{
			cout << "(" << paths_count << "/" << total_paths << ") !*" << infeasible_paths_count << endl;
			infeasible_paths_count = 0;
		}
	}
	else DBG(color::BIYel() << "Stopping current path analysis")
}

bool Analysis::isAHandledEdgeKind(Edge::kind_t kind) const
{
	return (kind == Edge::TAKEN) || (kind == Edge::NOT_TAKEN) || (kind == Edge::VIRTUAL) || (kind == Edge::VIRTUAL_RETURN);
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
