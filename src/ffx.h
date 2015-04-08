#ifndef _FFX_H_
#define _FFX_H_

#include <elm/io/OutStream.h>
#include <elm/io/OutFileStream.h>
#include "analysis.h"
using namespace elm;

class FFX
{
public:
	FFX(const Vector<Analysis::OrderedPath>& ips);
	void output(const elm::String& filename) const;

private:
	void printInfeasiblePath(io::Output& FFXFile, const Analysis::OrderedPath& ip) const;
	void printInfeasiblePathOldNomenclature(io::Output& FFXFile, const Analysis::OrderedPath& ip) const;

	const Vector<Analysis::OrderedPath>& infeasible_paths;
};

#endif
