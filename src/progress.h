#ifndef _PROGRESS_H
#define _PROGRESS_H

#include <elm/genstruct/HashTable.h>
#include <elm/string/String.h>
#include <otawa/cfg/CFG.h>
#include "analysis.h"
#include "debug.h"
#include "cfg_features.h"
// #include "debug.h"

using elm::genstruct::HashTable;
using otawa::CFG;

class Analysis::SolverProgress {
public:
	SolverProgress(int state_count) : i(0), n(state_count) { }
	~SolverProgress() { onFinish(); }
	inline void onSolving(bool ip) { elm::cout << "\e[1A\n\e[K" << percentBar(i++*10/n,10,ip) << " " << n << " states"; }

private:
	inline void onFinish() { elm::cout << "\e[1A\n\e[K"; }
	inline elm::String percentBar(int x, int n, bool ip = false) const {
		elm::String rtn = ip ? "["+color::IRed() : "[";
		int i = 0;
		for(; i < x; i++)
			rtn = rtn + "■";
		for(; i < n; i++)
			rtn = rtn + " ";
		return rtn + (ip ? color::RCol() + "]" : "]");
	}

	int i;
	const int n;
};

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
	Progress(CFG* main) : lines(0), maxlines(-1) { onEntry(main); }
	void onBlock(Block* b) {
		if(b->isEntry())
			onEntry(b->cfg());
		else if(b->isCall()){
			onExit(b->toSynth()->callee());
			// b = b->toSynth()->callee()->exit();
		}
		else if(b->isBasic())
			onBasic(b->toBasic());
		else if(b->isExit()) {
			onFinish();
			return;
		}
		// else unknown/virtual
		print(b->cfg());
	}

private:
	inline void onEntry(CFG* cfg) { ASSERT(!tab.exists(cfg)); tab.put(cfg, Stats(0, cfg->count()-2)); }
	inline void onExit (CFG* cfg) { ASSERT(tab.exists(cfg));  tab.remove(cfg); }
	inline void onBasic(BasicBlock* bb) { 
		bool all_leave = true;
		for(LoopHeaderIter lh(bb); lh; lh++)
			if(loopStatus(lh) != LEAVE)	{
				all_leave = false;
				break;
			}
		if(all_leave){
			ASSERTP(tab.exists(bb->cfg()), "reading block in CFG " << bb->cfg() << " after exiting (or before entering)")
			tab.put(bb->cfg(), *tab.get(bb->cfg()) + Stats(1, 0));
		}
	}
	inline void onFinish() { moveUp(maxlines); maxlines = 0; }
	void print(CFG* cfg) {
		// count number n of lines required
		int n = 0;
		for(Block* b = cfg->entry(); b; b = getCaller(b, NULL))
			n++;
		if(n > maxlines)
			newLines(n-maxlines);
		eraseAboveLines(lines);
		moveUp(n);
		lines = 0;
		for(Block* b = cfg->entry(); b; b = getCaller(b, NULL))
		{
			CFG* k = b->cfg();
			ASSERTP(tab.get(k), "parsing callee after caller has exited!")
			elm::cout /*<< lines+1 << "/" << n << indent(n-lines)*/ << StringFormat(k->name()).width(15)
			 << ": " << IntFormat((*tab.get(k)).curr).width(3).right() << "/" << IntFormat((*tab.get(k)).tot).width(3)
			 << " (" << IntFormat((*tab.get(k)).curr * 100/ (*tab.get(k)).tot).width(3) << "%)" << endl;
			lines++;
		}
		ASSERT(lines==n)
		maxlines = max(maxlines, lines);	
	}
	inline elm::String indent(int n) const { String str; for(int i = 0; i < n; i++) str=str+"\t"; return str; }
	inline void moveUp(int n) const { elm::cout << "\e[" << n << "A"; }
	inline void newLines(int n) const { for(int i = 0; i < n; i++) elm::cout << endl; }
	inline void eraseAboveLines(int n) const { moveUp(n); for(int i = 0; i < n; i++) elm::cout << "\e[K\e[1B"; }

	HashTable<CFG*, Stats> tab;
	int lines, maxlines;
};

#endif