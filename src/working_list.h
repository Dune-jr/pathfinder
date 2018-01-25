/*
 *	
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
 
#ifndef _WORKING_LIST_H
#define _WORKING_LIST_H

#include "cfg_features.h"

#include <elm/data/SortedList.h>
using elm::SortedList;

class BlockLoopComparator : public elm::Comparator<Block*>
{
public:
	static inline int arbitraryCompare(Block* const& b1, Block* const& b2)
		//{ return b1 > b2 ? +1 : -1; } // this causes determinism problems
		{
		  	return b1->cfg() == b2->cfg()
				? (b1->index() > b2->index() ? +1 : -1)
				: (b1->cfg()->index() > b2->cfg()->index() ? +1 : -1);
		}

	// says < when b1 is at a deeper loop level (so should be read first)
	static int compare(Block* b1, Block* b2) {
		if(b1 == b2)
			return 0; // ==
		LoopHeaderIter i1(b1), i2(b2);
		if( (!i1 && !i2) // both at seq level (no loop)
		 || (i1 && i2 && i1.item() == i2.item()) ) // same inmost loop
			return arbitraryCompare(b1,b2); // ==
		if(!i1) // then i2, so only i1 at seq level
			return +1; // >
		if(!i2) // then i1, so only i2 at seq level
			return -1; // < 

		// not equal: try to find inclusion
		Block *inlh1 = *i1, *inlh2 = *i2;
		for(; i1; i1++)
			if(*i1 == inlh2) // i1=L1-L2-L3-..., i2=L3-... i1 is deeper. here L3=inlh2
				return -1; // <
		for(; i2; i2++)
			if(*i2 == inlh1)
				return +1; // >

		// i1 is not included in i2, i2 is not included in i1. pick an arbitrary order based on block addresses
		return arbitraryCompare(b1,b2);
	}
	inline int doCompare(Block* v1, Block* v2) const
		{ return compare(v1, v2); } // TODO! why do we have to explicit this??? 
};

class WorkingList
{
	typedef SortedList<Block*, CompareManager<Block*, BlockLoopComparator> > wl_t;
public:
	WorkingList() { }
	inline Block* pop(void) { DBG("popping from " << toString()); Block* b = sl.first(); sl.removeFirst(); return b; }
	inline void push(Block* b) { if(!sl.contains(b)) sl.add(b); } // WARNING: sortedList seems to remove duplicates!!
	inline bool isEmpty(void) const { return sl.isEmpty(); }

	elm::String toString(void) const {
		elm::String rtn = "[";
		bool first = true;
		for(wl_t::Iter iter(sl); iter; iter++)
		{
			if(first) first = false; else
				rtn = rtn.concat(CString(", "));
			// rtn = _ << rtn << (*iter)->cfg() << ":" << (*iter)->index();
			rtn = _ << rtn << *iter;
		}
		rtn = rtn.concat(CString("]"));
		return rtn;
	}

	friend io::Output& operator<<(io::Output &out, const WorkingList& wl)
		{ return out << wl.toString(); }
private:
	wl_t sl;
};

#endif
