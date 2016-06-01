#ifndef _ORACLE_H
#define _ORACLE_H

#include <elm/genstruct/Vector.h>
#include <otawa/cfg/Edge.h>
#include "analysis.h"
#include "detailed_path.h"

class DefaultAnalysis : public Analysis
{
public:
	DefaultAnalysis(WorkSpace *ws, PropList &props, /*const context_t& context,*/ int flags, int merge_thresold)
		: Analysis(ws, props, /*context,*/ flags, merge_thresold) { }

private:
	LockPtr<States> narrowing(const Vector<Edge*>& edges) const;
	bool inD_ip(const otawa::Edge* e) const;
	IPStats ipcheck(States& ss, elm::genstruct::Vector<DetailedPath>& infeasible_paths) const;

	// SLList<State> listOfS(const Vector<Edge*>& ins) const;
	LockPtr<States> vectorOfS(const Vector<Edge*>& ins) const;
};

#endif