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
// #include <elm/sys/Thread.h> // multithreading
#include "analysis.h"
#include "debug.h"
#ifdef SMT_SOLVER_CVC4
	#include "cvc4/cvc4_smt.h"
#elif SMT_SOLVER_Z3
	#include "z3/z3_smt.h"
#endif

using namespace elm::genstruct;
using namespace elm::io;

// Identifier<SLList<Analysis::State> >		Analysis::PROCESSED_EDGES("IP analysis processed incoming edges"); //, SLList<State>::null);
// Identifier<Analysis::State*> 			Analysis::PROCESSED_LOOPHEADER_BB("IP analysis processed loop headers (for fixpoints)");
// Identifier<bool>			 				Analysis::MOTHERLOOP_FIXPOINT_STATE("IP analysis fixpoint state info on loop exit edges");
// Identifier<bool>							Analysis::FIXPOINT_REACHED("IP analysis fixpoint state info on loop headers"); // for enable_smt
// Identifier<Analysis::loopheader_status_t>Analysis::LOOPHEADER_STATUS("On loop headers, status of the fixpoint analysis");
// Identifier<Analysis::State>				Analysis::LOOPHEADER_STATE("On loop headers, current state we are working on to get a fixpoint");
Identifier<SLList<Analysis::State> >		Analysis::EDGE_S("Trace on an edge"); // old PROCESSED_EDGES  //TODO! try vector
Identifier<Analysis::State>					Analysis::LH_S("Trace on a loop header"); // maybe change to vector
Identifier<Analysis::loopheader_status_t>	Analysis::LH_STATUS("Fixpt status of a loop (on a loop header)");

Analysis::State::State() : dfa_state(NULL), sp(0), bottom(false), constants() { }

Analysis::State::State(const context_t& context)
	: dfa_state(context.dfa_state), sp(context.sp), bottom(true), constants(context.max_tempvars, context.max_registers) { }

Analysis::State::State(Block* entryb, const context_t& context, bool init)
	: dfa_state(context.dfa_state), sp(context.sp), bottom(false), constants(context.max_tempvars, context.max_registers)//, fixpoint(false)
{
	generated_preds.clear(); // generated_preds := [[]]
	labelled_preds.clear(); // labelled_preds := [[]]
	if(init)
	{
		Block::EdgeIter outs(entryb->outs());
		ASSERT(outs);
		path.addLast(*outs);
		constants.set(sp, SP, Set<Edge*>::null, false); // set that ?13==SP (since SP is the value of ?13 at the beginning of the program)
	}
}

Analysis::State::State(Edge* entry_edge, const context_t& context, bool init)
	: dfa_state(context.dfa_state), sp(context.sp), bottom(false), constants(context.max_tempvars, context.max_registers)//, fixpoint(false)
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
	: dfa_state(s.dfa_state), sp(s.sp), bottom(s.bottom), path(s.path), constants(s.constants), labelled_preds(s.labelled_preds), generated_preds(s.generated_preds), generated_preds_taken(s.generated_preds_taken)//, fixpoint(s.fixpoint)
	{ }

void Analysis::State::appendEdge(Edge* e, bool is_conditional)
{
	// add edge to the end of the path
	this->path.addLast(e);
	// we now need to label the correct list of predicates
	const SLList<LabelledPredicate> &relevant_preds = (is_conditional && e->isTaken()/*e->kind() == Edge::TAKEN*/) ?
		generated_preds_taken : // conditional TAKEN
		generated_preds;  // NOT TAKEN, VIRTUAL, VIRTUAL RETURN, non-conditional TAKEN

	// label our list of predicates with the current edge then append it
	labelled_preds += labelPredicateList(relevant_preds, e);
	constants.label(e); // label the constants as well
}

/*void Analysis::State::printFixPointState() const
{ }*/

/**
 * @fn void Analysis::processCFG(CFG* cfg);
 * Runs the analysis on the CFG cfg
*/
void Analysis::processCFG(CFG* cfg)
{
/* begin */
	/* ips ← {} */
	infeasible_paths.clear();
	/* for e ∈ E(G) */
		/* s_e ← nil */
	/* for h ∈ H(G) */
		/* s_h ← nil */
		/* status_h ← ENTER */
	/* for e ∈ entry.outs */
	for(Block::EdgeIter entry_outs(cfg->entry()->outs()); entry_outs; entry_outs++)
	{
		/* s_e ← T */
		SLList<Analysis::State> entry_s;
		entry_s += topState(cfg->entry());
		EDGE_S(*entry_outs) = entry_s;
		/* wl ← sink(e) */
		wl.push((*entry_outs)->target()); // only one outs, firstBB.
	}
	DBG(wl);
	DBG(*wl[0]->ins())
	DBG(EDGE_S.use(*wl[0]->ins()).first().dumpEverything())

	/* while wl ≠ {} do */
	while(!wl.isEmpty())
	{
		/* b ← pop(wl) */
		Block *b = wl.pop();
		/* pred ← 	b.ins \ B(G) if b ∈ H(G) ∧ status_b = ENTER */
		/* 			b.ins ∩ B(G) if b ∈ H(G) ∧ status_b ∈ {FIX, LEAVE} */
		/* 			b.ins 		 if b ∈/ H(G) */
		//const Vector<Edge*> pred1(b->ins()); // WTF
		const Vector<Edge*> pred(LOOP_HEADER(b) ? (loopStatusIsEnter(b)
				? nonBackIns(b) /* if b ∈ H(G) ∧ status_b = ENTER */
				: backIns(b) /* if b ∈ H(G) ∧ status_b ∈ {FIX, LEAVE} */
			) : allIns(b) /* if b ∉ H(G) */
		);

		/* if ∀e ∈ pred, s_e ≠ nil then */
		if(allEdgesHaveTrace(pred))
		{
			/* s ← |_|e∈pred s_e */
			Vector<State> s(narrowing(pred)); // TODO: def as Vector(1) if 
			/* for e ∈ pred */
			for(Vector<Edge*>::Iterator e(pred); e; e++)
				/* s_e ← nil */
				EDGE_S.remove(e);
			
			/* succ ← b.outs */
			bool propagate = true;
			/* if b ∈ H(G) then */
			if(LOOP_HEADER(b))
			{
				/* if status b = LEAVE then */
				if(loopStatusIsLeave(b))
				{
					/* if ∃e ∈ b.ins | s_e = nil then */
					if(anyEdgeHasTrace(b->ins()))
						/* wl ← wl ∪ {b} */
						wl.push(b);
					/* s_b ← nil */
					LH_S.remove(b);
					/* succ ← {} */
					propagate = false;
				}
				else /* else s_b ← s */
					LH_S(b) = s[0];
				/* status_b ← FIX if status_b = ENTER */
				if(LH_STATUS.get(b) == elm::none)
					LH_STATUS(b) = FIX; 
				/*			  LEAVE if status_b = FIX ∧ s ≡ s_b */
				else if(LH_STATUS.get(b) == elm::some(FIX) && s[0].equiv(LH_S.use(b)))
					LH_STATUS(b) = LEAVE; 
				/*			  ENTER if status_b = LEAVE */
				else if(LH_STATUS.get(b) == elm::some(LEAVE))
					LH_STATUS(b).remove();
			}
			I(b, s); // update s
			/* for e ∈ succ \ {EX_h | b ∈ L_h ∧ status_h =/ LEAVE} */
			if(propagate)
				for(Vector<Edge*>::Iterator e(outsWithoutUnallowedExits(b)); e; e++)
				{
					/* s_e ← I*[e](s) */
					// EDGE_S(e) = I(e, s);  // temporary: convert Vector to SLList
					EDGE_S.exists(e) ? EDGE_S.ref(e).clear() : EDGE_S.set(e, SLList<State>::null);
					EDGE_S.ref(e).addAll(I(e, s));
					/* ips ← ips ∪ ipcheck(s_e , {(h, status_h ) | b ∈ L_h }) */
					if(inD_ip(e))
						ipcheck(EDGE_S(e), infeasible_paths);
					/* wl ← wl ∪ {sink(e)} */
					wl.push((*e)->target());
				}
		}
	}
/* end */
}
#if 0
{
	DBG("Using SMT solver: " << (flags&DRY_RUN ? "(none)" : SMT::printChosenSolverInfo()))
	DBG(color::Whi() << "Processing CFG " << cfg)
	total_paths = loop_header_count = 0;
	int processed_bbs = 0;
	placeboProcessCFG(/*cfg*/);
	std::time_t timestamp = clock(); // Timestamp before analysis

	/* begin */
	/* wl <- {ɛ} */
	wl.push(cfg->entry()->outs().item()->target()); // firstBB
	/* lock[] <- {{}}; */
	{
		/*const*/ Edge* entry_edge = cfg->entry()->outs().item();
		SLList<Analysis::State> entry_annotation;
		entry_annotation += Analysis::State(cfg->entry(), dfa_state, sp, max_tempvars, max_registers);
		PROCESSED_EDGES(entry_edge) = entry_annotation;
	}

	/* While wl != ∅ */
	while(!wl.isEmpty())
	{
		DBG("wl=" << wlToString())
#		ifdef DBGG
			cout << color::Blu() << "\twl=" << wlToString() << color::RCol() << endl;
#		endif
		/* bb::wl <- wl; */
		Block *b = wl.pop();
		ASSERTP(b->isBasic() || b->isSynth() || b->isExit(), "Block is neither bb or sb or exit: " << b)
		SLList<Analysis::State> sl; //TODO: Vector
		const bool loop_header = LOOP_HEADER(b);

		/*  If lock[bb].count() < bb.ins.count
				continue;
			End If
		*/
		if(!allRequiredInEdgesAreProcessed(b)) // if new loop, only require for non back edges, otherwise, require all of them
			continue;

		if(dbg_verbose > DBG_VERBOSE_ALL && dbg_verbose < DBG_VERBOSE_RESULTS_ONLY) // -s verbose
			printCurrentlyProcessingBlock(b, ++processed_bbs*100/bb_count, loop_header);

		/* sl <- mergeIntoOneList(lock[bb]); */
		for(Block::EdgeIter b_ins(b->ins()); b_ins; b_ins++)
			sl.addAll(*PROCESSED_EDGES(*b_ins));
		// at this point of the algorithm, all incoming edges of bb have been processed
		purgeStateList(sl); // remove all invalid states

		// in case of a loop, merge the state list into a single state, and do lot of annotation stuff
		if(loop_header && !sl.isEmpty())
		{
			processLoopHeader(b, sl); //, PROCESSED_LOOPHEADER_BB, MOTHERLOOP_FIXPOINT_STATE, FIXPOINT_REACHED
			cleanIncomingBackEdges(b);
		}
		else
			cleanIncomingEdges(b);

		// SynthBlock case
		if(b->isSynth())
		{	// CALL
			ASSERTP(b->isCall(), "SB is not CALL")
			DBG("Entering function '" << b->toSynth()->callee() << "'")
			Edge* callee_entry_edge = b->toSynth()->callee()->entry()->outs().item();
			PROCESSED_EDGES(callee_entry_edge) = sl;
			if(!wl.contains(callee_entry_edge->target()))
				wl.push(callee_entry_edge->target());
			continue;
		}
		else if(b->isExit())
		{	// EXIT
			CFG::CallerIter citer(b->cfg()->callers());
			if(citer)
			{
				Block::EdgeIter caller_exit_edges_iter(citer->outs());
				ASSERTP(caller_exit_edges_iter, "must be at least 1 outedge from caller")
				PROCESSED_EDGES(*caller_exit_edges_iter) = sl;
				if(!wl.contains(caller_exit_edges_iter->target()))
					wl.push(caller_exit_edges_iter->target());
				ASSERTP(!(++caller_exit_edges_iter), "must be at max 1 outedge from caller")
				ASSERTP(!(++citer), "must be max. 1 caller");
				continue;
			}
			else
			{	// exiting main CFG: no caller
				cout << color::IGre() << "Reached end of program." << color::RCol() << endl;
				if(wl)
					cout << color::BIRed() << "WARNING: " << color::IRed() << "Reached end of program with non-empty working list. The following blocks could not be processed: " << wl << color::RCol() << endl;
				break; // continue would have the same effect if wl is empty (should normally be the case)
			}
		}
		else // BasicBlock
		{
			ASSERT(b->isBasic());
			int state_count = 0;
			/* For s in sl */
			for(SLList<Analysis::State>::MutableIterator sl_iter(sl); sl_iter; )
			{
				state_count++;
				DBG(color::Whi() << "Processing path " << sl_iter.item().getPathString())
				/* processBB(s, bb); */
				if(processBB(sl_iter.item(), b->toBasic()) != 0)
					sl.remove(sl_iter); // path ended
				else sl_iter++;
			}
			/* End For */

			// merging state lists that got too large
			if(!mergeOversizedStateList(sl))
				if(state_count >= 50 && dbg_verbose > DBG_VERBOSE_ALL && dbg_verbose < DBG_VERBOSE_RESULTS_ONLY)
					cout << " " << state_count << " states updated." << endl;

			bool enable_smt = true;
			// TODO! make sure that when we have 3 loops if lvl1=no fixpoint, lvl2=fixpoint, and lvl3 finds a fixpoint, we don't enable smt...
			/* For e in bb.outs */
			for(Block::EdgeIter b_outs(b->outs()); b_outs; b_outs++)
			{
				// TODO! implement this in v2
				// on call return, program stacking: remove obsolete predicates featuring stack addresses lower than current sp
				/*if(bb_outs->kind() == Edge::VIRTUAL_RETURN)
				{
					if(const Option<Constant> current_sp = getCurrentStackPointer(sl))
					{
						DBG(color::IYel() << "Return reached, invalidating stack below " << *current_sp) // invalid states have been purged at this point
						for(SLList<Analysis::State>::MutableIterator sl_iter(sl); sl_iter; sl_iter++)
							sl_iter.item().invalidateStackBelow(*current_sp);
					}
					else DBG(color::IYel() << "Return reached, but sp value is lost or variable")
				}*/
				if(BACK_EDGE(*b_outs))
					DBG(color::Whi() << "End of loop reached.")
				bool inner_fixpoint = ; // inner loop fixpoint status
				if(!fixpoint) // fixpoint not found
				{
					if(!LOOP_EXIT_EDGE.exists(*b_outs)) // take everything but loop exit edges
					{	// adds to PROCESSED_EDGE
						enable_smt = edgeIsExitingToLoopLevel0(b_outs); // only call smt if this is "level 0" sequential code
						processOutEdge(*b_outs, sl, isConditional(b), enable_smt); // annotate regardless of returned new_sl being empty or not
						if(!wl.contains(b_outs->target()))
							wl.push(b_outs->target());
						#ifdef DBGG
							cout << "\te=" << b_outs->source()->id() << "->" << b_outs->target()->id() << ", enable_smt=" << DBG_TEST(enable_smt, true) << endl;
						#endif
					}
				}
				else // fixpoint found
				{
					if(!BACK_EDGE(*b_outs)) // take everything but back edges
					{	// adds to PROCESSED_EDGE
						enable_smt = true;
						if(LOOP_EXIT_EDGE.exists(*b_outs)) // exiting the loop // Warning: we may exit several loops!
						{
							Block* outerloop_header = LOOP_EXIT_EDGE.get(*b_outs);
							for(SLList<Analysis::State>::MutableIterator sl_iter(sl); sl_iter; sl_iter++)
							{	// make sure to exit all loops until the outer loop of LOOP_EXIT_EDGE
								Block *header = loop_header ? b : ENCLOSING_LOOP_HEADER(b);
								while(header != outerloop_header) {
									ASSERT(header);
									sl_iter.item().onLoopExit(header);
									header = ENCLOSING_LOOP_HEADER(header);
								}
								sl_iter.item().onLoopExit(header);
							}
							if(MOTHERLOOP_FIXPOINT_STATE.exists(*b_outs))
							{
								bool new_fixpoint_state = MOTHERLOOP_FIXPOINT_STATE(*b_outs);
								for(SLList<Analysis::State>::MutableIterator sl_iter(sl); sl_iter; sl_iter++)
									sl_iter.item().setFixpointState(new_fixpoint_state); // put the fixpoint flag on all states
								// enable_smt = false;	// doesn't sound right to call with enable_smt=true even when we're coming back to the motherloop
								enable_smt = new_fixpoint_state || edgeIsExitingToLoopLevel0(b_outs); // enable smt if the parent loop has reached fixpoint or if theres no parent loop
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
								/*
								if(sl_iter.item().fixpointState() && bb_outs->kind() == Edge::VIRTUAL_CALL)
									sl_iter.item().onCall(*bb_outs); // don't write calls that are also LOOP_EXIT_EDGE / TODO! double check this is ok
								if(sl_iter.item().fixpointState() && bb_outs->kind() == Edge::VIRTUAL_RETURN && RETURN_OF.exists(bb_outs->source()))
									sl_iter.item().onReturn(RETURN_OF(bb_outs->source())); // don't write returns that are also LOOP_EXIT_EDGE
								*/
							}
						}
						enable_smt = enable_smt && fixpointFoundOnAllMotherLoops(b);
						#ifdef DBGG
							cout << "\tenable_smt=" << DBG_TEST(enable_smt, true) << ", shouldEnableSolver=" << DBG_TEST(shouldEnableSolver(*b_outs), true) << endl;
						#endif
						processOutEdge(*b_outs, sl, isConditional(b), enable_smt); // annotate regardless of returned new_sl being empty or not 
						if(!wl.contains(b_outs->target()))
							wl.push(b_outs->target());
					}
				}
			}
			/* End For */
			debugProgress(b->id(), enable_smt);
		}
	}
	/* End While */
	/* end */

	// analysis complete, print infeasible paths
	removeDuplicateInfeasiblePaths();
	printResults((clock()-timestamp)*1000/CLOCKS_PER_SEC);
}
#endif

Vector<Analysis::State>& Analysis::I(Block* b, Vector<State>& s)
{
	DBG(color::Bold() << "I(b=" << b << ")" << color::NoBold())// TODO!!!
	return s;
}

Vector<Analysis::State>& Analysis::I(Edge* e, Vector<State>& s)
{
	// TODO!!!
	ASSERT(false);
	return s;
}

/*
	@fn int Analysis::processBB(State& s, BasicBlock* bb);
	@brief Process a basic block, for a given state
	@param s Analysis state to update
	@param bb BasicBlock to process
	@return 0: continue, 1: stop analysis for this path
*/
int Analysis::processBB(BasicBlock* bb, State& s)
{
	if(bb->isExit())
	{
		onPathEnd();
		return 1;
	}
	
	DBG(color::Whi() << "Processing " << (Block*)bb)
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
	/*PROCESSED_EDGES.remove(e); // cleaning
	if(sl.isEmpty())
	{	// just propagate the {bottom}
		State invalid_state;
		PROCESSED_EDGES.ref(e).addFirst(invalid_state);
		return;
	}
	// bool is_call = e->kind() == Edge::VIRTUAL_CALL && !BACK_EDGE(*e);
	if(!enable_smt || !shouldEnableSolver(e))
	{
		for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++)
		{
			State s = *sl_iter; // copy into new state
			// if(is_call)
				// s.onCall(e);
			s.appendEdge(e, is_conditional);
			PROCESSED_EDGES.ref(e).addFirst(s);
		}
		return;
	}

	// SMT is enabled
	SLList<Option<Path> > sl_paths;
	// get a list of infeasible paths matching the list of state "sl_paths"
	stateListToInfeasiblePathList(sl_paths, sl, e, is_conditional);
	SLList<Option<Path> >::Iterator sl_paths_iter(sl_paths);
	for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++, sl_paths_iter++)
	{
		const State& s = *sl_iter;
		if(*sl_paths_iter) // is infeasible?
		{
			const Path& infeasible_path = **sl_paths_iter;
			elm::String counterexample;
			DBG("Path " << s.getPathString() << "->" << e->target()->index() << " minimized to " << pathToString(infeasible_path))
			bool valid = checkInfeasiblePathValidity(sl, sl_paths, e, infeasible_path, counterexample);
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
	}*/
}

void Analysis::processLoopHeader(Block* b, SLList<Analysis::State>& sl)
{
	/*n<// ensure BasicBlock
	ASSERTP(b->isBasic(), "\tprocessLoopHeader called with non-0Basic Block " << b);
	BasicBlock *bb = b->toBasic();

	State* s = new State((Edge*)NULL, dfa_state, sp, max_tempvars, max_registers, false); // entry is cleared anyway
	s->merge(sl); // merge paths properly (preserve calls)
	s->setFixpointState(Analysis::listOfFixpoints(sl));
#	ifdef DBGG
		cout << color::IPur() << "\tprocessLoopHeader after merge, sl = [" << sl.count() << "], s=" << *s << ", fixpt=" << DBG_TEST(s->fixpointState(), true) << color::RCol() << io::endl;
#	endif
	DBG("merged: constants=" << s->getConstants() << ", labelled_preds=" << s->getLabelledPreds());
	
	bool delete_s = false;
	if(PROCESSED_LOOPHEADER_BB.exists(bb))
	{
		if(s->fixpointState())
		{
			// Stage 3 (final): the fixpoint has been found and we just re-iterated with it, looking for infeasible paths
			// don't compute anything, just extract the fixpoint we already know
			delete s;
			s = PROCESSED_LOOPHEADER_BB(bb);
#ifdef NYU_BAD
			ASSERT(s->fixpointState()) // instead of //s->setFixpointState(true); // ensure we keep this property
#endif
			PROCESSED_LOOPHEADER_BB.remove(bb); // clean the loop header from the soon to be obsolete annotation // TODO! test this
		}
		else
		{
			const State* prev_s = PROCESSED_LOOPHEADER_BB(bb);
			ASSERT(prev_s != NULL);
			if(s->isFixPoint(*prev_s))
			{
				// Stage 2a: we just iterated on this loop, and found the fixpoint. Iterating one last time to check for infeasible paths
				// TO*DO! don't iterate if we don't have the fixpoint of outerloops yet, since we won't activate the SMT anyway
				if(dbg_verbose < DBG_VERBOSE_RESULTS_ONLY)
					cout << color::Bold() << "FIXPOINT on BB#" << bb->index() << color::RCol() << endl;
				s->setFixpointState(true);
				delete_s = true;
				// we need to clear the annotation for future fixpoints on this loop 
				// // huh, not really the reason why. If this was a fixpoint in a previous analysis it'll be one in future ones
				// // // what? even if it will be a fixpoint, it may be the wrong one. Imagine fixpointing on x=0 and x=1, it depends on the initialization...
				// PROCESSED_LOOPHEADER_BB.remove(bb); //  this makes absurd results... // I think that's because the code in stage 3 is bad when PROCESSED_LOOPHEADER_BB is removed after we iterate with fixpoint=true states
#ifndef TOREMOVE
				PROCESSED_LOOPHEADER_BB.remove(bb);
#ifdef DBGG
				cout << color::IYel << "PROCESSED_LOOPHEADER_BB(" << bb->cfg() << ":" << bb->index() << ") could have been removed, so yes!" << color::RCol << endl;
#endif
#endif
			}
			else
			{
				ASSERT(!s->fixpointState())
				// Stage 2b: we just iterated on this loop, and haven't found a fixpoint yet. Trying again.
				PROCESSED_LOOPHEADER_BB.ref(bb) = s;
				delete prev_s;
			}
		}
	}
	else // Stage 1 (initial): we have never iterated on this loop. Initialize the annotation on the loop header with s
	{
		s->setFixpointState(false); // TODO! test this, 01/03/16
		PROCESSED_LOOPHEADER_BB.ref(bb) = s;
	}

	ASSERTP(EXIT_LIST.exists(bb), "Loop header's EXIT_LIST does not exist. Maybe an infinite loop?");
	for(Vector<Edge*>::Iterator exits_iter(*EXIT_LIST.use(bb)); exits_iter; exits_iter++)
		MOTHERLOOP_FIXPOINT_STATE.ref(*exits_iter) = sl.first().fixpointState();  // annotate all the loop exit edges (bit hacky :-/)
	sl.clear();
	if(s->fixpointState())
		s->onLoopEntry(bb);
	sl += *s; // sl <- {s}
	FIXPOINT_REACHED.set(bb, s->fixpointState());
#	ifdef DBGG
		cout << color::IPur() << "\tend of processLoopHeader, s=" << *s << color::RCol() << io::endl;
#	endif
	if(delete_s)
		delete s;*/
}

void Analysis::stateListToInfeasiblePathList(SLList<Option<Path> >& sl_paths, const SLList<Analysis::State>& sl, Edge* e, bool is_conditional)
{
	/*
	for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++)
	{
		State s = *sl_iter;
		s.appendEdge(e, is_conditional);
		// if(is_call)
		// 	s.onCall(e);
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
			counterexample = _ << (*sl_subiter).getPathString() << "+" << e->source()->index() << "->" << e->target()->index();
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
	DBG("addDisorderedInfeasiblePath(...), ip=" << pathToString(ip) << ", " 
		<< color::ICya() << "full_path=[" << full_path << "]" << color::RCol() << ", result=" << detailed_ip); // TODO!
#endif
	addDetailedInfeasiblePath(detailed_ip);
}

void Analysis::addDetailedInfeasiblePath(const DetailedPath& ip)
{
	DetailedPath new_ip(ip);
	assert(ip.hasAnEdge()); // maybe a strong assumption, we'll see
	// first off, add loop entries for all missing loops
	Block *b = ip.firstEdge()->source();
	if(LOOP_HEADER(b))
		new_ip.addEnclosingLoop(b);
	while(ENCLOSING_LOOP_HEADER.exists(b))
	{
		b = ENCLOSING_LOOP_HEADER(b);
		new_ip.addEnclosingLoop(b);
	}
	// optimize by removing redundancies
	new_ip.optimize();
	infeasible_paths.add(new_ip);
}

/**
 * @brief Remove all bottom states
 * 
 * @param sl list to purge
 */
void Analysis::purgeStateList(SLList<Analysis::State>& sl) const
{
	for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; )
	{
		if(sl_iter->isBottom())
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
/*bool Analysis::mergeOversizedStateList(SLList<Analysis::State>& sl) const
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
}*/

/**
 * @brief remove annotations from incoming edges to given BasicBlock
 * 
 * @param b Block to process
 * @param processed_edges_id Identifier for processed edges
 */
/*void Analysis::cleanIncomingEdges(Block *b) const
{
	for(Block::EdgeIter b_ins(b->ins()); b_ins; b_ins++)
		PROCESSED_EDGES.remove(*b_ins);
		//PROCESSED_EDGES.ref(*b_ins).clear();
}*/

/**
 * @brief remove annotations from incoming back edges
 * 
 * @param b Block to process
 * @param processed_edges_id Identifier for processed (back) edges
 */
/*void Analysis::cleanIncomingBackEdges(Block *b) const
{
	for(Block::EdgeIter b_ins(b->ins()); b_ins; b_ins++)
		if(BACK_EDGE(*b_ins))
			PROCESSED_EDGES.remove(*b_ins);
			// PROCESSED_EDGES.ref(*b_ins).clear();
}*/

/**
 * @brief Checks if a fix point has been found on all parent loops
 */
/*bool Analysis::fixpointFoundOnAllMotherLoops(Block *b) const
{
	while(ENCLOSING_LOOP_HEADER.exists(b))
	{
		b = ENCLOSING_LOOP_HEADER(b);
		if(!FIXPOINT_REACHED.exists(b) || FIXPOINT_REACHED(b) != true)
			return false;
	}
	return true;
}*/

/**
 * @brief Test if the edge target has no mother loop.
 * 
 * @param e Edge to test
 * @return True if no mother loop was found at the target of the Edge e.
 */
/*bool Analysis::edgeIsExitingToLoopLevel0(const Edge* e) const
{
	return !LOOP_HEADER(e->target()) && !ENCLOSING_LOOP_HEADER.exists(e->target());
}*/

/**
 * @brief Checks if solver should be enabled
 * 
 * @param e Edge being processed
 * @return Returns true if solver should be enabled
 */
/*bool Analysis::shouldEnableSolver(const Edge* e)
{
	// return true; // TODO!!
	Block::EdgeIter src_outs(e->source()->outs());
	if(src_outs && ++src_outs) // outs > 1
		return true; // conditional edge, call...
	if(LOOP_EXIT_EDGE.exists(*e))
		return true;
	return false;
	// TODO: improve, let's be smarter
}*/

/**
 * @brief Dry run on the CFG to find properties on it (!no longer does anything!)
 */
void Analysis::placeboProcessCFG(/*CFG* cfg*/) const
{
	if(dbg_verbose > DBG_VERBOSE_ALL && dbg_verbose < DBG_VERBOSE_NONE)
		cout << color::ICya() << bb_count << color::RCol() << " BBs found." << endl;
	// else
	// 	DBG(color::Whi() << total_paths << " paths found.")
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

void Analysis::printCurrentlyProcessingBlock(Block* b, int progression_percentage, bool loop_header) const
{
	if(b->isBasic())
		cout << "[" << progression_percentage << "%] Processing BB  #" << IntFormat(b->id()).width(3) << " (" << b->cfg() << ":" << b->index() << ")";
	else if(b->isSynth()) {
		ASSERTP(b->toSynth()->callee(), "CALL TO NULL")
		cout << color::ICya() << "[" << progression_percentage << "%] Processing CALL#" << IntFormat(b->id()).width(3) << " (->" << b->toSynth()->callee() << ")";
	}
	else if(b->isExit())
		cout << color::IBlu() << "[" << progression_percentage << "%] Processing EXIT#" << IntFormat(b->id()).width(3) << " (" << b->cfg() << "->)";
	else ASSERT(false);
	cout << " of " << bb_count << (loop_header?" (loop header)":"") << color::RCol() << endl;
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

/*bool Analysis::isAHandledEdgeKind(Edge::kind_t kind) const
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
}*/

// either we find SP in all the paths we merge (and the same SP), either we return elm::none
Option<Constant> Analysis::getCurrentStackPointer(const SLList<Analysis::State>& sl) const
{
	Option<Constant> rtn = elm::none; // also acts like a bool first = true
	for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++)
	{
		if(sl_iter->getConstants().isConstant(context.sp))
		{
			if(rtn)
			{
				if(*rtn != sl_iter->getConstants()[context.sp])
					return elm::none; // paths have different SP value
			}
			else
				rtn = elm::some(sl_iter->getConstants()[context.sp]);
		}
		else
			return elm::none; // one of the paths has invalidated SP
	}
	return rtn;
}

/**
 * @brief Test if a BasicBlock is conditional
 * A BasicBlock is conditional if and only if it has more than one outgoing edge, including a taken edge
 * @param BasicBlock to test
 */
bool Analysis::isConditional(Block* b) const
{
	int count = 0;
	bool atLeastOneTaken = false;
	for(Block::EdgeIter outs(b->outs()); outs; outs++)
		//if(isAHandledEdgeKind(outs->kind()))
		{
			count++;
			ASSERTP(!atLeastOneTaken || !outs->isTaken(), "Multiple outgoing taken edges currently not supported by IP analysis (" << b <<")");
			ASSERTP(count <= 2, "3+ outgoing edges currently not supported by IP analysis (" << b << ")")
			atLeastOneTaken |= outs->isTaken() /*(outs->kind() == Edge::TAKEN)*/;
		}
	return atLeastOneTaken && (count > 1);
}
/*
bool Analysis::allRequiredInEdgesAreProcessed(Block* block) const
{
	if(LOOP_HEADER(block))
	{
		if(PROCESSED_LOOPHEADER_BB.exists(block)) // TODO!!! we have to clean these now!... see vincent/edn
		{
#ifndef TOREMOVE
			if(allIncomingNonBackEdgesAreAnnotated(block, PROCESSED_EDGES) && !allIncomingEdgesAreAnnotated(block, PROCESSED_EDGES)) {
				cout << color::IYel() << "block " << block << ":" << block->cfg() << " has existing PROCESSED_LOOPHEADER_BB (=" << FIXPOINT_REACHED(block) << ") annotation, and some back edges are not annotated" << endl;
			for(Block::EdgeIter b_ins(block->ins()); b_ins; b_ins++)
				if(BACK_EDGE(*b_ins))
					if(!PROCESSED_EDGES.exists(*b_ins))
						cout << "For example, BACK_EDGE '" << *b_ins << "' is not annotated." << endl;
#ifndef	NYU_BAD
			// cout << "we say it's ok to continue even though it's (probably) not" << elm::color::RCol() << endl;
			// return allIncomingNonBackEdgesAreAnnotated(block, PROCESSED_EDGES); // clean loop: only require for non-backedges to be annotated
#endif
			}
#endif
			return allIncomingEdgesAreAnnotated(block, PROCESSED_EDGES); // loop we are iterating on: require every in edge to be annotated
		}
		else
			return allIncomingNonBackEdgesAreAnnotated(block, PROCESSED_EDGES); // clean loop: only require for non-backedges to be annotated
	}
	else
		return allIncomingEdgesAreAnnotated(block, PROCESSED_EDGES); // not loop h.: require every in edge to be annotated
}*/

/**
	@fn int Analysis::allIncomingNonBackEdgesAreAnnotated(BasicBlock* bb, const Identifier<SLList<Analysis::State> >& annotation_identifier) const;
	Returns true when all incoming non-back edges (look edges) are annotated with the given annotation
	@param bb BasicBlock to check
	@param annotation_identifier the annotation id we test on the incoming edges
*/
/*bool Analysis::allIncomingNonBackEdgesAreAnnotated(Block* block, const Identifier<SLList<Analysis::State> >& annotation_identifier) const
{
	for(Block::EdgeIter b_ins(block->ins()); b_ins; b_ins++)
		if(!BACK_EDGE(*b_ins))
			if(!annotation_identifier.exists(*b_ins))
				return false;
	return true;
}

bool Analysis::allIncomingEdgesAreAnnotated(Block* block, const Identifier<SLList<Analysis::State> >& annotation_identifier) const
{
	for(Block::EdgeIter b_ins(block->ins()); b_ins; b_ins++)
		if(!annotation_identifier.exists(*b_ins))
			return false;
	return true;
}*/

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
	for(Vector<Block*>::Iterator iter(wl); iter; iter++)
	{
		if(first) first = false; else
			rtn = rtn.concat((CString)", ");
		// rtn = _ << rtn << (*iter)->id();
		rtn = _ << rtn << (*iter)->cfg() << ":" << (*iter)->index();
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
		str = str.concat(_ << (*iter)->source()->cfg() << ":" << (*iter)->source()->index() << "->" << (*iter)->target()->cfg() << ":" << (*iter)->target()->index());
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
		// if(!first && (*iter)->source()->index() != lastid)
		// {
		// 	DBG("str=" << str)
		// 	DBG("lastid=" << lastid << ", (*iter)->source->index()=" << (*iter))
		// }
		// when path is x->y and y'->z, there must be y=y'
		ASSERTP(first || (*iter)->source()->index() == lastid, "OrderedPath previous target and current source do not match! ex: 1->2, 2->4, 3->5");
		if(first)
		{
#			ifndef NO_UTF8
				if((*iter)->source()->index() == 0)
					str = _ << "ε";
				else
#			endif
				str = _ << (*iter)->source()->index();
			first = false;
		}
		str = _ << str << "->" << (*iter)->target()->index();
		lastid = (*iter)->target()->index();
	}
	if(str.isEmpty())
		return "(empty)";
	return str;
}

/**
 * @fn bool allEdgesHaveTrace(const Vector<Edge*>& edges) const;
 * @brief Checks for all edges to have a trace annotation
 * @param edges Vector of Block edges to inspect
 * @rtn true iff all edges have a trace annotation
 */
bool Analysis::allEdgesHaveTrace(const Vector<Edge*>& edges) const
{
	for(Vector<Edge*>::Iterator iter(edges); iter; iter++)
		if(!EDGE_S.exists(*iter))
			return false;
	return true;
}

/**
 * @fn bool allEdgesHaveTrace(const Block::EdgeIter& biter) const;
 * @brief Checks for all edges to have a trace annotation
 * @param biter Iterator over the list of Block edges to inspect
 * @rtn true iff all edges have a trace annotation
 */
bool Analysis::allEdgesHaveTrace(const Block::EdgeIter& biter) const
{
	for(Block::EdgeIter i(biter); i; i++)
		if(!EDGE_S.exists(*i))
			return false;
	return true;
}

/**
 * @fn bool anyEdgeHasTrace(const Vector<Edge*>& edges) const;
 * @brief Checks for at least one edge to have a trace annotation
 * @param edges Vector of Block edges to inspect
 * @rtn true iff at least one edge has a trace annotation
 */
bool Analysis::anyEdgeHasTrace(const Vector<Edge*>& edges) const
{
	for(Vector<Edge*>::Iterator iter(edges); iter; iter++)
		if(EDGE_S.exists(*iter))
			return true;
	return false;
}

/**
 * @fn bool anyEdgeHasTrace(const Block::EdgeIter& biter) const;
 * @brief Checks for at least one edge to have a trace annotation
 * @param biter Iterator over the list of Block edges to inspect
 * @rtn true iff at least one edge has a trace annotation
 */
bool Analysis::anyEdgeHasTrace(const Block::EdgeIter& biter) const
{
	for(Block::EdgeIter i(biter); i; i++)
		if(EDGE_S.exists(*i))
			return true;
	return false;
}
