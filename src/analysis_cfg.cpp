/*
 * Macro analysis: skeleton of the analysis algorithm, defines the way we parse the CFG 
 */

#include <ctime>
#include <cmath>
#include <iostream> // std::cout
#include <iomanip> // std::setprecision
#include <elm/io/Output.h>
#include <elm/genstruct/SLList.h>
#include <otawa/cfg/Edge.h>
#include <otawa/cfg/BasicBlock.h>
#include <otawa/cfg/features.h> 
// #include <elm/sys/Thread.h> // multithreading

#include "analysis.h"
// #include "smt_job.h"
// #include "smt.h"
#ifdef SMT_SOLVER_CVC4
	#include "cvc4/cvc4_smt.h"
#elif SMT_SOLVER_Z3
	#include "z3/z3_smt.h"
#endif
#include "debug.h"

using namespace elm::genstruct;
using namespace elm::io;

Identifier<SLList<Analysis::State> > Analysis::PROCESSED_EDGES("IP analysis processed incoming edges"); //, SLList<State>::null);
Identifier<Analysis::State*> 		 Analysis::PROCESSED_LOOPHEADER_BB("IP analysis processed loop headers (for fixpoints)");
Identifier<bool>			 		 Analysis::MOTHERLOOP_FIXPOINT_STATE("IP analysis fixpoint state info on loop exit edges");
Identifier<bool>			 		 Analysis::FIXPOINT_REACHED("IP analysis fixpoint state info on loop headers"); // for enable_smt

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
		path.addLast(*outs);
		constants.set(sp, SP, Set<Edge*>::null, false); // set that ?13==SP (since SP is the value of ?13 at the beginning of the program)
	}
}

Analysis::State::State(Edge* entry_edge, const dfa::State* state, const OperandVar& sp, unsigned int max_tempvars, unsigned int max_registers, bool init)
	: dfa_state(state), sp(sp), constants(max_tempvars, max_registers), fixpoint(false)
{
	generated_preds.clear(); // generated_preds := [[]]
	labelled_preds.clear(); // labelled_preds := [[]]
	if(init)
	{
		path.addLast(entry_edge);
		constants.set(sp, SP, Set<Edge*>::null, false); // set that ?13==SP (since SP is the value of ?13 at the beginning of the program)
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
	constants.label(e); // label the constants as well
}

void Analysis::State::printFixPointState() const
{
	if(dbg_verbose > DBG_VERBOSE_ALL)
		return; // save some time
	if(!path.hasAnEdge())
	{
		DBG("Failed to print fixpoint state: the state path is empty")
	}
	else
	{
		BasicBlock *bb = path.lastEdge()->target();
		DBG("Printing fixpoint state of BB#" << bb->number())
		if(LOOP_HEADER(bb))
			DBG("\t* loop#" << bb->number() << " = " << DBG_TEST(FIXPOINT_REACHED.exists(bb) && FIXPOINT_REACHED(bb) == true, true))
		while(ENCLOSING_LOOP_HEADER.exists(bb))
		{
			bb = ENCLOSING_LOOP_HEADER(bb);
			DBG("\t* loop#" << bb->number() << " = " << DBG_TEST(FIXPOINT_REACHED.exists(bb) && FIXPOINT_REACHED(bb) == true, true))
		}
	}
}

/**
 * @fn void Analysis::processCFG(CFG* cfg);
 * Runs the analysis on the CFG cfg
 * *WARNING*: atm, this function assumes we have NO LOOPS!
*/
void Analysis::processCFG(CFG* cfg)
{
	DBG("Using SMT solver: " << (flags&DRY_RUN ? "(none)" : SMT::printChosenSolverInfo()))
	DBG(color::Whi() << "Processing CFG " << cfg)
	total_paths = loop_header_count = 0;
	int processed_bbs = 0;
	placeboProcessCFG(cfg);
	std::time_t timestamp = clock(); // Timestamp before analysis

	/* begin */
	/* wl <- {ɛ} */
	wl.push(cfg->firstBB());
	/* lock[] <- {{}}; */
	BasicBlock::OutIterator entry_outs(cfg->entry());
	SLList<Analysis::State> entry_annotation;
	entry_annotation += Analysis::State(cfg->entry(), dfa_state, sp, max_tempvars, max_registers);
	PROCESSED_EDGES(*entry_outs) = entry_annotation;

	/* While wl != ∅ */
	while(!wl.isEmpty())
	{
		DBG("wl=" << wlToString())
#ifdef DBGG
		cout << color::Blu() << "wl=" << wlToString() << color::RCol() << endl;
#endif
		/* bb::wl <- wl; */
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
		purgeStateList(sl); // remove all invalid states
		cleanIncomingBackEdges(bb);//, PROCESSED_EDGES);

		// in case of a loop, merge the state list into a single state, and do lot of annotation stuff
		if(loop_header && !sl.isEmpty())
			processLoopHeader(bb, sl); //, PROCESSED_LOOPHEADER_BB, MOTHERLOOP_FIXPOINT_STATE, FIXPOINT_REACHED
		else
			cleanIncomingEdges(bb);//, PROCESSED_EDGES);

		bool fixpoint = true;
		int state_count = 0;
		/* For s in sl */
		for(SLList<Analysis::State>::MutableIterator sl_iter(sl); sl_iter; )
		{
			state_count++;
			if(dbg_verbose == DBG_VERBOSE_ALL)
				DBG(color::Whi() << "Processing path " << sl_iter.item().getPathString())
			fixpoint = sl_iter.item().fixpointState();
			#ifdef DBGG
			if(dbg_verbose <= DBG_VERBOSE_MINIMAL)
				cout << "BB #" << bb->number() << ", fixpoint=" << DBG_TEST(fixpoint, true) << endl;
			#endif
			/* processBB(s, bb); */
			if(processBB(sl_iter.item(), bb) > 0)
				sl.remove(sl_iter); // path ended
			else sl_iter++;
		}

		/* End For */
		// merging state lists that got too large
		if(!mergeOversizedStateList(sl))
			if(state_count >= 50 && dbg_verbose > DBG_VERBOSE_ALL && dbg_verbose < DBG_VERBOSE_RESULTS_ONLY)
				cout << " " << state_count << " states updated." << endl;

		bool enable_smt = true;
		// TODO! I want to make sure that when we have 3 loops if lvl1=no fixpoint, lvl2=fixpoint, and lvl3 finds a fixpoint, we don't enable smt...
		/* For e in bb.outs */
		for(BasicBlock::OutIterator bb_outs(bb); bb_outs; bb_outs++)
		{
			// on call return, program stacking: remove obsolete predicates featuring stack addresses lower than current sp
			if(bb_outs->kind() == Edge::VIRTUAL_RETURN)
			{
				if(const Option<Constant> current_sp = getCurrentStackPointer(sl))
				{
					DBG(color::IYel() << "Return reached, invalidating stack below " << *current_sp) // invalid states have been purged at this point
					for(SLList<Analysis::State>::MutableIterator sl_iter(sl); sl_iter; sl_iter++)
						sl_iter.item().invalidateStackBelow(*current_sp);
				}
				else DBG(color::IYel() << "Return reached, but sp value is lost or variable")
			}
			if(isAHandledEdgeKind(bb_outs->kind())) // filter out calls etc // not
			{
				if(BACK_EDGE(*bb_outs))
					DBG(color::Whi() << "End of loop reached.")
				if(!fixpoint) // fixpoint not found
				{
					if(!LOOP_EXIT_EDGE(*bb_outs)) // take everything but loop exit edges
					{	// adds to PROCESSED_EDGE
						enable_smt = edgeIsExitingToLoopLevel0(bb_outs); // only call smt if this is "level 0" sequential code
						processOutEdge(*bb_outs, sl, isConditional(bb), enable_smt); // annotate regardless of returned new_sl being empty or not
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
						enable_smt = true;
						if(LOOP_EXIT_EDGE(*bb_outs)) // exiting the loop
						{
							for(SLList<Analysis::State>::MutableIterator sl_iter(sl); sl_iter; sl_iter++)
								sl_iter.item().onLoopExit(loop_header ? elm::some(bb) : ENCLOSING_LOOP_HEADER.get(bb)); 
							if(MOTHERLOOP_FIXPOINT_STATE.exists(*bb_outs))
							{
								bool new_fixpoint_state = MOTHERLOOP_FIXPOINT_STATE(*bb_outs);
								for(SLList<Analysis::State>::MutableIterator sl_iter(sl); sl_iter; sl_iter++)
									sl_iter.item().setFixpointState(new_fixpoint_state); // put the fixpoint flag on all states
								// enable_smt = false;	// doesn't sound right to call with enable_smt=true even when we're coming back to the motherloop
								enable_smt = new_fixpoint_state || edgeIsExitingToLoopLevel0(bb_outs); // enable smt if the parent loop has reached fixpoint or if theres no parent loop
								// TODO! test this change
							}
							else // only state entering the loop is the strange case where it's a propagated infeasible path
								enable_smt = false;
						}
						else // this is sort of dirty, but it comes from the above code altering sl for all iterations of bb_outs when really it shouldn't... but copying is costly
						{
							for(SLList<Analysis::State>::MutableIterator sl_iter(sl); sl_iter; sl_iter++)
							{
								sl_iter.item().setFixpointState(true); // ensure this
								// unrelated: we also need to trigger calls 
								if(sl_iter.item().fixpointState() && bb_outs->kind() == Edge::VIRTUAL_CALL) // not handling non virtual Edge::CALL I guess?
									sl_iter.item().onCall(*bb_outs); // don't write calls that are also LOOP_EXIT_EDGE / TODO! double check this is ok
								if(sl_iter.item().fixpointState() && bb_outs->kind() == Edge::VIRTUAL_RETURN && RETURN_OF.exists(bb_outs->source()))
									sl_iter.item().onReturn(RETURN_OF(bb_outs->source())); // don't write returns that are also LOOP_EXIT_EDGE
							}
						}
						enable_smt = enable_smt && fixpointFoundOnAllMotherLoops(bb);
						#ifdef DBGG
							cout << "enable_smt=" << DBG_TEST(enable_smt, true) << ", shouldEnableSolver=" << DBG_TEST(shouldEnableSolver(*bb_outs), true) << endl;
						#endif
						processOutEdge(*bb_outs, sl, isConditional(bb), enable_smt); // annotate regardless of returned new_sl being empty or not 
						if(!wl.contains(bb_outs->target()))
							wl.push(bb_outs->target());
					}
				}
			}
		}
		/* End For */
		debugProgress(bb->number(), enable_smt);
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
void Analysis::processOutEdge(Edge* e, const SLList<Analysis::State>& sl, bool is_conditional, bool enable_smt)
{
	/* wl <- sl ⊙ e; */
	PROCESSED_EDGES.remove(e); // cleaning
	if(sl.isEmpty())
	{	// just propagate the {bottom}
		State invalid_state;
		PROCESSED_EDGES.ref(e).addFirst(invalid_state);
		return;
	}
	bool is_call = e->kind() == Edge::VIRTUAL_CALL && !BACK_EDGE(*e);
	if(!enable_smt || !shouldEnableSolver(e))
	{
		for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++)
		{
			State s = *sl_iter; // copy into new state
			if(is_call)
				s.onCall(e);
			s.appendEdge(e, is_conditional);
			PROCESSED_EDGES.ref(e).addFirst(s);
		}
		return;
	}

	// SMT is enabled
	SLList<Option<Path> > sl_paths;
	// get a list of infeasible paths matching the list of state "sl_paths"
	stateListToInfeasiblePathList(sl_paths, sl, e, is_conditional, is_call);
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
				addDisorderedInfeasiblePath(infeasible_path, s.getDetailedPath(), e); // infeasible_paths += order(infeasible_path); to output proprer ffx!
				DBG(color::On_IRed() << "Inf. path found: " << pathToString(infeasible_path))
				s.printFixPointState();
			}
			else // we found a counterexample, e.g. a feasible path that is included in the set of paths we marked as infeasible
			{
				DBG("   counterexample: " << counterexample)
				unminimized_ip_count++;
				if(flags&UNMINIMIZED_PATHS)
				{
					// falling back on full path (not as useful as a result, but still something)
					DetailedPath original_full_path = s.getDetailedPath();
					original_full_path.addLast(e); // need to add e
					// infeasible_paths.add(original_full_path);
					addDetailedInfeasiblePath(original_full_path);
					if(dbg_verbose == DBG_VERBOSE_ALL)
					{
						Path ofp;
						for(DetailedPath::EdgeIterator original_full_orderedpath_iter(original_full_path); original_full_orderedpath_iter; original_full_orderedpath_iter++)
							ofp += *original_full_orderedpath_iter;
						DBG(color::On_IRed() << "Inf. path found: " << pathToString(ofp) << color::RCol() << " (unrefined)")
					}
					// TODO: do a C) where we still try to refine this infeasible path?
				}
			}
			onAnyInfeasiblePath();
		}
	}
}

void Analysis::processLoopHeader(BasicBlock* bb, SLList<Analysis::State>& sl)
{
	State* s = new State((Edge*)NULL, dfa_state, sp, max_tempvars, max_registers, false); // entry is cleared anyway
	s->merge(sl); // merge paths properly (preserve calls)
	s->setFixpointState(Analysis::listOfFixpoints(sl));
#	ifdef DBGG
		cout << color::IPur() << "processLoopHeader after merge, s=" << *s << ", fixpt=" << DBG_TEST(s->fixpointState(), true) << color::RCol() << io::endl;
#	endif
	DBG("merged: constants=" << s->getConstants() << ", labelled_preds=" << s->getLabelledPreds());
	
	bool delete_s = false;
	if(PROCESSED_LOOPHEADER_BB.exists(bb))
	{
		if(s->fixpointState())
		{
			// don't compute anything, just extract the fixpoint we already know
			delete s;
			s = PROCESSED_LOOPHEADER_BB(bb);
			s->setFixpointState(true); // ensure we keep this property
		}
		else
		{
			const State* prev_s = PROCESSED_LOOPHEADER_BB(bb);
			ASSERT(prev_s != NULL);
			if(s->isFixPoint(*prev_s))
			{
				if(dbg_verbose < DBG_VERBOSE_RESULTS_ONLY)
					cout << color::Bold() << "FIXPOINT on BB#" << bb->number() << color::RCol() << endl;
				s->setFixpointState(true);
				delete_s = true;
				// we need to clear the annotation for future fixpoints on this loop 
				// // huh, not really the reason why. If this was a fixpoint in a previous analysis it'll be one in future ones
				// PROCESSED_LOOPHEADER_BB.remove(bb); //  this makes absurd results...
			}
			else
			{
				PROCESSED_LOOPHEADER_BB.ref(bb) = s;
				delete prev_s;
			}
		}
	}
	else
		PROCESSED_LOOPHEADER_BB.ref(bb) = s;

	ASSERTP(EXIT_LIST.exists(bb), "Loop header's EXIT_LIST does not exist. Maybe an infinite loop?");
	for(Vector<Edge*>::Iterator exits_iter(*EXIT_LIST.use(bb)); exits_iter; exits_iter++)
		MOTHERLOOP_FIXPOINT_STATE.ref(*exits_iter) = sl.first().fixpointState();  // annotate all the loop exit edges (bit hacky :-/)
	sl.clear();
	if(s->fixpointState())
		s->onLoopEntry(bb);
	sl += *s; // sl <- {s}
	FIXPOINT_REACHED.set(bb, s->fixpointState());
#	ifdef DBGG
		cout << color::IPur() << "end of processLoopHeader, s=" << *s << color::RCol() << io::endl;
#	endif
	if(delete_s)
		delete s;
}

void Analysis::stateListToInfeasiblePathList(SLList<Option<Path> >& sl_paths, const SLList<Analysis::State>& sl, Edge* e, bool is_conditional, bool is_call)
{
	//*
	for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++)
	{
		State s = *sl_iter;
		s.appendEdge(e, is_conditional);
		if(is_call)
			s.onCall(e);

		if(flags&DRY_RUN) // no SMT call
		{
			sl_paths.addLast(elm::none);
			PROCESSED_EDGES.ref(e).addFirst(s);
			continue;
		}
			
		// SMT call
#ifdef SMT_SOLVER_CVC4
		CVC4SMT smt;
#elif SMT_SOLVER_Z3
		Z3SMT smt;
#endif
		const Option<Path>& infeasible_path = smt.seekInfeasiblePaths(s);
		// mutex START
		sl_paths.addLast(infeasible_path);
		if(infeasible_path)
		{
			// we need to add some sort of {} to say this is an infeasible path
			State invalid_state;
			PROCESSED_EDGES.ref(e).addFirst(invalid_state);
			#ifdef DBG_WARNINGS
				if((*infeasible_path).contains(s.lastEdge())) // make sure the last edge was relevant in this path
					cerr << "WARNING: !infeasible_path->contains(s.lastEdge())" << endl;
			#endif
		}
		else
			PROCESSED_EDGES.ref(e).addFirst(s);
		// mutex END
	}
	//*/
	// TODO: rewrite with addFirst instead of addLast to optimize
	// TODO: replace both SLLists with Vector
	// first off, run threads
	/*
	SLList<elm::sys::Thread*> threads;
	SLList<SMTJob*> jobs;
	for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++)
	{
		State s = *sl_iter;
		s.appendEdge(e, is_conditional);
		SMTJob* job = new SMTJob(s); // runnable
		elm::sys::Thread* t = elm::sys::Thread::make(*job);
		t->start();
		threads.addLast(t);
		jobs.addLast(job);
	}

	// then, join and get result
	SLList<SMTJob*>::Iterator jobs_iter(jobs);
	for(SLList<elm::sys::Thread*>::Iterator threads_iter(threads); threads_iter; threads_iter++, jobs_iter++)
	{
		threads_iter->join();
		const Option<Path>& infeasible_path = jobs_iter.item()->getResult(); // TODO is this ok? o_o
		const State& s = jobs_iter.item()->getState();
		sl_paths.addLast(infeasible_path);
		if(infeasible_path)
		{
			// we need to add some sort of {} to say this is an infeasible path
			State invalid_state;
			processed_edges_id.ref(e).addFirst(invalid_state);
		}
		else
			processed_edges_id.ref(e).addFirst(s);
		delete *jobs_iter;
	}
	//*/
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
		if(!*sl_paths_subiter && isSubPath((*sl_subiter).getDetailedPath().toOrderedPath(), e, infeasible_path))
		{
			valid = false;
			counterexample = _ << (*sl_subiter).getPathString() << "+" << e->source()->number() << "->" << e->target()->number();
			break;
		}
	}
	return valid;
}

/**
 * @brief Reorder the infeasible path then add it to the list of paths (no unicity check)
 * 
 * @param ip Infeasible path to add
 * @param full_path Full path it originates from (must include ip)
 * @param last_edge Edge to add at the end of full_path
 */
void Analysis::addDisorderedInfeasiblePath(const Path& ip, const DetailedPath& full_path, Edge* last_edge)
{
	DetailedPath detailed_ip;
	// parse the detailed path and add all the edges that match the Path ip and the loop entries
	for(DetailedPath::Iterator full_path_iter(full_path); full_path_iter; full_path_iter++)
	{
		if(full_path_iter->isEdge())
		{
			if(ip.contains(full_path_iter->getEdge()))
				detailed_ip.addLast(*full_path_iter);
		}
		else
			detailed_ip.addLast(*full_path_iter);
	}
	if(ip.contains(last_edge)) // add the last edge too if in the infeasible path
		detailed_ip.addLast(last_edge);
#ifdef DBGG
	detailed_ip.optimize();
	DBGM("addDisorderedInfeasiblePath(...), ip=" << pathToString(ip) << ", " 
		<< color::ICya() << "full_path=[" << full_path << "]" << color::RCol() << ", result=" << detailed_ip); // TODO!
#endif
	addDetailedInfeasiblePath(detailed_ip);
}

void Analysis::addDetailedInfeasiblePath(const DetailedPath& ip)
{
	DetailedPath new_ip(ip);
	assert(ip.hasAnEdge()); // maybe a strong assumption, we'll see
	// first off, add loop entries for all missing loops
	BasicBlock *bb = ip.firstEdge()->source();
	if(LOOP_HEADER(bb))
		new_ip.addEnclosingLoop(bb);
	while(ENCLOSING_LOOP_HEADER.exists(bb))
	{
		bb = ENCLOSING_LOOP_HEADER(bb);
		new_ip.addEnclosingLoop(bb);
	}
	// optimize by removing redundancies
	new_ip.optimize();
	infeasible_paths.add(new_ip);
}

/**
 * @brief Remove all invalid states
 * 
 * @param sl list to purge
 */
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

/**
 * @brief If the list is bigger than a thresold, merge all states into one (a bit brutal)
 * 
 * @param sl List of states to process
 * @return Returns true iff a merge was done
 */
bool Analysis::mergeOversizedStateList(SLList<Analysis::State>& sl/*, int thresold*/) const
{
	if(!(flags&MERGE))
		return false;
	int count = sl.count();
	if(count >= state_size_limit)
	{
		State s((Edge*)NULL, dfa_state, sp, max_tempvars, max_registers, false); // entry is cleared anyway
		s.merge(sl);
		s.setFixpointState(Analysis::listOfFixpoints(sl));
		sl.clear();
		sl += s;
		if(dbg_verbose < DBG_VERBOSE_RESULTS_ONLY)
			cout << " " << count << " states updated then merged into 1." << endl;
		return true;
	}
	return false;
}

/**
 * @brief remove annotations from incoming edges to given BasicBlock
 * 
 * @param bb BasicBlock to process
 * @param processed_edges_id Identifier for processed edges
 */
void Analysis::cleanIncomingEdges(BasicBlock *bb) const
{
	for(BasicBlock::InIterator bb_ins(bb); bb_ins; bb_ins++)
		PROCESSED_EDGES.remove(*bb_ins);
		//PROCESSED_EDGES.ref(*bb_ins).clear();
}

/**
 * @brief remove annotations from incoming back edges
 * 
 * @param bb BasicBlock to process
 * @param processed_edges_id Identifier for processed (back) edges
 */
void Analysis::cleanIncomingBackEdges(BasicBlock *bb) const
{
	for(BasicBlock::InIterator bb_ins(bb); bb_ins; bb_ins++)
		if(BACK_EDGE(*bb_ins))
			PROCESSED_EDGES.remove(*bb_ins);
			// PROCESSED_EDGES.ref(*bb_ins).clear();
}

/**
 * @brief Checks if a fix point has been found on all parent loops
 */
bool Analysis::fixpointFoundOnAllMotherLoops(BasicBlock *bb) const
{
	while(ENCLOSING_LOOP_HEADER.exists(bb))
	{
		bb = ENCLOSING_LOOP_HEADER(bb);
		if(!FIXPOINT_REACHED.exists(bb) || FIXPOINT_REACHED(bb) != true)
			return false;
	}
	return true;
}

/**
 * @brief Test if the edge target has no mother loop.
 * 
 * @param e Edge to test
 * @return True if no mother loop was found at the target of the Edge e.
 */
bool Analysis::edgeIsExitingToLoopLevel0(const Edge* e) const
{
	return !LOOP_HEADER(e->target()) && !ENCLOSING_LOOP_HEADER.exists(e->target());
}

/**
 * @brief Checks if solver should be enabled
 * 
 * @param e Edge being processed
 * @return Returns true if solver should be enabled
 */
bool Analysis::shouldEnableSolver(const Edge* e)
{
	// return true; // TODO!!
	if(e->source()->outs.count() > 1)
		return true; // conditional edge, call...
	if(LOOP_EXIT_EDGE(*e))
		return true;
	return false;
	// TODO: improve, let's be smarter
}

/**
 * @brief Dry run on the CFG to find properties on it (!no longer used!)
 */
void Analysis::placeboProcessCFG(CFG* cfg)
{
	if(dbg_verbose > DBG_VERBOSE_ALL && dbg_verbose < DBG_VERBOSE_NONE)
		cout << color::ICya() << bb_count << color::RCol() << " BBs found." << endl;
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

// obsolete
void Analysis::placeboProcessBB(BasicBlock* bb)
{
	if(bb->isExit())
	{
		total_paths++;
		return;
	}
	if(LOOP_HEADER(bb))
		loop_header_count++;
	for(BasicBlock::OutIterator outs(bb); outs; outs++)
		if(!BACK_EDGE(*outs) && isAHandledEdgeKind(outs->kind())) // Filter out irrelevant edges (calls...)
			placeboProcessBB((*outs)->target());
}

/**
 * @brief Print results after a CFG analysis completes
 * 
 * @param exec_time_ms Measured execution time of the analysis (in ms)
 */
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
	for(Vector<DetailedPath>::Iterator iter(infeasible_paths); iter; iter++)
	{
		if(dbg_verbose == DBG_VERBOSE_ALL)
			DBG(color::IGre() << "    * [" << *iter << "]")
		else if(dbg_verbose < DBG_VERBOSE_NONE)
			cout << "    * [" << *iter << "]" << endl;
	}
	if(dbg_verbose > DBG_VERBOSE_ALL && dbg_verbose < DBG_VERBOSE_NONE)
	{
		cout << color::BIGre() << infeasible_paths_count << color::RCol() << " infeasible path(s) found.";
		if(!(dbg_flags&DBG_NO_TIME))
		{
		    std::ios_base::fmtflags oldflags = std::cout.flags();
		    std::streamsize oldprecision = std::cout.precision();
			std::cout << std::fixed << std::setprecision(3) << color::IYel().chars() << " (" << ((float)exec_time_ms)/1000.f << "s)" << color::RCol().chars() << std::endl;
		    std::cout.flags(oldflags);
		    std::cout.precision(oldprecision);
		}
		else
			cout << endl;
	}
	cout << "Minimized+Unminimized => Total w/o min. : " << color::On_Bla() << color::IGre() << infeasible_paths_count-unminimized_ip_count << color::RCol() <<
			"+" << color::Yel() << unminimized_ip_count << color::RCol() << " => " << color::IRed() << ip_count << color::RCol() << endl;
	if(dbg_flags&DBG_AVG_IP_LENGTH && infeasible_paths_count > 0)
	{
		int sum_path_lengths = 0, squaredsum_path_lengths = 0;
		for(Vector<DetailedPath>::Iterator iter(infeasible_paths); iter; iter++)
		{
			sum_path_lengths += iter->countEdges();
			squaredsum_path_lengths += iter->countEdges() * iter->countEdges();
		}
		float average_length = (float)sum_path_lengths / (float)infeasible_paths_count;
		float norm2 = sqrt((float)squaredsum_path_lengths / (float)infeasible_paths_count);
	    std::ios_base::fmtflags oldflags = std::cout.flags();
	    std::streamsize oldprecision = std::cout.precision();
		std::cout << std::fixed << std::setprecision(2) << " (Average: " << average_length << ", Norm2: " << norm2 << ")" << endl;
		std::cout.flags(oldflags);
		std::cout.precision(oldprecision);
	}
}

/**
 * @fn void Analysis::removeDuplicateInfeasiblePaths();
 * Look for infeasible paths that share the same ordered list of edges and remove duplicates 
 */
void Analysis::removeDuplicateInfeasiblePaths()
{
	Vector<DetailedPath> new_ips;
	for(Vector<DetailedPath>::Iterator dp_iter(infeasible_paths); dp_iter; )
	{
		DetailedPath dp(*dp_iter);
		infeasible_paths.remove(dp_iter);
		bool contains = false;
		for(Vector<DetailedPath>::Iterator dp_subiter(infeasible_paths); dp_subiter; dp_subiter++)
		{
			if(dp_subiter->weakEqualsTo(dp)) // if dp == *dp_subiter*
			{
				contains = true; // dp is equal to an element of infeasible_paths \ dp
				break;
			}
		}
		if(!contains)
			new_ips.add(dp);
	}
	infeasible_paths.clear();
	infeasible_paths = new_ips;
}

/**
 * @fn void Analysis::onPathEnd();
 * Print debugs due on path end
 */
void Analysis::onPathEnd()
{
	DBG(color::BBla() << color::On_Yel() << "EXIT block reached")
}

/**
 * @fn void Analysis::onAnyInfeasiblePath();
 * Print debugs due on finding an infeasible path
 */
void Analysis::onAnyInfeasiblePath()
{
	DBG(color::BIYel() << "Stopping current path analysis")
}

bool Analysis::isAHandledEdgeKind(Edge::kind_t kind) const
{
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

// either we find SP in all the paths we merge (and the same SP), either we return elm::none
Option<Constant> Analysis::getCurrentStackPointer(const SLList<Analysis::State>& sl) const
{
	Option<Constant> rtn = elm::none; // also acts like a bool first = true
	for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++)
	{
		if(sl_iter->getConstants().isConstant(sp))
		{
			if(rtn)
			{
				if(*rtn != sl_iter->getConstants()[sp])
					return elm::none; // paths have different SP value
			}
			else
				rtn = elm::some(sl_iter->getConstants()[sp]);
		}
		else
			return elm::none; // one of the paths has invalidated SP
	}
	return rtn;
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
			if(!annotation_identifier.exists(*bb_ins))
				return false;
	return true;
}

bool Analysis::allIncomingEdgesAreAnnotated(BasicBlock* bb, const Identifier<SLList<Analysis::State> >& annotation_identifier) const
{
	for(BasicBlock::InIterator bb_ins(bb); bb_ins; bb_ins++)
		if(!annotation_identifier.exists(*bb_ins))
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

/**
 * @brief Get pretty printing for any unordered Path (Set of Edge*)
 * 
 * @param path Path to parse
 * @return String representing the path
 */
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

/**
 * @brief Get pretty printing for any OrderedPath (SLList of Edge*)
 * 
 * @param path OrderedPath to parse
 * @return String representing the path
 */
elm::String Analysis::orderedPathToString(const OrderedPath& path)
{
	elm::String str;
	bool first = true;
	int lastid = 0; // -Wmaybe-uninitialized
	for(OrderedPath::Iterator iter(path); iter; iter++)
	{
		// if(!first && (*iter)->source()->number() != lastid)
		// {
		// 	DBG("str=" << str)
		// 	DBG("lastid=" << lastid << ", (*iter)->source->number()=" << (*iter))
		// }
		// when path is x->y and y'->z, there must be y=y'
		ASSERTP(first || (*iter)->source()->number() == lastid, "OrderedPath previous target and current source do not match! ex: 1->2, 2->4, 3->5");
		if(first)
		{
#			ifndef NO_UTF8
				if((*iter)->source()->number() == 0)
					str = _ << "ε";
				else
#			endif
				str = _ << (*iter)->source()->number();
			first = false;
		}
		str = _ << str << "->" << (*iter)->target()->number();
		lastid = (*iter)->target()->number();
	}
	if(str.isEmpty())
		return "(empty)";
	return str;
}
