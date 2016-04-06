#include "oracle.h"
#include "analysis.h"

DefaultAnalysis::DefaultAnalysis(otawa::CFG *cfg, const dfa::State *dfa_state, int sp, unsigned int max_tempvars, unsigned int max_registers, int state_size_limit, int flags)
	: Analysis(cfg, dfa_state, sp, max_tempvars, max_registers, state_size_limit, flags) { }

Vector<Analysis::State> DefaultAnalysis::narrowing(const Vector<Edge*>& edges) const
{
	// TODO
}

bool DefaultAnalysis::inD_ip(const otawa::Edge* e) const
{
	// TODO
	return true;
}

void DefaultAnalysis::ipcheck(const elm::genstruct::SLList<Analysis::State>& sl, elm::genstruct::Vector<DetailedPath>& infeasible_paths) const
{
	// TODO
}
