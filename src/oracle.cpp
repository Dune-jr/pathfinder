/**
 * Standard implementation of the Analysis
 */

// #include <elm/sys/Thread.h> // multithreading
#include "analysis_state.h"
#include "cfg_features.h"
#include "debug.h"
#include "oracle.h"
#include "progress.h"
#include "smt_job.h"
#ifdef SMT_SOLVER_CVC4
	#include "cvc4/cvc4_smt.h"
 	typedef CVC4SMT chosen_smt_t;
#elif SMT_SOLVER_Z3
	#include "z3/z3_smt.h"
 	typedef Z3SMT chosen_smt_t;
#else
 	#error "no SMT solver"
#endif

/**
 * actually... this is widening
 */
LockPtr<Analysis::States> DefaultAnalysis::narrowing(const Vector<Edge*>& ins) const
{
	ASSERTP(ins, "narrowing given empty ingoing edges vector")
	LockPtr<States> v = vectorOfS(ins);
	Block* b = ins[0]->target();
	// loop header or too big state
	if(LOOP_HEADER(b) || ((flags&MERGE) && v->count() > state_size_limit))
	{
		if(LOOP_HEADER(b) && LH_S.exists(b)) // also merge with the state on the loop header if it exists
			v->push(LH_S(b));
		purgeBottomStates(*v);
		// ASSERTP(v, "Loop Header received only bottom state, case not handled yet. The main algorithm will use s[0] so...")
		if(v->isEmpty())
		{
			DBGG("narrowing returns null vector")
			return v;
		}
		State s((Edge*)NULL, context, false); // entry is cleared anyway
		s.merge(*v, b); // s <- widening(s0, s1, ..., sn)
		LockPtr<States> rtnv(new States(1));
		rtnv->push(s);
		if(dbg_verbose < DBG_VERBOSE_RESULTS_ONLY && v->count() > 50)
			cout << " " << v->count() << " states merged into 1 (from " << ins.count() << " ins)." << endl;
		return rtnv;
	}
	else
		return v; // no widening
}

/**
 * @brief Checks if a path ending with a certain edge is within the domain D of path we test the (in)feasibility of
 * @param e Edge the path ends with
 * @return true if the path is in D
 */
bool DefaultAnalysis::inD_ip(const otawa::Edge* e) const
{
	bool all_leave = true; // enable SMT when on sequential level
	for(LoopHeaderIter lh(e->source()); lh; lh++)
	{
		if(loopStatus(lh) != LEAVE)
		{
			all_leave = false;
			break;
		}
	}
	return all_leave && isConditional(e->source()); //TODOv2: add condition: && isConditional(e->source())
}

// look for infeasible paths, add them to infeasible_paths, and removes the states from ss
Analysis::IPStats DefaultAnalysis::ipcheck(States& ss, Vector<DetailedPath>& infeasible_paths) const
// void Analysis::stateListToInfeasiblePathList(SLList<Option<Path> >& sl_paths, const SLList<Analysis::State>& sl, Edge* e, bool is_conditional)
{
	IPStats stats;
	if(flags&DRY_RUN) // no SMT call
		return stats;
	const int state_count = ss.count();
	SolverProgress* sprogress;
	if(flags&SHOW_PROGRESS)
		sprogress = new SolverProgress(state_count);
	// find the conflicts
	Vector<Option<Path*> > sv_paths;
	Vector<Analysis::State> new_sv(state_count); // safer to do it this way than remove on the fly (i think more convenient later too)

	if(flags&Analysis::MULTITHREADING && state_count >= nb_cores)
	{	// with multithreading
		const int nb_threads = nb_cores;
		DBGG("1) Initializing " << nb_threads << " threads")
		Vector<elm::sys::Thread*> threads(nb_threads);
		Vector<SMTJob<chosen_smt_t>*> jobs(nb_threads);
		States::Iterator si(ss.states());
		for(int tid = 0, i = 0; tid < nb_threads; tid++)
		{
			SMTJob<chosen_smt_t>* job = new SMTJob<chosen_smt_t>(flags);
			const int thresold = state_count * (tid+1)/nb_threads; // add states until this thresold
			DBGG("\tthread #" << tid << ", doing jobs [" << i << "," << thresold << "[")
			for(; i < thresold; i++, si++)
				job->addState(&*(si));
			elm::sys::Thread* t = elm::sys::Thread::make(*job);
			jobs.push(job);
			threads.push(t);
		}
		DBGG("2) Starting threads")
		for(int i = 0; i < nb_threads; i++)
			threads[i]->start();
		DBGG("3) Joining threads")
		// join and get result
		for(int i = 0; i < nb_threads; i++)
		{
			threads[i]->join();
			DBGG("\t(joined #" << i+1 << ")")
			for(SMTJob<chosen_smt_t>::Iterator ji(jobs[i]->getResults()); ji; ji++)
			{
				const State* s = (*ji).fst;
				Option<Path*> infeasible_path = (*ji).snd;
				if(flags&SHOW_PROGRESS)
					sprogress->onSolving(infeasible_path);
				sv_paths.addLast(infeasible_path);
				if(!infeasible_path)
					new_sv.addLast(*s);
			}
			delete jobs[i];
			delete threads[i];
		}
		DBGG("4) done")
	}
	else
	{	// without multithreading
		for(States::Iterator si(ss.states()); si; si++)
		{	// SMT call
			chosen_smt_t smt(flags);
			const Option<Path*> infeasible_path = smt.seekInfeasiblePaths(*si);
			sv_paths.addLast(infeasible_path);
			if(!infeasible_path)
				new_sv.addLast(*si); // only add feasible states to new_sv
#			ifdef DBG_WARNINGS
				else if(! (*infeasible_path)->contains(si->lastEdge())) // make sure the last edge was relevant in this path
					cerr << "WARNING: !infeasible_path->contains(s.lastEdge())" << endl;
#			endif
			if(flags&SHOW_PROGRESS)
				sprogress->onSolving(infeasible_path);
		}
	}


	// cout << "["; for(Vector<Option<Path*> >::Iterator i(sv_paths); i; i++)
	// 	if(*i) cout << pathToString(***i) << ", "; cout << "\n";
	if(flags&SHOW_PROGRESS)
		delete sprogress;
	// analyse the conflicts found
	ASSERTP(ss.count() == sv_paths.count(), "different size of ss and sv_paths")
	Vector<Option<Path*> >::Iter pi(sv_paths);
	for(States::Iterator si(ss.states()); si; si++, pi++) // iterate on paths and states simultaneously
	{
		const State& s = *si;
		if(*pi) // is infeasible?
		{
			const Path& ip = ***pi;
			elm::String counterexample;
			DBG("Path " << s.getPathString() << color::Bold() << " minimized to " << color::NoBold() << pathToString(ip))
			bool valid = checkInfeasiblePathValidity(ss.states(), sv_paths, ip, counterexample);
			DBG(color::BIWhi() << "B)" << color::RCol() << " Verifying minimized path validity... " << (valid?color::IGre():color::IRed()) << (valid?"SUCCESS!":"FAILED!"))
			stats.onAnyInfeasiblePath();
			if(valid)
			{
				DetailedPath reordered_path = reorderInfeasiblePath(ip, s.getDetailedPath());
				reordered_path.optimize();
				addDetailedInfeasiblePath(reordered_path, infeasible_paths); // infeasible_paths += order(ip); to output proprer ffx!
				DBG(color::On_IRed() << "Inf. path found: " << reordered_path << color::RCol())
			}
			else // we found a counterexample, e.g. a feasible path that is included in the set of paths we marked as infeasible
			{
				DetailedPath full_path = s.getDetailedPath();
				full_path.optimize();
				DBG("   counterexample: " << counterexample)
				stats.onUnminimizedInfeasiblePath();
				if(flags&UNMINIMIZED_PATHS)
				{	// falling back on full path (not as useful as a result, but still something)
					addDetailedInfeasiblePath(full_path, infeasible_paths);
					if(dbg_verbose == DBG_VERBOSE_ALL)
					{
						Path fp;
						for(DetailedPath::EdgeIterator fpi(full_path); fpi; fpi++)
							fp += *fpi;
						DBG(color::On_IRed() << "Inf. path found: " << pathToString(fp) << color::RCol() << " (unrefined)")
					}
				}
				else
					DBG(color::IRed() << "Ignored infeasible path that could not be minimized")
			}
			onAnyInfeasiblePath();
			delete *pi;
		}
	}
	for(Vector<Analysis::State>::Iter svi(new_sv); svi; svi++)
		new_sv[svi].removeConstantPredicates(); // remaining constant predicates are tautologies, there is no need to keep them
	ss = new_sv; // TODO!! this is copying states, horribly unoptimized, we only need to remove a few states!
	return stats;
}

/*SLList<Analysis::State> DefaultAnalysis::listOfS(const Vector<Edge*>& ins) const
{
	SLList<State> sl;
	for(Vector<Edge*>::Iterator i(ins); i; i++)
		sl.addAll(EDGE_S.use(*i).states());
	return sl;
}*/

LockPtr<Analysis::States> DefaultAnalysis::vectorOfS(const Vector<Edge*>& ins) const
{
	if(ins.count() == 1) // opti TODO! should work now with the LockPtr
		return EDGE_S.use(ins[0]);
	LockPtr<States> s(new States());
	for(Vector<Edge*>::Iter i(ins); i; i++)
		s->states().addAll(EDGE_S.use(*i)->states());
	return s;
}
