/*
 * EdgeDominance.cpp
 */

#include "EdgeDominance.h"

void RPO::process(DiGraph *g)
{
	/* O ← [] */ // order
	O.clear();
	/* S ← [ε] */ // stack
	genstruct::SLList<Vertex*> S;
	S.add(g->entry());
	/* D ← { } */ // done
	genstruct::Vector<Vertex*> D;
	/* while S ≠ [] do */
	while(S) {
		/* v ← hd S */
		Vertex* v = S.first();
		/* D ← D ∪ { v } */
		if(! D.contains(v))
			D.push(v);
		/* if ∃ (v, w) ∈ E, w ∉ D then */
		bool all_done = true;
		for(Vertex::EdgeIter e(v->outs()); e; e++)
			if(! D.contains(e->sink())) {
				all_done = false;
				/* S ← w :: S */
				S.addFirst(e->sink());
			}
		/* else */
		if(all_done) {
			/* O ← v :: O */
			O.addFirst(v);
			/* S ← tl S */
			S.removeFirst();
		}
	}
}

void EdgeDominance::DominanceProblem(DiGraph *g) {
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

Vertex* EdgeDominance::intersect(Vertex* b1, Vertex* b2) {
	// DBG("    intersect(" << b1 << ", " << b2 << ")")
	/* finger1 ← b1 */
	/* finger2 ← b2 */
	Vertex *finger1 = b1, *finger2 = b2;
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
	for(genstruct::HashTable<Vertex*, Vertex*>::PairIterator i(doms); i; i++) {
		out << (first?"":",  ") << (*i).fst << ":" << (*i).snd;
		first = false;
	}
	return out;
}

io::Output& operator<<(io::Output& out, Vertex* b)
{
	return (out << b->index());
}

#if 0
Vertex* EdgeDominance::inter(Vertex* a, Vertex* b) {
	ASSERTP(a && b, "a: " << a->index() << ", b: " << b->index());
	DBG("    inter(" << a->index() << ", " << b->index() << ")");
	if(a == b) {
		DBG("      = " << a)
		return a;
	}
	SLList<Vertex*> al, bl;
	al.addFirst(a);
	bl.addFirst(b);
	while(doms[a] != a) {
		a = doms[a];
		al.addFirst(a);
	}
	while(doms[b] != b) {
		b = doms[b];
		bl.addFirst(b);
	}
	DBG(IPur() << "        " << al)
	DBG(IPur() << "      & " << bl)
	Vertex* rtn = NULL;
	for(SLList<Vertex*>::Iterator ai(al), bi(bl); ai && bi && *ai == *bi; ai++, bi++)
		{ rtn = *ai; }
	ASSERT(rtn);
	DBG("      = " << rtn)
	return rtn;
}
Vertex* EdgeDominance::intersectOld(Vertex::EdgeIter edges) {
	Vertex* rtn = NULL;
	for(; edges; edges++)
		if(doms.exists(edges->source())) { // otherwise is a back edge!
			if(! rtn)
				rtn = edges->source();
			else
				rtn = inter(rtn, edges->source());
		}
	ASSERTP(rtn, "only back edges???");
	return rtn;
}

void EdgeDominance::DominanceProblemOld(DiGraph *g) {
	// DepthFirstOrder(g);
	/*for all nodes, b //initialize the dominators array */
		/*doms[b] ← Undefined*/
	/*doms[start node] ← start node*/
	doms.put(g->entry(),g->entry());
	bool changed;
	int iter = 0;
	/*do*/
	do {
		DBG("iter#" << iter++)
		/*Changed ← false*/
		changed = false;

		Vector<Vertex*> todo, done;
		for(Vertex::EdgeIter iter(g->entry()->outs()); iter; iter++)
			todo.push(iter->sink());
		/*for all nodes, b, in reverse postorder (except start node)*/
		while(todo) {
			Vertex *b = todo.pop();
			DBG("  pop " << b->index())
			/*new idom ← first (processed) predecessor of b //(pick one) */
			/*for all other predecessors, p, of b*/
			/*if doms[p] != Undefined // i.e., if doms[p] already calculated */
				/*new idom ← intersectOld(p, new idom)*/
			Vertex *new_idom = intersectOld(b->ins());
			/*if doms[b] != new idom*/
			if(doms.get(b, NULL) != new_idom) {
				/*doms[b] ← new idom*/
				doms.put(b, new_idom);
				DBG("    idom= " << new_idom->index())
				/*Changed ← true*/
				changed = true;
			}
			for(Vertex::EdgeIter iter(b->outs()); iter; iter++) { // push outs 
				if(! done.contains(iter->sink())) // ensure we don't parse loops
					if(! todo.contains(iter->sink())) // ensure unicity in todo
						todo.push(iter->sink());
						// DBG("    push " << iter->sink()->index())
					}
			done.push(b);
		}
	/*while (Changed)*/
	} while(changed);
}

void EdgeDominance::DepthFirstOrderOld(DiGraph *g) {
	dfs(g->entry());
	elm::String str = "result: ";
	for(genstruct::SLList<Vertex*>::Iterator iter(reversePostOrder); iter; iter++)
		str = str.concat(_ << iter->index() << ", ");
	DBG(str);
}
void EdgeDominance::dfs(Vertex* b) {
	if(! reversePostOrder.contains(b)) {
		DBG(indent( ) << IPur() << "dfs(" << b->index() << ")"); indent(+1);
		for(Vertex::EdgeIter i(b->outs()); i; i++)
			dfs(i->sink());
		reversePostOrder.addFirst(b);
		DBG(indent(-1) << IGre() << b->index() << " done")
	}
	else
		DBG(indent( ) << ICya() << "check " << b->index() )
}

elm::String EdgeDominance::indent(int indent_increase)
{
	elm::String str;
	indent_level += indent_increase;
	if(indent_level <= 0)
		indent_level = 0; // cannot have negative indent levels
	else 
		for(int i = 0; i < indent_level; i++)
			str = str + "  ";
	return str;
}
#endif
