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
	enum {
		BLOCK_DOM 	 = 1 << 0,
		EDGE_DOM 	 = 1 << 1,
		EDGE_POSTDOM = 1 << 2,
	};

	GlobalDominance(const otawa::CFGCollection *cfgs, int flags) {
		for(otawa::CFGCollection::Iterator cfg(cfgs); cfg; cfg++) {
			DBG("Computing dominance for CFG: " << cfg->name())

			if(flags&BLOCK_DOM) {
				bdoms.put(*cfg, new BlockDominance(*cfg));
				DBG("\tbdoms: " << **bdoms[*cfg])
			}

			if(flags&EDGE_DOM) {
				edoms.put(*cfg, new EdgeDom(*cfg, 
					singleton<otawa::sgraph::Edge*>(theOnly(cfg->entry()->outs()))
				));
				DBG("\tedoms: " << **edoms[*cfg])
			}

			if(flags&EDGE_POSTDOM) {
				genstruct::SLList<otawa::sgraph::Edge*> sl; 
				for(Block::EdgeIter i(cfg->exit()->ins()); i; i++)
					sl += *i;
				epdoms.put(*cfg, new EdgePostDom(*cfg, sl));
				DBG("\tepdoms: " << **epdoms[*cfg])
			}
		}
	}

	~GlobalDominance() {
		for(genstruct::HashTable<CFG*, BlockDominance*>::PairIterator i(bdoms); i; i++)
			delete (*i).snd;
		for(genstruct::HashTable<CFG*, EdgeDom*>::PairIterator i(edoms); i; i++)
			delete (*i).snd;
		for(genstruct::HashTable<CFG*, EdgePostDom*>::PairIterator i(epdoms); i; i++)
			delete (*i).snd;
	}

	bool dom(Block* b1, Block* b2) const {
		while(b2->cfg() != b1->cfg()) {
			b2 = getCaller(b2, NULL);
			if(!b2) // reached main, still didn't match b1's cfg, so b2 isn't even indirectly called by b1
				return false;
		}
		while(b2 != b1) {
			Block* prev_b2 = b2;
			b2 = static_cast<Block*>((*bdoms.get(b2->cfg()))->idom(b2));
			if(b2 == prev_b2) // reached entry
				return false;
		}
		return true;
	}
	bool dom(otawa::Edge* e1, otawa::Edge* e2) const {
		// a nice thing with dominance is that we don't need to check that the entry dominates the edge, it's always the case
		while(e2->source()->cfg() != e1->source()->cfg()) {
			Block* b = getCaller(e2->source()->cfg(), NULL);
			if(!b) // reached main, still didn't match e1's cfg, so e2 isn't even indirectly called by e1
				return false;
			e2 = theOnly(b->ins());
		}
		while(e2 != e1) {
			otawa::Edge* prev_e2 = e2;
			e2 = static_cast<otawa::Edge*>((*edoms.get(e2->source()->cfg()))->idom(e2));
			if(e2 == prev_e2) // reached entry
				return false;
		}
		return true;
	}
	bool postdom(otawa::Edge* e1, otawa::Edge* e2) const {
		// now we're going to have to check that the entry is post-dominated by the edge, everytime we go up in the caller tree
		while(e1->source()->cfg() != e2->source()->cfg()) {
			if(! postdom(e1, theOnly(e1->source()->cfg()->entry()->outs())) )
				return false; // e1 must post-dominate the entry edge of its cfg!
			Block* b = getCaller(e1->source()->cfg(), NULL);
			if(!b) // reached main, still didn't match e2's cfg, so e1 isn't even indirectly called by e2
				return false;
			e1 = theOnly(b->ins());
		}
		while(e2 != e1) {
			otawa::Edge* prev_e2 = e2;
			e2 = static_cast<otawa::Edge*>((*epdoms.get(e2->source()->cfg()))->idom(e2));
			if(!e2 || e2 == prev_e2) // reached (virtual) exit
				return false;
		}
		return true;
	}

private:
	genstruct::HashTable<CFG*, BlockDominance*> bdoms;
	genstruct::HashTable<CFG*, EdgeDom*> edoms;
	genstruct::HashTable<CFG*, EdgePostDom*> epdoms;
	// BlockDominanceGen<PCGBlock, PCGEdge> pcg_dom;
};

#endif