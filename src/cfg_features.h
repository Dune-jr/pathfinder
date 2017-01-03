/**
 General functions to browse a virtualized CFG (inline, unique-Block calls)
*/

#ifndef _CFG_FEATURES_H
#define _CFG_FEATURES_H
#include <elm/PreIterator.h>
#include <elm/genstruct/SLList.h>
#include <otawa/cfg/CFG.h>
#include <otawa/cfg/features.h>

using otawa::Block;
using otawa::CFG;
using otawa::LOOP_HEADER;
using otawa::ENCLOSING_LOOP_HEADER;
using elm::PreIterator;
using elm::genstruct::SLList;

extern bool cfg_follow_calls; // TODO!! dirty

template <class I, class T> inline T theOnly(PreIterator<I, T>& i) {
	ASSERT(i);
	T rtn = *i;
	i++;
	ASSERTP(!i, "not alone, next is " << *i);
	return rtn;
}
template <class I, class T> inline T theOnly(const PreIterator<I, T>& i) 
	{ I j(static_cast<const I&>(i)); return theOnly(j); }
	// { ASSERT(i); T rtn = *i; I j(static_cast<const I&>(i)); ASSERT(!++j); return rtn; }
template <class T> inline SLList<T> singleton(const T& x) { SLList<T> l; l.add(x); return l; }

Option<Block*> getCaller(CFG* cfg);
Block* getCaller(CFG* cfg, Block* def);
inline Block* getCaller(Block* b, Block* def) { return getCaller(b->cfg(), def); }

// Loop Header Iterator for a virtualized CFG
class LoopHeaderIter: public PreIterator<LoopHeaderIter, Block*> {
public:
	inline LoopHeaderIter(Block* b, bool inlined = true): lh(b)
		{ if(!LOOP_HEADER(b)) next(); }
	inline bool ended(void) const
		{ return lh == NULL; }
	Block* item(void) const
		{ return lh; } // works even when ended()
	inline void next(void) {
		while(lh && !(tmp = ENCLOSING_LOOP_HEADER.get(lh, NULL)))
			lh = cfg_follow_calls ? getCaller(lh->cfg(), NULL) : lh;
		lh = lh ? tmp : NULL;
	}

private:
	Block* lh; // can be NULL
	Block* tmp;
};

#endif
