#ifndef _CFGANALYSIS_H
#define _CFGANALYSIS_H

#include <elm/imm/list.h>
#include "predicate.h"

using namespace otawa;
using namespace elm::imm;

// TODO: why doesn't it work?
template <class T> extern inline list<T> operator+=(const T& h, list<T> t) { return cons(h, t); }

class CFGAnalysis {
public:
	CFGAnalysis(CFG *cfg);
	
private:
	typedef list<Edge*> Path;
	
	// got to choose between american and british english
	class LabeledPredicate {
	private:
		Predicate _pred;
		Edge* _label;
	
	public:
		LabeledPredicate(Predicate pred, Edge* label);
		
		inline Predicate pred() const { return _pred; };
		inline Edge* label() const { return _label; };
	};
		
	// The actual struct 
	list<Path> infeasible_paths;
	list<list<LabeledPredicate> > labeled_preds;
	// bool ...;
	
	// Private methods
	void processBB(BasicBlock *bb);
	void processCFG(CFG *cfg);
	void processEdge(Edge *edge);
};

#endif
