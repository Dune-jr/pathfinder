#ifndef _ORACLE_H
#define _ORACLE_H

#include <elm/genstruct/Vector.h>
#include <otawa/cfg/Edge.h>
#include "analysis.h"
#include "detailed_path.h"

class DefaultAnalysis : public Analysis
{
public:
	DefaultAnalysis() : Analysis() { }

protected:
	LockPtr<States> join(const Vector<Edge*>& edges) const;
	LockPtr<States> merge(LockPtr<States>, Block* b) const;
	bool inD_ip(const otawa::Edge* e) const;
	IPStats ipcheck(States& ss, Vector<DetailedPath>& infeasible_paths) const;

	LockPtr<States> vectorOfS(const Vector<Edge*>& ins) const;
};

#endif