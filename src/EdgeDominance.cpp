/*
 * EdgeDominance.cpp
 */

#include <elm/genstruct/SLList.h>
#include "EdgeDominance.h"
#include "cfg_features.h"

using namespace elm;
using otawa::sgraph::Edge;

io::Output& operator<<(io::Output& out, otawa::sgraph::Edge* e) {
	return (out << e->source()->index() << "->" << e->sink()->index());
}

void EdgeDominance::RPO::process(DiGraph *g)
{
	/* O ← [] */ // order
	O.clear();
	/* S ← [e_ε] */ // stack
	genstruct::SLList<Edge*> S;
	S.add(theOnly(g->entry()->outs()));
	/* D ← { } */ // done
	genstruct::Vector<Edge*> D;
	/* while S ≠ [] do */
	while(S) {
		/* v ← hd S */
		Edge* e = S.first();
		/* D ← D ∪ { e } */
		if(! D.contains(e))
			D.push(e);
		/* if ∃ (e, f) ∈ V, f ∉ D then, for each, */
		bool all_done = true;
		for(Vertex::EdgeIter f(e->sink()->outs()); f; f++)
			if(! D.contains(*f)) {
				all_done = false;
				/* S ← f :: S */
				S.addFirst(*f);
				break;
			}
		/* else */
		if(all_done) {
			/* O ← e :: O */
			O.addFirst(e);
			/* S ← tl S */
			S.removeFirst();
		}
	}
}

void EdgeDominance::DominanceProblem(DiGraph *g) {
	Edge* program_entry_edge = theOnly(g->entry()->outs());
	/* for all nodes, e // initialize the dominators array */
		/* doms[e] ← Undefined */
	doms.clear();
	/* doms[start edge] ← start edge */
	doms.put(program_entry_edge, program_entry_edge);
	/* Changed ← true */
	bool changed = true;
	/* while (Changed) */
	while(changed) {
		/* Changed ← false */
		changed = false;
		/* for all nodes, e, in reverse postorder (except start node) */
		for(RPO::Iterator e(rpo.toList()); e; e++) {
			if(e == program_entry_edge)
				continue;
			/* new idom ← first (processed) predecessor of e // (pick one) */
			Edge* new_idom = NULL;
			/* for all other predecessors, p, of e */
			for(Vertex::EdgeIter p(e->source()->ins()); p; p++) {
				/* if doms[p] != Undefined // i.e., if doms[p] already calculated */
				if(doms.exists(p))
					/* new idom ← intersect(p, new idom) */
					new_idom = (new_idom==NULL) ? p : intersect(p, new_idom);
			}
			ASSERT(new_idom);
			/* if doms[e] != new idom */
			if(doms.get(e, NULL) != new_idom) {
				/* doms[e] ← new idom */
				doms.put(e, new_idom);
				/* Changed ← true */
				changed = true;
			}
		}
	}
}

Edge* EdgeDominance::intersect(Edge* b1, Edge* b2) {
	// DBG("    intersect(" << b1 << ", " << b2 << ")")
	/* finger1 ← b1 */
	/* finger2 ← b2 */
	Edge *finger1 = b1, *finger2 = b2;
	/* while (finger1 != finger2) */
	while(finger1 != finger2) {
		/* while (finger1 < finger2) */
		while(rpo.strictOrdered(finger1, finger2))
			/* finger1 = doms[finger1] */
			finger1 = doms[finger1];
		// DBG("             (" << finger1 << ", " << finger2 << ")")
		/* while (finger2 < finger1) */
		while(rpo.strictOrdered(finger2, finger1))
			/* finger2 = doms[finger2] */
			finger2 = doms[finger2];
		// DBG("             (" << finger1 << ", " << finger2 << ")")
	}
	// DBG("      = " << finger1)
	/* return finger1 */
	return finger1;
}

io::Output& EdgeDominance::print(io::Output& out) const {
	bool first = true;
	for(genstruct::HashTable<Edge*, Edge*>::PairIterator i(doms); i; i++) {
		out << (first?"":",  ") << (*i).fst << ":" << (*i).snd;
		first = false;
	}
	return out;
}
