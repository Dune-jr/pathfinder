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

Identifier<LockPtr<Analysis::States> >		Analysis::EDGE_S("Trace on an edge"); // old PROCESSED_EDGES  //TODO! try vector
Identifier<Analysis::State>					Analysis::LH_S("Trace on a loop header"); // maybe change to vector
Identifier<Analysis::loopheader_status_t>	Analysis::LH_STATUS("Fixpt status of a loop (on a loop header)");

/**
 * @fn void Analysis::processCFG(CFG* cfg);
 * Runs the analysis on the CFG cfg
*/
void Analysis::processCFG(CFG* cfg)
{
/* begin */
	/* ips ← {} */
	infeasible_paths.clear();
	/* for e ∈ E(G) */
		/* s_e ← nil */
	/* for h ∈ H(G) */
		/* s_h ← nil */
		/* status_h ← ENTER */
	/* for e ∈ entry.outs */
	for(Block::EdgeIter entry_outs(cfg->entry()->outs()); entry_outs; entry_outs++)
	{
		/* s_e ← T */
		LockPtr<States> entry_s(new States());
		entry_s->push(topState(cfg->entry()));
		EDGE_S(*entry_outs) = entry_s;
		/* wl ← sink(e) */
		wl.push((*entry_outs)->target()); // only one outs, firstBB.
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
			I(b, *s); // update s
			/* for e ∈ succ \ {EX_h | b ∈ L_h ∧ status_h =/ LEAVE} */
			const Vector<Edge*> succ(propagate ? outsWithoutUnallowedExits(b) : nullVector<Edge*>());
			for(Vector<Edge*>::Iter e(succ); e; e++)
			{
				/* s_e ← I*[e](s) */
				EDGE_S(e) = I(e, *s);
				/* ips ← ips ∪ ipcheck(s_e , {(h, status_h ) | b ∈ L_h }) */
				if(inD_ip(e))
					ip_stats += ipcheck(*EDGE_S.ref(e), infeasible_paths);
				/* wl ← wl ∪ {sink(e)} */
				wl_push(e->target());
			}
		}
	}
/* end */
}

Analysis::States& Analysis::I(Block* b, States& s)
{
	if(flags&SHOW_PROGRESS)
		progress->onBlock(b);
	if(b->isBasic())
	{
		DBGG(color::Bold() << "-\tI(b=" << b << ") " << color::NoBold() << printFixPointStatus(b))
		for(States::Iterator si(s.states()); si; si++)
			s[si].processBB(b->toBasic(), flags&(Analysis::WITH_V1 | Analysis::WITH_V2));
	}
	else if(b->isEntry())
		s.onCall((*getCaller(b->cfg()))->toSynth());
	else if(b->isCall())
		s.onReturn(b->toSynth());
	else if(b->isExit()) // main
		{ }
	else
		DBGG("WARNING: not doing anything");
	return s;
}

LockPtr<Analysis::States> Analysis::I(Edge* e, const States& s)
{
	if(s.isEmpty())
		DBGG("-\tpropagating bottom state")
	// DBGG(color::Bold() << "-\tI(e= " << color::NoBold() << e << color::Bold() << " )" << color::NoBold() << (e->source()->isEntry() ? " (entry)" : ""))
	LockPtr<States> rtns(new States(s));
	if(! e->source()->isEntry()) // do not process entry: no generated preds and uninteresting edge to add (everything comes from the entry)
		for(States::Iterator rtnsi(rtns->states()); rtnsi; rtnsi++)
			rtns->states()[rtnsi].appendEdge(e);
	if(LOOP_EXIT_EDGE(e))
		rtns->onLoopExit(e);
	return rtns;
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
	DetailedPath ordered_ip;
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
	// DBG("reorderInfeasiblePath(...), ip=" << pathToString(ip) << ", " << color::ICya() << "full_path=[" << full_path << "]" 
		// << color::RCol() << ", result=" << ordered_ip);
	return ordered_ip;
}

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
 * @fn void Analysis::removeDuplicateInfeasiblePaths();
 * @brief Look for infeasible paths that share the same ordered list of edges and remove duplicates 
 */
/*void Analysis::removeDuplicateInfeasiblePaths()
{
	Vector<DetailedPath> new_ips;
	for(Vector<DetailedPath>::Iterator dp_iter(infeasible_paths); dp_iter; )
	{
		DetailedPath dp(*dp_iter);
		infeasible_paths.remove(dp_iter);
		bool contains = false;
		for(Vector<DetailedPath>::Iterator dp_subiter(infeasible_paths); dp_subiter; dp_subiter++)
		{
			if(dp_subiter->weakEqualsTo(dp)) // if dp == *dp_subiter*
			{
				contains = true; // dp is equal to an element of infeasible_paths \ dp
				break;
			}
		}
		if(!contains)
			new_ips.add(dp);
	}
	infeasible_paths.clear();
	infeasible_paths = new_ips;
}*/

/**
 * @fn static void Analysis::onAnyInfeasiblePath();
 * @brief Debugs to do when detecting any infeasible path
 */
void Analysis::onAnyInfeasiblePath()
{
	DBG(color::BIYel() << "Stopping current path analysis")
}

/**
 * @brief either we find SP in all the paths we merge (and the same SP), either we return elm::none
 */
Option<Constant> Analysis::getCurrentStackPointer(const SLList<Analysis::State>& sl) const
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
}

/**
 * @fn inline static bool Analysis::isConditional(Block* b);
 * @brief Test if a BasicBlock is conditional, i.e. has more than one edge
 * @param BasicBlock to test
 */
