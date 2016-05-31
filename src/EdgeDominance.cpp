/*
 * EdgeDominance.cpp
 */

#include <elm/genstruct/SLList.h>
#include "EdgeDominance.h"
#include "cfg_features.h"

using namespace elm;
using otawa::sgraph::Edge;

io::Output& operator<<(io::Output& out, otawa::sgraph::Edge* e) {
	return !e ? (out << "null") : (out << e->source()->index() << "->" << e->sink()->index());
}

// void Order::process(DiGraph *g, Edge* start)
void Order::process(DiGraph *g, genstruct::SLList<Edge*> S)
{
	/* O ← [] */ // order
	O.clear();
	/* S ← [e_ε] */ // stack
	// genstruct::SLList<Edge*> S;
	// S.add(start);
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
		for(Vertex::EdgeIter f(succ(e)); f; f++)
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