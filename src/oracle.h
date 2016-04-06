#ifndef _ORACLE_H
#define _ORACLE_H

#include <elm/genstruct/Vector.h>
#include <otawa/cfg/Edge.h>
#include "analysis.h"
#include "detailed_path.h"

class DefaultAnalysis : public Analysis
{
public:
	DefaultAnalysis(otawa::CFG *cfg, const dfa::State *dfa_state, int sp, unsigned int max_tempvars, unsigned int max_registers, int state_size_limit, int flags);
private:
	Vector<Analysis::State> narrowing(const Vector<Edge*>& edges) const;
	bool inD_ip(const otawa::Edge* e) const;
	void ipcheck(const elm::genstruct::SLList<Analysis::State>& sl, elm::genstruct::Vector<DetailedPath>& infeasible_paths) const;
};

#endif