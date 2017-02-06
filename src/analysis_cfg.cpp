/*
 * Macro analysis: skeleton of the analysis algorithm, defines the way we parse the CFG 
 */

// #include <elm/genstruct/SLList.h>
#include <otawa/cfg/Edge.h>
#include "analysis_state.h"
#include "progress.h"
#include "cfg_features.h"

// using namespace elm::genstruct;
using namespace elm::io;

Identifier<LockPtr<Analysis::States> >		Analysis::EDGE_S("Trace on an edge"); // old PROCESSED_EDGES
Identifier<Analysis::State>					Analysis::LH_S("Trace on a loop header"); // maybe change to vector
Identifier<Analysis::loopheader_status_t>	Analysis::LH_STATUS("Fixpt status of a loop (on a loop header)");
// for non-virtualized CFGs only
Identifier<const Operand*>					Analysis::LH_I("Iterator operand for the loop"); // on lheaders
Identifier<LockPtr<Analysis::States> >		Analysis::CFG_S("Trace on a CFG");
Identifier<LockPtr<VarMaker> >				Analysis::CFG_VARS("VarMaker of a CFG"); // VarMaker to be copied, updated, and appended on f° return

/**
 * @brief       Runs the analysis on a whole program
 * @param entry Entry block of the program to analyse
 */
void Analysis::processProg(CFG* cfg)
{
	/* ips ← {} */
	infeasible_paths.clear();
	/* ... */
	processCFG(cfg, flags&USE_INITIAL_DATA);
	DBGG(IGre() << "Reached end of program.")
}

/**
 * @fn void Analysis::processCFG(Block* entry);
 * @brief Runs the analysis starting from the CFG entry entry
*/
void Analysis::processCFG(CFG* cfg, bool use_initial_data)
{
	WorkingList wl;
	vm = new VarMaker();
	DBGG(IPur() << "==>\"" << cfg->name() << "\"")
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
			LockPtr<States> s = narrowing(pred);
			/* for e ∈ pred */
			for(Vector<Edge*>::Iter e(pred); e; e++)
				/* s_e ← nil */
				EDGE_S.remove(e);
			
			/* succ ← b.outs */
			bool propagate = true;
			/* if b ∈ H(G) then */
			if(LOOP_HEADER(b))
			{
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
						s->onLoopEntry(b);
						break;
					/* status_b ← LEAVE if status_b = FIX ∧ s ≡ s_b */
					case FIX: if(s->one().equiv(LH_S(b)))
						LH_STATUS(b) = LEAVE;
						break;
					/* status_b ← ENTER if status_b = LEAVE */
					case LEAVE:
						LH_STATUS(b).remove();
						break;
				}
			}
			I(b, s); // update s
			/* for e ∈ succ \ {EX_h | b ∈ L_h ∧ status_h =/ LEAVE} */
			const Vector<Edge*> succ(propagate ? outsWithoutUnallowedExits(b) : nullVector<Edge*>());
			for(Vector<Edge*>::Iter e(succ); e; e++)
			{
				/* s_e ← I*[e](s) */
				EDGE_S(e) = I(e, s);
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
}

/**
 * @brief      Interpretation function of a Block
 */
void Analysis::I(Block* b, LockPtr<States> s)
{
	if(flags&SHOW_PROGRESS)
		progress->onBlock(b);
	if(b->isBasic())
	{
		DBGG(Bold() << "-\tI(b=" << b << ") " << NoBold() << IYel() << "x" << s->count() << RCol() << printFixPointStatus(b))
		for(States::Iterator si(s->states()); si; si++)
			(*s)[si].processBB(b->toBasic(), flags&(Analysis::WITH_V1 | Analysis::WITH_V2 | Analysis::SP_CRITICAL));
	}
	else if(b->isEntry())
		s->onCall((*getCaller(b->cfg()))->toSynth());
	else if(b->isCall())
	{
		if(flags&VIRTUALIZE_CFG)
			s->onReturn(b->toSynth());
	}
	else if(b->isExit()) // main
		{ }
	else
		DBGG("WARNING: not doing anything");

	if(!(flags&VIRTUALIZE_CFG))
	{
		if(b->isCall())
		{
			CFG* called_cfg = b->toSynth()->callee();
			if(!CFG_S.exists(called_cfg)) // if the called CFG has not been processed yet
				processCFG(called_cfg, false);

			// merging tops
			VarMaker called_vm_copy(**CFG_VARS(called_cfg));
			vm->transfer(called_vm_copy); // vm receives more tops: we load the responsibility for all the pointers and clear the vector without deleting OpTops
			// working on the paths
			s->onCall(b->toSynth());
			s->apply(**CFG_S(called_cfg));
			s->onReturn(b->toSynth());
		}
		else if(b->isExit())
			CFG_S(b->cfg()) = s; // we will never free this, which shouldn't be a problem because it should only be freed at the end of analysis
	}
}

/**
 * @brief      Interpretation function of an Edge. Returns a processed copy of the provided states.
 */
LockPtr<Analysis::States> Analysis::I(const Vector<Edge*>::Iter& e, LockPtr<States> s)
{
	if(! e.ended()) // more edges to come
		s = LockPtr<States>(new States(*s));
	if(s->isEmpty())
		DBGG("-\tpropagating bottom state")
	// DBGG(Bold() << "-\tI(e= " << NoBold() << e << Bold() << " )" << NoBold() << (e->source()->isEntry() ? " (entry)" : ""))
	if(! e->source()->isEntry()) // do not process CFG entry: no generated preds and uninteresting edge to add (everything comes from the entry)
		for(States::Iterator si(s->states()); si; si++)
			(*s)[si].appendEdge(e);
	if(LOOP_EXIT_EDGE(e))
		s->onLoopExit(e);
	return s;
}

/**
 * @fn bool Analysis::allEdgesHaveTrace(const Vector<Edge*>& edges) const;
 * @brief Checks for all edges to have a trace annotation
 * @param edges Vector of Block edges to inspect
 * @return true iff all edges have a trace annotation
 */
bool Analysis::allEdgesHaveTrace(const Vector<Edge*>& edges) const
{
	for(Vector<Edge*>::Iter iter(edges); iter; iter++)
		if(!EDGE_S.exists(*iter))
			return false;
	DBGG("-..." << edges)
	return true;
}

/**
 * @fn bool Analysis::allEdgesHaveTrace(const Block::EdgeIter& biter) const;
 * @brief Checks for all edges to have a trace annotation
 * @param biter Iterator over the list of Block edges to inspect
 * @return true iff all edges have a trace annotation
 */
bool Analysis::allEdgesHaveTrace(const Block::EdgeIter& biter) const
{
	for(Block::EdgeIter i(biter); i; i++)
		if(!EDGE_S.exists(*i))
			return false;
	return true;
}

/**
 * @fn bool Analysis::anyEdgeHasTrace(const Vector<Edge*>& edges) const;
 * @brief Checks for at least one edge to have a trace annotation
 * @param edges Vector of Block edges to inspect
 * @return true iff at least one edge has a trace annotation
 */
bool Analysis::anyEdgeHasTrace(const Vector<Edge*>& edges) const
{
	for(Vector<Edge*>::Iter iter(edges); iter; iter++)
		if(EDGE_S.exists(*iter))
			return true;
	return false;
}

/**
 * @fn bool Analysis::anyEdgeHasTrace(const Block::EdgeIter& biter) const;
 * @brief Checks for at least one edge to have a trace annotation
 * @param biter Iterator over the list of Block edges to inspect
 * @return true iff at least one edge has a trace annotation
 */
bool Analysis::anyEdgeHasTrace(const Block::EdgeIter& biter) const
{
	for(Block::EdgeIter i(biter); i; i++)
		if(EDGE_S.exists(*i))
			return true;
	return false;
}

/**
 * @fn bool Analysis::checkInfeasiblePathValidity(const Vector<Analysis::State>& sl, const Vector<Option<Path> >& sl_paths, const Edge* e, const Path& infeasible_path, elm::String& counterexample);
 * Checks if the minimized list of edges we found 'infeasible_path' is valid,
 * that is if all the paths it represents (all the 'sl[i]->e') are infeasible ('sl_paths[i]' is not 'elm::option::none')
 * If invalid, returns a counter-example in counterexample.
 * @return true if valid
*/
bool Analysis::checkInfeasiblePathValidity(const Vector<Analysis::State>& sv, const Vector<Option<Path*> >& sv_paths, const Path& infeasible_path, elm::String& counterexample)
{
	// check that all the paths going to the current BB are sound with the minimized inf. path we just found
	Vector<Option<Path*> >::Iter pi(sv_paths);
	for(Vector<Analysis::State>::Iter si(sv); si; si++, pi++) // iterate through paths at the same time
	{
		// if feasible path && contained in the minimized inf. path
		if((*pi).isNone() && isSubPath(si->getDetailedPath().toOrderedPath(), infeasible_path))
		{
			counterexample = si->getPathString();
			return false;
		}
	}
	return true;
}

/**
 * @brief Reorder the infeasible path ip using the order path full_path
 * @param ip Infeasible path to add
 * @param full_path Full path it originates from (must include ip)
 */
DetailedPath Analysis::reorderInfeasiblePath(const Path& ip, const DetailedPath& full_path)
{
	DetailedPath ordered_ip(full_path.function());
	// parse the detailed path and add all the edges that match the Path ip and the loop entries
	for(DetailedPath::Iterator full_path_iter(full_path); full_path_iter; full_path_iter++)
	{
		if(full_path_iter->isEdge())
		{
			if(ip.contains(full_path_iter->getEdge()))
				ordered_ip.addLast(*full_path_iter);
		}
		else
			ordered_ip.addLast(*full_path_iter);
	}
	// DBG("reorderInfeasiblePath(...), ip=" << pathToString(ip) << ", " << ICya() << "full_path=[" << full_path << "]" 
		// << RCol() << ", result=" << ordered_ip);
	return ordered_ip;
}

/**
 * @brief      Register a new infeasible path
 *
 * @param[in]  ip                The infeasible path to add
 * @param      infeasible_paths  The collection of paths to add to
 */
void Analysis::addDetailedInfeasiblePath(const DetailedPath& ip, Vector<DetailedPath>& infeasible_paths)
{
	DetailedPath new_ip(ip);
	ASSERT(ip.hasAnEdge());
	// first off, add loop entries for all missing loops
	// Block *b = ip.firstEdge()->source();
	// for(LoopHeaderIter i(b); i; i++)
	// 	new_ip.addEnclosingLoop(*i);
	if(! infeasible_paths.contains(new_ip))
		infeasible_paths.add(new_ip);
	else
		DBG("not adding redundant IP: " << new_ip)
}

/**
 * @fn static void Analysis::onAnyInfeasiblePath();
 * @brief Debugs to do when detecting any infeasible path
 */
void Analysis::onAnyInfeasiblePath()
{
	DBG(BIYel() << "Stopping current path analysis")
}

/**
 * @brief either we find SP in all the paths we merge (and the same SP), either we return elm::none
 */
/*Option<Constant> Analysis::getCurrentStackPointer(const SLList<Analysis::State>& sl) const
{
	Option<Constant> rtn = elm::none; // also acts like a bool first = true
	for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++)
	{
		if(sl_iter->getConstants().isConstant(context.sp))
		{
			if(rtn)
			{
				if(*rtn != sl_iter->getConstants()[context.sp])
					return elm::none; // paths have different SP value
			}
			else
				rtn = elm::some(sl_iter->getConstants()[context.sp]);
		}
		else
			return elm::none; // one of the paths has invalidated SP
	}
	return rtn;
}*/

/**
 * @fn inline static bool Analysis::isConditional(Block* b);
 * @brief Test if a BasicBlock is conditional, i.e. has more than one edge
 * @param b Block to test
 */
