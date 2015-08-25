#ifndef _FFX_H_
#define _FFX_H_

#include <elm/io/OutStream.h>
#include <elm/io/OutFileStream.h>
#include "analysis.h"
using namespace elm;

class FFX
{
public:
	FFX(const Vector<DetailedPath>& ips);
	void output(const elm::String& filename);

private:
	void printInfeasiblePath(io::Output& FFXFile, const DetailedPath& ip);
	void printInfeasiblePathOldNomenclature(io::Output& FFXFile, const DetailedPath& ip);
	elm::String indent(int indent_increase = 0);

	const Vector<DetailedPath>& infeasible_paths;
	int indent_level;
};

#endif
