#ifndef _FFX_H_
#define _FFX_H_

#include <elm/io/OutStream.h>
#include <elm/io/OutFileStream.h>
#include "analysis.h"
using namespace elm;

class FFX
{
public:
	FFX(const Set<Analysis::Path>& ips);
	void output(const elm::String& filename) const;

private:
	void printInfeasiblePath(io::Output FFXFile, const Analysis::Path& ip) const;

	const Set<Analysis::Path>& infeasible_paths;
};

#endif
