#include "oracle.h"
#include "analysis_state.h"

DefaultAnalysis::DefaultAnalysis(const context_t& context, int state_size_limit, int flags)
	: Analysis(context, state_size_limit, flags) { }

Vector<Analysis::State> DefaultAnalysis::narrowing(const Vector<Edge*>& ins) const
{
	ASSERTP(ins, "narrowing given empty ingoing edges vector")
	const Vector<State> v(vectorOfS(ins));
	if(LOOP_HEADER(ins[0]->target()) || ((flags&MERGE) && v.count() > state_size_limit))
	{
		State s((Edge*)NULL, context, false); // entry is cleared anyway
		s.merge(v); // TODOv2: change this into Vector
		if(dbg_verbose < DBG_VERBOSE_RESULTS_ONLY && v.count() > 50)
			cout << " " << v.count() << " states merged into 1 (from " << ins.count() << " ins)." << endl; // TODOv2: talk about merging after the LH interpretation
		Vector<State> rtnv(1);
		rtnv.push(s);
		return rtnv;
	}
	else
		return v;
}

bool DefaultAnalysis::inD_ip(const otawa::Edge* e) const
{
	// TODOv2
	return true;
}

void DefaultAnalysis::ipcheck(const elm::genstruct::Vector<Analysis::State>& sl, elm::genstruct::Vector<DetailedPath>& infeasible_paths) const
{
	// TODOv2
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
