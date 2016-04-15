/**
 General functions to browse a virtualized CFG (inline, unique-Block calls)
*/

#ifndef _CFG_FEATURES_H
#define _CFG_FEATURES_H
#include <elm/PreIterator.h>
#include <otawa/cfg/CFG.h>
#include <otawa/cfg/features.h>

using otawa::Block;
using otawa::CFG;
using otawa::LOOP_HEADER;
using otawa::ENCLOSING_LOOP_HEADER;
using elm::PreIterator;

Option<Block*> getCaller(CFG* cfg);
Block* getCaller(CFG* cfg, Block* def);
inline Block* getCaller(Block* b, Block* def) { return getCaller(b->cfg(), def); }

// Loop Header Iterator for a virtualized CFG
class LoopHeaderIter: public PreIterator<LoopHeaderIter, Block*> {
public:
	inline LoopHeaderIter(Block* b): lh(b) { if(!LOOP_HEADER(b)) next(); }
	inline LoopHeaderIter(const LoopHeaderIter& i): lh(i.lh) { }
	inline LoopHeaderIter& operator=(const LoopHeaderIter& i) { lh = i.lh; return *this; }

	inline bool ended(void) const { return lh == NULL; }
	Block* item(void) const { return lh; }
	inline void next(void) {
		while(lh && !(tmp = ENCLOSING_LOOP_HEADER.get(lh, NULL))) {
			lh = getCaller(lh->cfg(), NULL);
		}
		lh = lh ? tmp : NULL;
	}

private:
	Block* lh; // can be NULL
	Block* tmp;
};

#endif