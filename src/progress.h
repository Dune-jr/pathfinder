#ifndef _PROGRESS_H
#define _PROGRESS_H

#include <elm/genstruct/HashTable.h>
#include <elm/string/String.h>
#include <otawa/cfg/CFG.h>
#include "analysis.h"
#include "cfg_features.h"
// #include "debug.h"

using elm::genstruct::HashTable;
using otawa::CFG;

class Analysis::Progress
{
private:
	struct Stats
	{
		Stats() : curr(0), tot(0) { }
		Stats(int curr, int tot) : curr(curr), tot(tot) { }
		Stats operator+(const Stats& s) const { return Stats(curr+s.curr, tot+s.tot); }

		int curr; // current count of parsed BBs
		int tot; // total count of BBs in CFG
	};

public:
	Progress(CFG* main) { onEntry(main); }
	void onBlock(Block* b) {
		if(b->isEntry())
			onEntry(b->cfg());
		else if(b->isCall()){
			onExit(b->toSynth()->callee());
			// b = b->toSynth()->callee()->exit();
		}
		else if(b->isBasic())
			onBasic(b->toBasic());
		else if(b->isExit())
			onFinish();
		// else unknown/virtual
		print(b->cfg());
	}

private:
	inline void onEntry(CFG* cfg) { ASSERT(!tab.get(cfg)); tab.put(cfg, Stats(0, cfg->count())); }
	inline void onExit (CFG* cfg) { ASSERT(tab.get(cfg));  tab.remove(cfg); }
	inline void onBasic(BasicBlock* bb) { 
		bool all_leave = true;
		for(LoopHeaderIter lh(bb); lh; lh++)
			if(loopStatus(lh) != LEAVE)	{
				all_leave = false;
				break;
			}
		if(all_leave){
			// tab[bb->cfg()] = *tab.get(bb->cfg()) + Stats(1, 0);
			ASSERTP(tab.get(bb->cfg()), "reading block in CFG " << bb->cfg() << " after exiting (or before entring)")
			Stats v(*tab.get(bb->cfg()));
			tab.remove(bb->cfg());
			tab.put(bb->cfg(), v + Stats(1, 0));
		}
	}
	inline void onFinish() { }
	void print(CFG* cfg) const {
		for(Block* b = cfg->entry(); b; b = getCaller(b, NULL))
		{
			CFG* k = b->cfg();
			elm::String indent;
			ASSERTP(tab.get(k), "parsing callee after caller has exited!")
			elm::cout << indentMore(indent) << StringFormat(k->name()).width(15)
			 << ": " << IntFormat((*tab.get(k)).curr).width(3).right() << "/" << IntFormat((*tab.get(k)).tot)/*.width(3)*/
			 << " (" << IntFormat((*tab.get(k)).curr * 100/ (*tab.get(k)).tot).width(3) << "%)" << endl;
		}
		cout << endl;
	}
	inline elm::String& indentMore(elm::String &str) const { return (str = str + "\t"); }

	HashTable<CFG*, Stats> tab;
};

#endif