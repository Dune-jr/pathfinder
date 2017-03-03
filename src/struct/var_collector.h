#ifndef STRUCT_VAR_COLLECTOR_H
#define STRUCT_VAR_COLLECTOR_H

#include <elm/util/BitVector.h>
#include <elm/io/Output.h>

class VarCollector
{
public:
	VarCollector(elm::Pair<int, int> sizes) : bv(sizes.fst), k(sizes.snd) { ASSERT(k >= 0); }
	inline void collect(int top_id) { if(top_id-k >= 0 && top_id-k < bv.size()) bv.set(top_id - k);
			else if(top_id-k >= bv.size()) DBGW("Not adding T" << top_id) }
	inline int size() const { return bv.size(); }
	inline bool operator[](int i) const { return bv[i]; }
	friend inline io::Output& operator<<(io::Output& out, const VarCollector& vc) { return vc.print(out); }

private:
	BitVector bv;
	int k;

	io::Output& print(io::Output& out) const { return out << "VC{" << k << " virtual, '" << bv << "'}"; }
};

#endif