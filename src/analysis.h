#ifndef _ANALYSIS_H
#define _ANALYSIS_H

#include <otawa/otawa.h>
#include <elm/genstruct/SLList.h>
#include "predicate.h"

using namespace otawa;
using namespace elm::genstruct;

template <class T> inline const SLList<T>& null(void) {
	static SLList<T> _null;
	return _null;
}
// SLList<int>test_null = (SLList<int>)null<int>();

template <class T> inline SLList<T>& operator+=(SLList<T> &t, const T& h) { t.addFirst(h); return t; }
template <class T> inline SLList<T>& operator+=(SLList<T> &t, const SLList<T>& l) { t.addAll(l); return t; }

template <class T> inline io::Output& operator<<(io::Output& out, const SLList<T>& l)
{
	out << "[ ";
	for(typename SLList<T>::Iterator elems(l); elems; elems++)
		out << *elems << ", ";
	out << "]";
	return out;
};

class Analysis {
public:
	Analysis(CFG *cfg);
	
private:
	typedef SLList<Edge*> Path;
	
	class LabelledPredicate {
	private:
		Predicate _pred;
		Edge* _label;
		io::Output& print(io::Output& out) const;
	
	public:
		LabelledPredicate(Predicate pred, Edge* label);
		
		// Accessors
		inline Predicate pred() const { return _pred; };
		inline Edge* label() const { return _label; };
		
		friend io::Output& operator<<(io::Output& out, const LabelledPredicate& lp) { return lp.print(out); }
	};
		
	// The actual struct 
	SLList<Path>						infeasible_paths;
	SLList<SLList<LabelledPredicate> >	labelled_preds;
	// bool								solverHasBeenCalled;
	
	// Private methods
	// analysis.cpp
	void initializeAnalysis();
	
	// analysis_cfg.cpp
	void processBB(BasicBlock *bb);
	void processCFG(CFG *cfg);
	void processEdge(Edge *edge);
	SLList<LabelledPredicate> labelPredicateList (SLList<Predicate> pred_list, Edge* label);
	
	// analysis_bb.cpp
	SLList<Predicate> analyzeBB(const BasicBlock *bb);
	
	// inline list<LabelledPredicate>& currentLPList() const { return labelled_preds.hd(); }
};

#endif
