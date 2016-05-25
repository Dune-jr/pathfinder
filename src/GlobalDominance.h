/*
 * GlobalDominance.h
 */

#ifndef GLOBALDOMINANCE_H
#define GLOBALDOMINANCE_H

#include <otawa/cfg/features.h>
#include <otawa/prog/WorkSpace.h>
#include <otawa/pcg/PCG.h>
#include <otawa/pcg/PCGBuilder.h>
#include "BlockDominance.h"
#include "EdgeDominance.h"
#include "cfg_features.h"

using namespace elm;
using otawa::Block;

class GlobalDominance
{
public:
	GlobalDominance(const CFGCollection *cfgs) {
		for(CFGCollection::Iterator cfg(cfgs); cfg; cfg++) {
			doms.put(*cfg, new BlockDominance(*cfg));
			edoms.put(*cfg, new EdgeDominance(*cfg));
		}
	}

	~GlobalDominance() {
		for(genstruct::HashTable<CFG*, BlockDominance*>::PairIterator i(doms); i; i++)
			delete (*i).snd;
		for(genstruct::HashTable<CFG*, EdgeDominance*>::PairIterator i(edoms); i; i++)
			delete (*i).snd;
	}

	bool dom(Block* b1, Block* b2) {
		// PCG dominance
		while(b2->cfg() != b1->cfg()) {
			b2 = getCaller(b2, NULL);
			if(!b2) // reached main, still didn't match b1's cfg, so b2 isn't even indirectly called by b1
				return false;
		}
		while(b2 != b1) {
			Block* prev_b2 = b2;
			b2 = static_cast<Block*>((*doms.get(b2->cfg()))->idom(b2));
			if(b2 == prev_b2) // reached entry
				return false;
		}
		return true;
	}
	bool dom(Edge* e1, Edge* e2) {
		// PCG dominance
		while(e2->source()->cfg() != e1->source()->cfg()) {
			Block* b = getCaller(e2->source()->cfg(), NULL);
			if(!b) // reached main, still didn't match e1's cfg, so e2 isn't even indirectly called by e1
				return false;
			e2 = theOnly(b->ins());
		}
		while(e2 != e1) {
			Edge* prev_e2 = e2;
			e2 = static_cast<Edge*>((*edoms.get(e2->source()->cfg()))->idom(e2));
			if(e2 == prev_e2) // reached entry
				return false;
		}
		return true;
	}

private:
	genstruct::HashTable<CFG*, BlockDominance*> doms;
	genstruct::HashTable<CFG*, EdgeDominance*> edoms;
	// BlockDominanceGen<PCGBlock, PCGEdge> pcg_dom;
};

#endif