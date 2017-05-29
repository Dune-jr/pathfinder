/**
 * v1 and v2
 */

#include <otawa/cfg/Edge.h>
#include "analysis1.h"
#include "../analysis_states.h"
#include "../progress.h"

using namespace elm::io;

// for otawa::Processor
p::feature otawa::OLD_INFEASIBLE_PATHS_FEATURE("otawa::pathfinder::OLD_INFEASIBLE_PATHS_FEATURE", new Maker<Analysis1>());
p::declare Analysis1::reg = p::init("otawa::pathfinder::pathfinder", Version(2, 0, 0))
	.maker<Analysis1>()
	.require(dfa::INITIAL_STATE_FEATURE)
	.require(VIRTUALIZED_CFG_FEATURE)
	.require(COLLECTED_CFG_FEATURE)
	.require(LOOP_HEADERS_FEATURE)
	.require(LOOP_INFO_FEATURE)
	.provide(OLD_INFEASIBLE_PATHS_FEATURE);

/**
 * @fn void Analysis::processCFG(Block* entry);
 * @brief Runs the analysis starting from the CFG entry entry
*/
void Analysis1::processCFG(CFG* cfg, bool use_initial_data)
{
#ifdef V1
	ASSERT(flags&VIRTUALIZE_CFG);
	
	WorkingList wl;
	vm = new VarMaker();
/* begin */
	/* for e ∈ E(G) */
		/* s_e ← nil */
	/* for h ∈ H(G) */
		/* s_h ← nil */
		/* status_h ← ENTER */
	/* for e ∈ entry.outs */
	for(Block::EdgeIter e(cfg->entry()->outs()); e; e++)
	{
		/* s_e ← T */
		LockPtr<States> s_entry(new States());
		s_entry->push(topState(cfg->entry()));
		if(use_initial_data)
			s_entry->states()[0].initializeWithDFA();
		EDGE_S(*e) = s_entry;
		/* wl ← sink(e) */
		wl.push((*e)->target()); // only one outs, firstBB.
	}

	/* while wl ≠ {} do */
	while(!wl.isEmpty())
	{
		/* b ← pop(wl) */
		Block *b = wl.pop();
		/* pred ← 	b.ins \ B(G) if b ∈ H(G) ∧ status_b = ENTER */
		/* 			b.ins ∩ B(G) if b ∈ H(G) ∧ status_b ∈ {FIX, LEAVE} */
		/* 			b.ins 		 if b ∈/ H(G) */
		const Vector<Edge*> pred(LOOP_HEADER(b) ? (loopStatus(b) == ENTER
				? nonBackIns(b) /* if b ∈ H(G) ∧ status_b = ENTER */
				: backIns(b) /* if b ∈ H(G) ∧ status_b ∈ {FIX, LEAVE} */
			) : allIns(b) /* if b ∉ H(G) */
		);

		/* if ∀e ∈ pred, s_e ≠ nil then */
		if(allEdgesHaveTrace(pred))
		{
			/* s ← |_|e∈pred s_e */
			LockPtr<States> s = join(pred);
			/* for e ∈ pred */
			for(Vector<Edge*>::Iter e(pred); e; e++)
				/* s_e ← nil */
				EDGE_S.remove(e);
			
			/* succ ← b.outs */
			bool propagate = true;
			/* if b ∈ H(G) then */
			if(LOOP_HEADER(b))
			{
				if(loopStatus(b) == FIX)
					s->push(LH_S(b)); /* s ← s ∪ s_b */
				s = merge(s, b);
				ASSERT(s->count() <= 1);
				/* if status_b = LEAVE then */
				if(loopStatus(b) == LEAVE)
				{
					/* if ∃e ∈ b.ins | s_e = nil then */
					if(anyEdgeHasTrace(b->ins()))
						/* wl ← wl ∪ {b} */
						wl.push(b);
					/* s_b ← nil */
					LH_S.remove(b);
					/* succ ← {} */
					propagate = false;
				}
				else /* else s_b ← s */
					LH_S(b) = s->one();
				switch(loopStatus(b)) {
					/* status_b ← FIX if status_b = ENTER */
					case ENTER:
						LH_STATUS(b) = FIX;
						addLoopStats(b); // TODO!
						// s->onLoopEntry(b);
						break;
					/* status_b ← LEAVE if status_b = FIX ∧ s ≡ s_b */
					case FIX: if(s->one().equiv(LH_S(b)))
						#warning This is buggy: we have already set LH_S to s!
						LH_STATUS(b) = LEAVE;
						break;
					/* status_b ← ENTER if status_b = LEAVE */
					case LEAVE:
						LH_STATUS(b).remove();
						break;
					default: crash();
				}
			}
			I(b, s); // update s
			/* for e ∈ succ \ {EX_h | b ∈ L_h ∧ status_h =/ LEAVE} */
			const Vector<Edge*> succ(propagate ? outsWithoutUnallowedExits(b) : nullVector<Edge*>());
			for(Vector<Edge*>::Iter e(succ); e; e++)
			{
				/* s_e ← I*[e](s) */
				EDGE_S(e) = Analysis::I(e, s);
				/* ips ← ips ∪ ipcheck(s_e , {(h, status_h ) | b ∈ L_h }) */
				if(inD_ip(e))
					ip_stats += ipcheck(*EDGE_S.ref(e), infeasible_paths);
				/* wl ← wl ∪ {sink(e)} */
				 wl.push(outsAlias(e->sink()));
			}
		}
	}
/* end */
	DBGG(IPur() << "\"" << cfg->name() << "\"==>")
	// vm->clean(*CFG_S(cfg));
	CFG_VARS(cfg) = LockPtr<VarMaker>(vm);
#else
	crash();
#endif
}

/**
 * @brief      Interpretation function of a Block
 */
void Analysis1::I(Block* b, LockPtr<States> s)
{
	if(flags&SHOW_PROGRESS)
		progress->onBlock(b);
	if(b->isBasic())
	{
		DBGG(Bold() << "-\tI(b=" << b << ") " << NoBold() << IYel() << "x" << s->count() << RCol() << printFixPointStatus(b))
		for(States::Iter si(s->states()); si; si++)
			(*s)[si].processBB(b->toBasic(), *vm, flags);
	}
	else if(b->isEntry())
		s->onCall((*getCaller(b->cfg()))->toSynth());
	else if(b->isCall())
	{
		s->onReturn(b->toSynth());
	}
	else if(b->isExit()) // main
		{ }
	else
		DBGW("not doing anything");
}
