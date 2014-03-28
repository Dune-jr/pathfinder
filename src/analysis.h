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

template <class T> inline SLList<T>& operator+=(SLList<T> &t, const T& h) { t.addFirst(h); return t; }
template <class T> inline SLList<T>& operator+=(SLList<T> &t, const SLList<T>& l) { t.addAll(l); return t; }

void addIndents(io::Output& out, int n);
template <class T> io::Output& operator<<(io::Output& out, const SLList<T>& l)
{
	int count = l.count();
	if(count > 9)
	{
		out << "#" << count << """#";
		return out; // For now, we just exit if too many items
	}
	
	static int indent = 0;
	bool indented_output = (count > 4);
	
	if(indented_output)
		addIndents(out, indent++);
	out << "[";
	if(indented_output)
		out << io::endl;
	bool first = true;
	for(typename SLList<T>::Iterator elems(l); elems; elems++)
	{
		if(first)
		{
			if(indented_output)
				addIndents(out, indent);
			out << *elems;
			first = false;
		}
		else
		{
			out << ", ";
			if(indented_output)
			{
				out << io::endl;
				addIndents(out, indent);
			}
			out << *elems;
		}
	}
	if(indented_output)
	{
		out << io::endl;
		addIndents(out, --indent);
	}
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
		LabelledPredicate(const Predicate& pred, Edge* label);
		
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
	SLList<LabelledPredicate> labelPredicateList (const SLList<Predicate>& pred_list, Edge* label);
	
	// analysis_bb.cpp
	SLList<Predicate> analyzeBB(const BasicBlock *bb);
	
	// inline list<LabelledPredicate>& currentLPList() const { return labelled_preds.hd(); }
};

#endif
