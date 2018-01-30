/*
 *	Collects lambdas (named tops)
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2006-2018, IRIT UPS.
 * 
 *	OTAWA is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	OTAWA is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OTAWA; if not, write to the Free Software 
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
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