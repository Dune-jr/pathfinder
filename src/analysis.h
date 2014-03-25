#ifndef _ANALYSIS_H
#define _ANALYSIS_H

#include <otawa/otawa.h>
#include <elm/imm/list.h>
#include "predicate.h"

using namespace otawa;
using namespace elm::imm;

template <class T> inline list<T>& operator+=(list<T> &t, const T& h) { return t = cons(h, t); }
template <class T> inline list<T>& operator+=(list<T> &t, list<T> l) { return t = concat(t, l); } // TODO check if it's the right way (instead of concat(l, t))

class Analysis {
public:
	Analysis(CFG *cfg);
	
private:
	typedef list<Edge*> Path;
	
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
	list<Path> 						infeasible_paths;
	list<list<LabelledPredicate> >	labeled_preds;
	// bool 						solverHasBeenCalled;
	
	// Private methods
	// analysis.cpp
	void initializeAnalysis();
	
	// analysis_cfg.cpp
	void processBB(BasicBlock *bb);
	void processCFG(CFG *cfg);
	void processEdge(Edge *edge);
	list<LabelledPredicate> labelPredicateList (list<Predicate> pred_list, Edge* label);
	
	// analysis_bb.cpp
	list<Predicate> analyzeBB(BasicBlock *bb);
	
	// inline list<LabelledPredicate>& currentLPList() const { return labeled_preds.hd(); }
};

#endif
