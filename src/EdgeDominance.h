/*
 * EdgeDominance.h
 */

#ifndef EDGEDOMINANCE_H
#define EDGEDOMINANCE_H

#include <otawa/cfg.h>
#include <otawa/cfg/features.h>
#include <otawa/sgraph/DiGraph.h>
#include <otawa/prog/WorkSpace.h>
#include <elm/genstruct/SLList.h>
#include <elm/genstruct/HashTable.h>
#include "debug.h"

using namespace elm;
// using namespace elm::genstruct;
using namespace elm::color;
using namespace otawa;
using namespace otawa::sgraph; // DiGraph, Vertex, ...

Output& operator<<(Output& out, Vertex* b);

class RPO {
public:
	RPO(DiGraph* g) { process(g); }
	const SLList<Vertex*>& toList() const { return O; }
	bool ordered(Vertex* a, Vertex* b) const { // test a <= b
		for(Iterator i(O); i; i++) {
			if(*i == a) return true; // a <= b
			if(*i == b) return false; // a >  b
		}
		ASSERTP(false, "rpo.ordered found neither " << a << " nor " << b);assert(false);
	}
	inline bool strictordered(Vertex* a, Vertex* b) const { return !ordered(a,b); } // a < b = !(b <= a)

	typedef genstruct::SLList<Vertex*>::Iterator Iterator;

private:
	void process(DiGraph *g);
	genstruct::SLList<Vertex*> O;
};


class EdgeDominance {
public:
	EdgeDominance(/*WorkSpace *ws, */CFG *cfg) : rpo(cfg), indent_level(0) {
		// ws->require(COLLECTED_CFG_FEATURE, props); // INVOLVED_CFGS
		process(cfg);
	}

private:
	void process(DiGraph *g) {
		// RPO rpo(g);
		DBG("rpo= " << rpo.toList())
		DominanceProblem(g);
		DBG("dom= " << *this)
	}

	// dominance (new)
	void DominanceProblem(DiGraph *g) {
		/* for all nodes, b // initialize the dominators array */
			/* doms[b] ← Undefined */
		doms.clear();
		/* doms[start node] ← start node */
		doms.put(g->entry(),g->entry());
		/* Changed ← true */
		bool changed = true;
		/* while (Changed) */
		while(changed) {
			/* Changed ← false */
			changed = false;
			/* for all nodes, b, in reverse postorder (except start node) */
			for(RPO::Iterator b(rpo.toList()); b; b++) {
				// DBG("  b= " << *b)
				if(b == g->entry())
					continue;
				/* new idom ← first (processed) predecessor of b // (pick one) */
				Vertex* new_idom = NULL;
				/* for all other predecessors, p, of b */
				for(Vertex::EdgeIter pe(b->ins()); pe; pe++) {
					/* if doms[p] != Undefined // i.e., if doms[p] already calculated */
					if(doms.exists(pe->source()))
						/* new idom ← intersect(p, new idom) */
						new_idom = (new_idom==NULL) ? pe->source() : intersect(pe->source(), new_idom);
				}
				ASSERT(new_idom);
				/* if doms[b] != new idom */
				if(doms.get(b, NULL) != new_idom) {
					/* doms[b] ← new idom */
					doms.put(b, new_idom);
					/* Changed ← true */
					changed = true;
				}
			}
		}
	}
	Vertex* intersect(Vertex* b1, Vertex* b2) {
		// DBG("    intersect(" << b1 << ", " << b2 << ")")
		/* finger1 ← b1 */
		/* finger2 ← b2 */
		Vertex *finger1 = b1, *finger2 = b2;
		/* while (finger1 != finger2) */
		while(finger1 != finger2) {
			/* while (finger1 < finger2) */
			while(rpo.strictordered(finger1, finger2))
				/* finger1 = doms[finger1] */
				finger1 = doms[finger1];
			// DBG("             (" << finger1 << ", " << finger2 << ")")
			/* while (finger2 < finger1) */
			while(rpo.strictordered(finger2, finger1))
				/* finger2 = doms[finger2] */
				finger2 = doms[finger2];
			// DBG("             (" << finger1 << ", " << finger2 << ")")
		}
		// DBG("      = " << finger1)
		/* return finger1 */
		return finger1;
	}
	io::Output& print(io::Output& out) const; // print doms
	inline friend io::Output& operator<<(io::Output& out, const EdgeDominance& ed) { return ed.print(out); }
	RPO rpo;
	genstruct::HashTable<Vertex*, Vertex*> doms;

	// dominance (old)
	Vertex* inter(Vertex* a, Vertex* b);
	Vertex* intersectOld(Vertex::EdgeIter edges);
	void DominanceProblemOld(DiGraph* g);
	// genstruct::HashTable<Vertex*, Vertex*> doms;

	// RPO (old)
	void DepthFirstOrderOld(DiGraph *g);
	void dfs(Vertex* b);
	elm::String indent(int indent_increase = 0);
	genstruct::SLList<Vertex*> reversePostOrder;

	int indent_level;
};

#endif /* EDGEDOMINANCE_H */
