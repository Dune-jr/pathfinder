#ifndef _WORKING_LIST_H
#define _WORKING_LIST_H

#include "cfg_features.h"
/*
#include <elm/genstruct/Vector.h>
using elm::genstruct::Vector;

class WorkingList
{
public:
	WorkingList() { }
	WorkingList(const WorkingList& wl) : v(wl.v) { }
	inline Block* pop(void) { return v.pop(); }
	inline void push(Block* b) { if(!v.contains(b)) v.push(b); }
	inline bool isEmpty(void) const { return v.isEmpty(); }

	elm::String toString(void) const {
		elm::String rtn = "[";
		bool first = true;
		for(Vector<Block*>::Iterator iter(v); iter; iter++)
		{
			if(first) first = false; else
				rtn = rtn.concat(CString(", "));
			// rtn = _ << rtn << (*iter)->cfg() << ":" << (*iter)->index();
			rtn = _ << rtn << *iter;
		}
		rtn = rtn.concat(CString("]"));
		return rtn;
	}
private:
	Vector<Block*> v;
};
//*/

#include <elm/data/SortedList.h>
using elm::SortedList;

class BlockLoopComparator : public elm::Comparator<Block*>
{
public:
	static inline int arbitraryCompare(Block* const& b1, Block* const& b2)
		{ return b1 > b2 ? +1 : -1; }
	// says < when b1 is at a deeper loop level (so should be read first)
	static int compare(Block* const& b1, Block* const& b2) {
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
	inline int doCompare(Block* const& v1, Block* const& v2) const
		{ return compare(v1, v2); } // TODO!!! why do we have to explicit this??? 
};

// TODO: now we need a comparator
class WorkingList
{
	typedef SortedList<Block*,BlockLoopComparator> wl_t;
public:
	WorkingList() { }
	// WorkingList(const WorkingList& wl) : sl(wl.sl) { } // doesn't work...
	inline Block* pop(void) { Block* b = sl.first(); sl.removeFirst(); return b; }
	inline void push(Block* b) { if(!sl.contains(b)) sl.add(b); } // WARNING: sortedList seems to remove duplicates!!
	inline bool isEmpty(void) const { return sl.isEmpty(); }

	elm::String toString(void) const {
		elm::String rtn = "[";
		bool first = true;
		for(wl_t::Iterator iter(sl); iter; iter++)
		{
			if(first) first = false; else
				rtn = rtn.concat(CString(", "));
			// rtn = _ << rtn << (*iter)->cfg() << ":" << (*iter)->index();
			rtn = _ << rtn << *iter;
		}
		rtn = rtn.concat(CString("]"));
		return rtn;
	}
private:
	wl_t sl;
};

#endif
