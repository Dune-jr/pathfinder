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
	: dfa_state(state), sp(sp), constants(max_tempvars, max_registers), fixpoint(false)
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
	: dfa_state(state), sp(sp), constants(max_tempvars, max_registers), fixpoint(false)
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
	: dfa_state(s.dfa_state), sp(s.sp), path(s.path), constants(s.constants), labelled_preds(s.labelled_preds), generated_preds(s.generated_preds), generated_preds_taken(s.generated_preds_taken), fixpoint(s.fixpoint)
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
	labelled_preds += labelPredicateList(relevant_preds, e);
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
	total_paths = loop_header_count = 0;
	int processed_bbs = 0;
	placeboProcessCFG(cfg);
	std::time_t timestamp = clock(); // Timestamp before analysis

	/* begin */
	/* wl <- {ɛ} */
	wl.push(cfg->firstBB());
	/* lock[] <- {{}}; */
	Identifier<SLList<Analysis::State> > PROCESSED_EDGES("IP analysis processed incoming edges"); //, SLList<State>::null);
	Identifier<Analysis::State*> 		 PROCESSED_LOOPHEADER_BB("IP analysis processed loop headers (for fixpoints)");
	Identifier<bool>			 		 MOTHERLOOP_FIXPOINT_STATE("IP analysis fixpoint state info on loop exit edges");
	Identifier<bool>			 		 FIXPOINT_REACHED("IP analysis fixpoint state info on loop headers"); // for enable_smt
	BasicBlock::OutIterator entry_outs(cfg->entry());
	SLList<Analysis::State> entry_annotation;
	entry_annotation += Analysis::State(cfg->entry(), dfa_state, sp, max_tempvars, max_registers);
	PROCESSED_EDGES(*entry_outs) = entry_annotation;

	/* While wl != ∅ */
	while(!wl.isEmpty())
	{
		DBG("wl=" << wlToString())
		/* bb::wl <- wl; */
#ifdef DBGG
		cout << color::Blu() << "wl=" << wlToString() << color::RCol() << endl;
#endif
		BasicBlock *bb = wl.pop();
		SLList<Analysis::State> sl;
		const bool loop_header = LOOP_HEADER(bb);

		/*  If lock[bb].count() < bb.ins.count
				continue;
			End If
		*/
		if(!allIncomingNonBackEdgesAreAnnotated(bb, PROCESSED_EDGES))
			continue;
		
		if(dbg_verbose > DBG_VERBOSE_ALL && dbg_verbose < DBG_VERBOSE_RESULTS_ONLY)
			cout << "[" << ++processed_bbs*100/bb_count << "%] Processing BB #" << bb->number() << " of " << bb_count << (loop_header?" (loop header)":"") << endl;
		/* sl <- mergeIntoOneList(lock[bb]); */
		for(BasicBlock::InIterator bb_ins(bb); bb_ins; bb_ins++)
			sl.addAll(*PROCESSED_EDGES(*bb_ins));
		// at this point of the algorithm, all incoming edges of bb have been processed
		purgeStateList(sl); // remove all invalidate states
		cleanIncomingBackEdges(bb, PROCESSED_EDGES);

		// in case of a loop, merge the state list into a single state, and do lot of annotation stuff
		if(loop_header && !sl.isEmpty())
			processLoopHeader(bb, sl, PROCESSED_LOOPHEADER_BB, MOTHERLOOP_FIXPOINT_STATE, FIXPOINT_REACHED);
		else
			cleanIncomingEdges(bb, PROCESSED_EDGES);

		bool fixpoint = true;
		int state_count = 0;
		/* For s in sl */
		for(SLList<Analysis::State>::MutableIterator sl_iter(sl); sl_iter; )
		{
			state_count++;
			if(dbg_flags&DBG_VERBOSE_ALL)
				DBG(color::Whi() << "Processing path " << sl_iter.item().getPathString())
			fixpoint = sl_iter.item().fixpointState();
			#ifdef DBGG
				cout << "BB #" << bb->number() << ", fixpoint=" << DBG_TEST(fixpoint, true) << endl;
			#endif
			/* processBB(s, bb); */
			if(processBB(sl_iter.item(), bb) > 0)
				sl.remove(sl_iter); // path ended
			else sl_iter++;
		}

		/* End For */
		// merging state lists that got too large
		mergeOversizedStateList(sl, 250);
		if(state_count >= 50 && dbg_verbose > DBG_VERBOSE_ALL && dbg_verbose < DBG_VERBOSE_RESULTS_ONLY)
			cout << " " << state_count << " states updated." << endl;

		/* For e in bb.outs */
		for(BasicBlock::OutIterator bb_outs(bb); bb_outs; bb_outs++)
		{
			if(isAHandledEdgeKind(bb_outs->kind())) // filter out calls etc
			{
				if(BACK_EDGE(*bb_outs))
					DBG(color::Whi() << "End of loop reached.")
				if(!fixpoint) // fixpoint not found
				{
					if(!LOOP_EXIT_EDGE(*bb_outs)) // take everything but loop exit edges
					{	// adds to PROCESSED_EDGE
						bool enable_smt = ENCLOSING_LOOP_HEADER.exists(bb_outs->target()) ? false : true; // only compute smt if this is "level 0" sequential code
						processOutEdge(*bb_outs, PROCESSED_EDGES, sl, isConditional(bb), enable_smt); // annotate regardless of returned new_sl being empty or not
						if(!wl.contains(bb_outs->target()))
							wl.push(bb_outs->target());
						#ifdef DBGG
							cout << "e=" << bb_outs->source()->number() << "->" << bb_outs->target()->number() << ", enable_smt=" << DBG_TEST(enable_smt, true) << endl;
						#endif
					}
				}
				else // fixpoint found
				{
					if(!BACK_EDGE(*bb_outs)) // take everything but back edges
					{	// adds to PROCESSED_EDGE
						bool enable_smt = true;
						if(LOOP_EXIT_EDGE(*bb_outs))
						{
							if(MOTHERLOOP_FIXPOINT_STATE.exists(*bb_outs))
							{
								bool new_fixpoint_state = MOTHERLOOP_FIXPOINT_STATE(*bb_outs);
								for(SLList<Analysis::State>::MutableIterator sl_iter(sl); sl_iter; sl_iter++)
									sl_iter.item().setFixpointState(new_fixpoint_state); // put the fixpoint flag on all states
								enable_smt = false;	// doesn't sound right to call with enable_smt=true even when we're coming back to the motherloop
							}
							else // only state entering the loop is the strange case where it's a propagated infeasible path
								enable_smt = false;
						}
						else // this is dirty, but it comes from the above code altering sl for all iterations of bb_outs when really it shouldn't... but copying is costly
							for(SLList<Analysis::State>::MutableIterator sl_iter(sl); sl_iter; sl_iter++)
								sl_iter.item().setFixpointState(true); // ensure this
						enable_smt = enable_smt && fixpointFoundOnAllMotherLoops(bb, FIXPOINT_REACHED);
						#ifdef DBGG
							cout << "enable_smt=" << DBG_TEST(enable_smt, true) << endl;
						#endif
						processOutEdge(*bb_outs, PROCESSED_EDGES, sl, isConditional(bb), enable_smt); // annotate regardless of returned new_sl being empty or not 
						if(!wl.contains(bb_outs->target()))
							wl.push(bb_outs->target());
					}
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
void Analysis::processOutEdge(Edge* e, const Identifier<SLList<Analysis::State> >& processed_edges_id, const SLList<Analysis::State>& sl, bool is_conditional, bool enable_smt)
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

		if(enable_smt)
		{
			// SMT call
			SMT smt;
			const Option<Path>& infeasible_path = smt.seekInfeasiblePaths(s);
			sl_paths.addLast(infeasible_path);
			if(infeasible_path)
			{
				// we need to add some sort of {} to say this is an infeasible path
				State invalid_state;
				processed_edges_id.ref(e).addFirst(invalid_state);
#ifdef DBG_WARNINGS
				if((*infeasible_path).contains(s.lastEdge())) // make sure the last edge was relevant in this path
					cerr << "WARNING: !infeasible_path->contains(s.lastEdge())" << endl;
#endif
			}
			else
				processed_edges_id.ref(e).addFirst(s);
		}
		else
			processed_edges_id.ref(e).addFirst(s);			
	}
	if(!enable_smt) 
		return;
	SLList<Option<Path> >::Iterator sl_paths_iter(sl_paths);
	for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++, sl_paths_iter++)
	{
		const State& s = *sl_iter;
		if(*sl_paths_iter) // is infeasible?
		{
			const Path& infeasible_path = **sl_paths_iter;
			elm::String counterexample;
			DBG("Path " << s.getPathString() << "->" << e->target()->number() << " minimized to " << pathToString(infeasible_path))
			bool valid = /*false&&*/checkInfeasiblePathValidity(sl, sl_paths, e, infeasible_path, counterexample);
			DBG(color::BIWhi() << "B)" << color::RCol() << " Verifying minimized path validity... " << (valid?color::IGre():color::IRed()) << (valid?"SUCCESS!":"FAILED!"))
			ip_count++;
			if(valid)
			{
				addDisorderedPath(infeasible_path, (*sl_iter).getPath(), e); // infeasible_paths += order(infeasible_path); to output proprer ffx!
				DBG(color::On_IRed() << "Inf. path found: " << pathToString(infeasible_path))
			}
			else // we found a counterexample, e.g. a feasible path that is included in the set of paths we marked as infeasible
			{
				unminimized_ip_count++;
				DBG("   counterexample: " << counterexample)
				 // falling back on full path (not as useful as a result, but still something)
				OrderedPath original_full_path = (*sl_iter).getPath();
				original_full_path.addLast(e); // need to add e
				infeasible_paths.add(original_full_path);
				// TODO! clean that mess below...
				if(dbg_verbose == DBG_VERBOSE_ALL)
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

void Analysis::processLoopHeader(BasicBlock* bb, SLList<Analysis::State>& sl, const Identifier<Analysis::State*>& processed_loopheader_bb_id,
	const Identifier<bool>& motherloop_fixpoint_state_id, const Identifier<bool>& fixpoint_reached_id)
{
	State* s = new State((Edge*)NULL, dfa_state, sp, max_tempvars, max_registers, false); // entry is cleared anyway
	s->merge(sl);//, *bb_outs);
	s->setFixpointState(Analysis::listOfFixpoints(sl));
	#ifdef DBGG
		cout << "merged:" << s->dumpEverything();
	#endif
	if(processed_loopheader_bb_id.exists(bb))
	{
		if(s->fixpointState())
		{
			// don't compute anything, just extract the fixpoint we already know
			delete s;
			s = processed_loopheader_bb_id(bb);
			s->setFixpointState(true); // ensure we keep this property
		}
		else
		{
			const State* prev_s = processed_loopheader_bb_id(bb);
			ASSERT(prev_s != NULL);
			if(s->isFixPoint(*prev_s))
			{
				if(dbg_verbose < DBG_VERBOSE_RESULTS_ONLY)
					cout << color::Bold() << "FIXPOINT on BB#" << bb->number() << color::RCol() << endl;
				s->setFixpointState(true);
				// we need to clear the annotation for future fixpoints on this loop 
				// // huh,not really the reason why. If this was a fixpoint in a previous analysis it'll be one in future ones
				// processed_loopheader_bb_id.remove(bb); //  this makes absurd results...
			}
			else
			{
				processed_loopheader_bb_id.ref(bb) = s;
				delete prev_s;
			}
		}
	}
	else
		processed_loopheader_bb_id.ref(bb) = s;

	for(Vector<Edge*>::Iterator exits_iter(*EXIT_LIST.use(bb)); exits_iter; exits_iter++)
		motherloop_fixpoint_state_id.ref(*exits_iter) = sl.first().fixpointState();  // annotate all the loop exit edges (bit hacky :-/)
	sl.clear();
	sl += *s; // sl <- {s}
	fixpoint_reached_id.set(bb, s->fixpointState());
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

void Analysis::mergeOversizedStateList(SLList<Analysis::State>& sl, int thresold) const
{
	if(!(flags&MERGE))
		return;
	int count = sl.count();
	if(count >= thresold)
	{
		State s((Edge*)NULL, dfa_state, sp, max_tempvars, max_registers, false); // entry is cleared anyway
		s.merge(sl);
		s.setFixpointState(Analysis::listOfFixpoints(sl));
		sl.clear();
		sl += s;
		if(dbg_verbose < DBG_VERBOSE_RESULTS_ONLY)
			cout << " " << count << " states updated then merged into 1." << endl;
	}
}

void Analysis::cleanIncomingEdges(BasicBlock *bb, const Identifier<SLList<Analysis::State> >& processed_edges_id) const
{
	for(BasicBlock::InIterator bb_ins(bb); bb_ins; bb_ins++)
		processed_edges_id.remove(*bb_ins);
		//processed_edges_id.ref(*bb_ins).clear();
}

void Analysis::cleanIncomingBackEdges(BasicBlock *bb, const Identifier<SLList<Analysis::State> >& processed_edges_id) const
{
	for(BasicBlock::InIterator bb_ins(bb); bb_ins; bb_ins++)
		if(BACK_EDGE(*bb_ins))
			processed_edges_id.remove(*bb_ins);
			// processed_edges_id.ref(*bb_ins).clear();
}

bool Analysis::fixpointFoundOnAllMotherLoops(BasicBlock *bb, const Identifier<bool>& fixpoint_found_id) const
{
	while(ENCLOSING_LOOP_HEADER.exists(bb))
	{
		bb = ENCLOSING_LOOP_HEADER(bb);
		if(!fixpoint_found_id.exists(bb) || fixpoint_found_id(bb) != true)
			return false;
	}
	return true;
}

// figures properties on the CFG without doing any actual analysis
void Analysis::placeboProcessCFG(CFG* cfg)
{
	if(dbg_verbose > DBG_VERBOSE_ALL && dbg_verbose < DBG_VERBOSE_NONE)
		cout << bb_count << " BBs found." << endl;
	else
		DBG(color::Whi() << total_paths << " paths found.")
	if(!(dbg_flags&DBG_PREANALYSIS))
	{
		total_paths = 777;
		return;
	}
	if(dbg_verbose > DBG_VERBOSE_ALL && dbg_verbose < DBG_VERBOSE_NONE)
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
	if(dbg_verbose == DBG_VERBOSE_NONE)
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
		if(dbg_verbose > DBG_VERBOSE_ALL && dbg_verbose < DBG_VERBOSE_NONE)
			cout << str << endl;
	}
	if(dbg_verbose > DBG_VERBOSE_ALL && dbg_verbose < DBG_VERBOSE_NONE)
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
		else
			cout << endl;
	}
	cout << "Total IP count: " << ip_count << ", Unminimized IP count:" << unminimized_ip_count << endl;
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
				if(equals)
				{
					contains = true;
					break;
				}
			}
		}
		if(!contains)
			new_ips.add(op);
	}
	infeasible_paths.clear();
	infeasible_paths = new_ips;
}

// debugs to do on path end
void Analysis::onPathEnd()
{
	DBG(color::BBla() << color::On_Yel() << "EXIT block reached")
}

// debugs to do when we find an infeasible path
void Analysis::onAnyInfeasiblePath()
{
	DBG(color::BIYel() << "Stopping current path analysis")
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

bool Analysis::allIncomingEdgesAreAnnotated(BasicBlock* bb, const Identifier<SLList<Analysis::State> >& annotation_identifier) const
{
	for(BasicBlock::InIterator bb_ins(bb); bb_ins; bb_ins++)
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
