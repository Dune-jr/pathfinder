/**
 * Standard implementation of the Analysis
 */

// #include <elm/sys/Thread.h> // multithreading
#include "analysis_state.h"
#include "cfg_features.h"
#include "oracle.h"
#include "debug.h"
#ifdef SMT_SOLVER_CVC4
	#include "cvc4/cvc4_smt.h"
 	typedef CVC4SMT chosen_smt_t;
#elif SMT_SOLVER_Z3
	#include "z3/z3_smt.h"
 	typedef Z3SMT chosen_smt_t;
#endif

DefaultAnalysis::DefaultAnalysis(const context_t& context, int state_size_limit, int flags)
	: Analysis(context, state_size_limit, flags) { }

Vector<Analysis::State> DefaultAnalysis::narrowing(const Vector<Edge*>& ins) const
{
	ASSERTP(ins, "narrowing given empty ingoing edges vector")
	Vector<State> v(vectorOfS(ins));
	if(LOOP_HEADER(ins[0]->target()) || ((flags&MERGE) && v.count() > state_size_limit))
	{
		ASSERTP(v, "Loop Header received only bottom state, case not handled yet. The main algorithm will use s[0] so...")
		State s((Edge*)NULL, context, false); // entry is cleared anyway
		s.merge(v);
		if(dbg_verbose < DBG_VERBOSE_RESULTS_ONLY && v.count() > 50)
			cout << " " << v.count() << " states merged into 1 (from " << ins.count() << " ins)." << endl;
		Vector<State> rtnv(1);
		rtnv.push(s);
		return rtnv;
	}
	else
		return v;
}

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
	return all_leave; //TODOv2: add condition: && isConditional(e->source())
}

//TODOv2: make this method const
// look for infeasible paths, add them to infeasible_paths, and removes the states from sv
void DefaultAnalysis::ipcheck(elm::genstruct::Vector<Analysis::State>& sv, elm::genstruct::Vector<DetailedPath>& infeasible_paths) //const
// void Analysis::stateListToInfeasiblePathList(SLList<Option<Path> >& sl_paths, const SLList<Analysis::State>& sl, Edge* e, bool is_conditional)
{
	if(flags&DRY_RUN) // no SMT call
		return;
	// find the conflicts
	Vector<Option<Path> > vl_paths;
	Vector<Analysis::State> new_sv(sv.count()); // safer :)
	for(Vector<Analysis::State>::Iterator si(sv); si; si++)
	{
		// SMT call
		chosen_smt_t smt;
		const Option<Path>& infeasible_path = smt.seekInfeasiblePaths(*si);
		vl_paths.addLast(infeasible_path);
		if(infeasible_path)
		{	//TODOv2: test this: we remove state instead of bottomizing it
#			ifdef DBG_WARNINGS
				if(! infeasible_path->contains(s.lastEdge())) // make sure the last edge was relevant in this path
					cerr << "WARNING: !infeasible_path->contains(s.lastEdge())" << endl;
#			endif
		}
		else
			new_sv.addLast(*si);
	}
	// analyse the conflicts found
	ASSERTP(sv.count() == vl_paths.count(), "different size of sv and vl_paths")
	Vector<Option<Path> >::Iterator pi(vl_paths);
	for(Vector<Analysis::State>::Iterator si(sv); si; si++, pi++) // iterate on paths and states simultaneously
	{
		const State& s = *si;
		if(*pi) // is infeasible?
		{
			const Path& ip = **pi;
			elm::String counterexample;
			DBG("Path " << s.getPathString() << " minimized to " << pathToString(ip))
			bool valid = checkInfeasiblePathValidity(sv, vl_paths, ip, counterexample);
			DBG(color::BIWhi() << "B)" << color::RCol() << " Verifying minimized path validity... " << (valid?color::IGre():color::IRed()) << (valid?"SUCCESS!":"FAILED!"))
			ip_count++;
			if(valid)
			{
				addDisorderedInfeasiblePath(ip, s.getDetailedPath(), infeasible_paths); // infeasible_paths += order(ip); to output proprer ffx!
				DBG(color::On_IRed() << "Inf. path found: " << pathToString(ip))
			}
			else // we found a counterexample, e.g. a feasible path that is included in the set of paths we marked as infeasible
			{
				DBG("   counterexample: " << counterexample)
				unminimized_ip_count++;
				if(flags&UNMINIMIZED_PATHS)
				{	// falling back on full path (not as useful as a result, but still something)
					DetailedPath full_path = s.getDetailedPath();
					addDetailedInfeasiblePath(full_path, infeasible_paths);
					if(dbg_verbose == DBG_VERBOSE_ALL)
					{
						Path fp;
						for(DetailedPath::EdgeIterator fpi(full_path); fpi; fpi++)
							fp += *fpi;
						DBG(color::On_IRed() << "Inf. path found: " << pathToString(fp) << color::RCol() << " (unrefined)")
					}
					// TODO: do a C) where we still try to refine this infeasible path?
				}
				else
					DBG(color::IRed() << "Ignored infeasible path that could not be minimized")
			}
			onAnyInfeasiblePath();
		}
	}
	sv = new_sv;
}

SLList<Analysis::State> DefaultAnalysis::listOfS(const Vector<Edge*>& ins) const
{
	SLList<State> sl;
	for(Vector<Edge*>::Iterator i(ins); i; i++)
		sl.addAll(EDGE_S.use(*i));
	return sl;
}

Vector<Analysis::State> DefaultAnalysis::vectorOfS(const Vector<Edge*>& ins) const
{
	Vector<State> vl;
	for(Vector<Edge*>::Iterator i(ins); i; i++)
		vl.addAll(EDGE_S.use(*i));
		// for(Vector<State>::Iterator j(EDGE_S.use(*i)); j; j++)
		// 	vl.push(j);
	return vl;
}
