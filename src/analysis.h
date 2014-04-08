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
	if(count > 10)
	{
		out << "#" << count << "#";
		return out; // just exit if too many items
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
		const Edge* _label;
		io::Output& print(io::Output& out) const;
	
	public:
		LabelledPredicate(const Predicate& pred, const Edge* label);
		
		// Accessors
		inline const Predicate& pred() const { return _pred; };
		inline const Edge* label() const { return _label; };
		
		friend io::Output& operator<<(io::Output& out, const LabelledPredicate& lp) { return lp.print(out); }
	};
	
	SLList<Predicate> generated_preds;
	int max_tempvars; // This is more of an optimization thing than a necessary measure
	
	// The actual struct
	SLList<Path>						infeasible_paths;
	SLList<SLList<LabelledPredicate> >	labelled_preds;
	// bool								solverHasBeenCalled;
	
	// Private methods
	// analysis.cpp
	void initializeAnalysis();
	SLList<LabelledPredicate> getTopList();
	void setTopList(const SLList<LabelledPredicate>& lps);
	
	// analysis_cfg.cpp
	void processBB(BasicBlock *bb);
	void processCFG(CFG *cfg);
	void processEdge(Edge *edge);
	SLList<LabelledPredicate> labelPredicateList (const SLList<Predicate>& pred_list, Edge* label);
	
	// analysis_bb.cpp
	void analyzeBB(const BasicBlock *bb);
	bool invalidateVar(const OperandVar& var);
	bool invalidateTempVars();
	bool replaceTempVar(const OperandVar& temp_var, const Operand& expr);
	bool findConstantValueOfTempVar(const OperandVar& var, t::int32& val);
	bool update(const OperandVar& opd_to_update, const Operand& opd_modifier);
	bool updateAdd(OperandVar opd_to_update, OperandVar opd_modifier);
	bool updateSub(OperandVar opd_to_update, OperandVar opd_modifier, bool reverse);
	bool seekValue(const OperandVar& var, OperandConst& val);
	
	// inline list<LabelledPredicate>& currentLPList() const { return labelled_preds.hd(); }
};

#endif
