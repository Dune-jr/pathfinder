#ifndef _ORACLE_H
#define _ORACLE_H

#include <elm/genstruct/Vector.h>
#include <otawa/cfg/Edge.h>
#include "analysis.h"
#include "detailed_path.h"

class DefaultAnalysis : public Analysis
{
public:
	DefaultAnalysis(WorkSpace *ws, PropList &props, int flags, int merge_thresold, int nb_cores)
		: Analysis(ws, props, flags, merge_thresold, nb_cores) { }

protected:
	LockPtr<States> narrowing(const Vector<Edge*>& edges) const;
	bool inD_ip(const otawa::Edge* e) const;
	IPStats ipcheck(States& ss, Vector<DetailedPath>& infeasible_paths) const;

	LockPtr<States> vectorOfS(const Vector<Edge*>& ins) const;
};

#endif