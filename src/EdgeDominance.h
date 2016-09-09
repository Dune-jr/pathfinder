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

#define VIRTUAL_ENTRY_EDGE NULL

using namespace elm;
using namespace elm::color;
using otawa::sgraph::DiGraph;
using otawa::sgraph::Vertex;

io::Output& operator<<(io::Output& out, otawa::sgraph::Edge* e); // TODO

class Order {
public:
	typedef otawa::sgraph::Edge edge_t;
	// to implement
	virtual Vertex::EdgeIter pred(edge_t* e) const = 0;
	virtual Vertex::EdgeIter succ(edge_t* e) const = 0;

	// Order(DiGraph* g, edge_t* start) { /*process(g, start);*/ }
	const genstruct::SLList<edge_t*>& toList() const { return O; }
	bool ordered(edge_t* a, edge_t* b) const { // test a <= b
		if(!a) return true; // a=NULL: a <= b
		if(!b) return false; // b=NULL: a < b
		for(Iterator i(O); i; i++) {
			if(*i == b) return true; // a <= b
			if(*i == a) return false; // a > b
		}
		ASSERTP(false, "Order::ordered found neither " << a << " nor " << b);
	}
	inline bool strictOrdered(edge_t* a, edge_t* b) const { return !ordered(b,a); } // a < b = !(b <= a)

	typedef genstruct::SLList<edge_t*>::Iterator Iterator;
protected:
	// void process(DiGraph *g, edge_t* start);
	void process(DiGraph *g, genstruct::SLList<edge_t*> S);
private:
	genstruct::SLList<edge_t*> O;
};

template <class O> class EdgeDominance;

// Reverse Post Order (for dominance)
class RPO : public Order {
public:
	RPO(DiGraph* g, const genstruct::SLList<edge_t*>& start) { process(g, start); }
	inline virtual Vertex::EdgeIter pred(edge_t* e) const { return e->source()->ins(); }
	inline virtual Vertex::EdgeIter succ(edge_t* e) const { return e->sink()->outs(); }
};
typedef EdgeDominance<RPO> EdgeDom;

// Pre(Post?) Order (for post dominance)
class PO : public Order {
public:
	PO(DiGraph* g, const genstruct::SLList<edge_t*>& start) { process(g, start); }
	inline virtual Vertex::EdgeIter pred(edge_t* e) const { return e->sink()->outs(); }
	inline virtual Vertex::EdgeIter succ(edge_t* e) const { return e->source()->ins(); }
};
typedef EdgeDominance<PO> EdgePostDom;

template <class O> // chosen order
class EdgeDominance {
public:
	typedef otawa::sgraph::Edge edge_t;

	EdgeDominance(DiGraph *g, genstruct::SLList<edge_t*> start) : o(g, start) {
		DominanceProblem(g, start);
	}
	edge_t* idom(edge_t* e) {
		ASSERT(doms.exists(e));
		return doms.get(e);
	}

private:
	void DominanceProblem(DiGraph *g, genstruct::SLList<edge_t*> start) {
		bool multiple_start = start.count() > 1;
		/* for all nodes, e // initialize the dominators array */
			/* doms[e] ← Undefined */
		doms.clear();
		/* doms[start edge] ← start edge */
		// doms.put(start, start);
		if(multiple_start) doms.put(VIRTUAL_ENTRY_EDGE, VIRTUAL_ENTRY_EDGE); // hack
		for(genstruct::SLList<edge_t*>::Iterator i(start); i; i++)
			if(multiple_start) doms.put(*i, VIRTUAL_ENTRY_EDGE); // hack	
			else
				doms.put(*i, *i);
		/* Changed ← true */
		bool changed = true;
		/* while (Changed) */
		while(changed) {
			/* Changed ← false */
			changed = false;
			/* for all nodes, e, in reverse postorder (except start node) */
			for(typename O::Iterator e(o.toList()); e; e++) {
				// if(e == start)
				if(start.contains(e))
					continue;
				/* new idom ← first (processed) predecessor of e // (pick one) */
				Option<edge_t*> new_idom = none;
				/* for all other predecessors, p, of e */
				for(Vertex::EdgeIter p(o.pred(e)); p; p++) {
					/* if doms[p] != Undefined // i.e., if doms[p] already calculated */
					if(doms.exists(p))
						/* new idom ← intersect(p, new idom) */
						new_idom = new_idom ? intersect(p, new_idom) : p;
				}
				ASSERT(new_idom.isOne());
				/* if doms[e] != new idom */
				if(!doms.exists(e) || doms.get(e) != *new_idom) {
					/* doms[e] ← new idom */
					doms.put(e, new_idom);
					/* Changed ← true */
					changed = true;
				}
			}
		}
	}

	edge_t* intersect(edge_t* b1, edge_t* b2) {
		// cout << "intersect(" << b1 << ", " << b2 << ")\n";
		/* finger1 ← b1 */
		/* finger2 ← b2 */
		edge_t *finger1 = b1, *finger2 = b2;
		/* while (finger1 != finger2) */
		while(finger1 != finger2) {
			/* while (finger1 < finger2) */
			while(o.strictOrdered(finger1, finger2)) {
				/* finger1 = doms[finger1] */
				finger1 = doms[finger1];
				if(!finger1) return VIRTUAL_ENTRY_EDGE;
				// DBG("finger1= " << finger1)
			}
			/* while (finger2 < finger1) */
			while(o.strictOrdered(finger2, finger1)) {
				/* finger2 = doms[finger2] */
				finger2 = doms[finger2];
				if(!finger2) return VIRTUAL_ENTRY_EDGE;
				// DBG("finger2= " << finger2)
			}
		}
		/* return finger1 */
		return finger1;
	}

	io::Output& print(io::Output& out) const {
		bool first = true;
		for(genstruct::HashTable<edge_t*, edge_t*>::PairIterator i(doms); i; i++) {
			out << (first?"":",  ") << (*i).fst << ":" << (*i).snd;
			first = false;
		}
		return out;
	}
	inline friend io::Output& operator<<(io::Output& out, const EdgeDominance& ed) { return ed.print(out); }
	
	O o;
	genstruct::HashTable<edge_t*, edge_t*> doms;
};

#endif /* EDGEDOMINANCE_H */
