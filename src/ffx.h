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
	void output(const elm::String& filename, const elm::String& function_name, const elm::String& graph_filename = "");

private:
	void printInfeasiblePath(io::Output& FFXFile, const DetailedPath& ip);
	void writeGraph(io::Output& GFile, const Vector<DetailedPath>& ips);
	void checkPathValidity(const DetailedPath& ip) const;
	inline bool edgeAfter(SLList<DetailedPath::FlowInfo>::Iterator iter) const
		{ for(; iter; iter++) if(iter->isEdge()) return true; return false; }
	void printInfeasiblePathOldNomenclature(io::Output& FFXFile, const DetailedPath& ip);

	bool nextElementisCall(const DetailedPath::Iterator& iter, CFG* cfg);
	elm::String indent(int indent_increase = 0);

	const Vector<DetailedPath>& infeasible_paths;
	int indent_level;

	enum
	{
		FFX_TAG_LOOP=0,
		FFX_TAG_CALL=1,
	};
	typedef bool ffx_tag_t;
};

#endif
