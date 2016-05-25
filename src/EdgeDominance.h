/*
 * EdgeDominance.h
 */

#ifndef EDGEDOMINANCE_H
#define EDGEDOMINANCE_H

#include <otawa/cfg.h>
#include <otawa/cfg/features.h>
#include <otawa/graph/GenGraph.h>
#include <otawa/sgraph/DiGraph.h>
#include <otawa/prog/WorkSpace.h>
#include <elm/genstruct/SLList.h>
#include <elm/genstruct/HashTable.h>
#include "debug.h"

using namespace elm;
using namespace elm::color;
// using namespace otawa::graph; // GenGraph
using otawa::sgraph::DiGraph;
using otawa::sgraph::Vertex;

io::Output& operator<<(io::Output& out, otawa::sgraph::Edge* e); // TODO

class EdgeDominance {
public:
	class RPO {
	public:
		RPO(DiGraph* g) { process(g); }
		const genstruct::SLList<otawa::sgraph::Edge*>& toList() const { return O; }
		bool ordered(otawa::sgraph::Edge* a, otawa::sgraph::Edge* b) const { // test a <= b
			for(Iterator i(O); i; i++) {
				if(*i == a) return true; // a <= b
				if(*i == b) return false; // a > b
			}
			ASSERTP(false, "rpo.ordered found neither " << a << " nor " << b);assert(false);
		}
		inline bool strictOrdered(otawa::sgraph::Edge* a, otawa::sgraph::Edge* b) const { return !ordered(a,b); } // a < b = !(b <= a)

		typedef genstruct::SLList<otawa::sgraph::Edge*>::Iterator Iterator;

	private:
		void process(DiGraph *g);
		genstruct::SLList<otawa::sgraph::Edge*> O;
	};

	EdgeDominance(/*WorkSpace *ws, */DiGraph *g) : rpo(g) {
		process(g);
	}
	otawa::sgraph::Edge* idom(otawa::sgraph::Edge* e) {
		ASSERT(doms.exists(e))
		return doms.get(e);
	}

private:
	void process(DiGraph *g) {
		// DBG("rpo= " << rpo.toList())
		DominanceProblem(g);
	}
	// dominance (new)
	void DominanceProblem(DiGraph *g);
	otawa::sgraph::Edge* intersect(otawa::sgraph::Edge* b1, otawa::sgraph::Edge* b2);
	io::Output& print(io::Output& out) const; // print doms
	inline friend io::Output& operator<<(io::Output& out, const EdgeDominance& ed) { return ed.print(out); }
	
	RPO rpo;
	genstruct::HashTable<otawa::sgraph::Edge*, otawa::sgraph::Edge*> doms;
};

#endif /* EDGEDOMINANCE_H */
