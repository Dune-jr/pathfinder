/*
 * GlobalDominance.h
 */

#ifndef GLOBALDOMINANCE_H
#define GLOBALDOMINANCE_H

#include <otawa/cfg/features.h>
#include <otawa/prog/WorkSpace.h>
#include <otawa/pcg/PCG.h>
#include <otawa/pcg/PCGBuilder.h>
#include <otawa/cfg/CFGTransformer.h>
#include "BlockDominance.h"
#include "EdgeDominance.h"
#include "cfg_features.h"

using namespace elm;
using otawa::Block;

class MyTransformer : public otawa::CFGTransformer {

Block* clone(Block *b) {
		// end case
		if(b->isEnd()) {
			if(b->isEntry())
				return cur->entry();
			else if(b->isExit()) {
				return cur->exit();
				Block::EdgeIter exits(cur->exit()->ins());
				otawa::Edge* first_exit_edge = *exits;
				ASSERT(first_exit_edge);
				if(++exits)
				{
					// need to create an empty basic block to make sure we only have one exit edge
					genstruct::Table<otawa::Inst *> insts;
					otawa::BasicBlock* bb = build(insts);
					build(bb, cur->exit(), otawa::Edge::NOT_TAKEN); // add bb out edge
					build(first_exit_edge->source(), bb, otawa::Edge::NOT_TAKEN); // add bb in edge
					for(; exits; exits++)
						build((*exits)->source(), bb, otawa::Edge::NOT_TAKEN); // add bb in edge
				}
				return cur->exit();
			}
			else if(b->isUnknown())
				return cur->unknown();
			else {
				ASSERT(false);
				return 0;
			}
		}

		// synthetic block case
		else if(b->isSynth())
			return build(b->toSynth()->callee());

		// basic block
		else {
			otawa::BasicBlock *bb = b->toBasic();
			genstruct::Vector<otawa::Inst *> insts(bb->count());
			for(otawa::BasicBlock::InstIter i = bb; i; i++)
				insts.add(*i);
			return build(insts.detach());
		}
	}
};

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
				genstruct::SLList<otawa::sgraph::Edge*> sll;
				sll += theOnly(cfg->entry()->outs());
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
		while(e1->source()->cfg() != e2->source()->cfg()) {
			Block* b = getCaller(e1->source()->cfg(), NULL);
			if(!b) // reached main, still didn't match e2's cfg, so e1 isn't even indirectly called by e2
				return false;
			e1 = theOnly(b->ins());
		}
		while(e2 != e1) {
			otawa::Edge* prev_e2 = e2;
			e2 = static_cast<otawa::Edge*>((*epdoms.get(e2->source()->cfg()))->idom(e2));
			if(e2 == prev_e2) // reached entry
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