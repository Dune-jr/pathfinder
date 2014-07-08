#ifndef _ANALYSIS_H
#define _ANALYSIS_H

#include <otawa/cfg/Edge.h>
#include <elm/genstruct/SLList.h>
#include <otawa/sem/inst.h>
#include "predicate.h"
#include "debug.h"

using namespace otawa;
using namespace elm::genstruct; 

template <class T> inline const SLList<T>& null(void) {
	static SLList<T> _null;
	return _null;
}

template <class T> inline SLList<T>& operator+=(SLList<T> &t, const T& h) { t.addFirst(h); return t; }
template <class T> inline SLList<T>& operator+=(SLList<T> &t, const SLList<T>& l) { t.addAll(l); return t; }

void addIndents(io::Output& out, int n);
template <class T> io::Output& operator<<(io::Output& out, const SLList<T>& l);

class Analysis {
public:
	Analysis(CFG *cfg, int sp_id);
	
	typedef SLList<const Edge*> Path;
	
	class LabelledPredicate {
	private:
		Predicate _pred;
		SLList<const Edge*> _labels;
		io::Output& print(io::Output& out) const;
	
	public:
		LabelledPredicate(const Predicate& pred, SLList<const Edge*> labels);
		inline const Predicate& pred() const { return _pred; };
		inline const SLList<const Edge*>& labels() const { return _labels; };
		friend io::Output& operator<<(io::Output& out, const LabelledPredicate& lp) { return lp.print(out); }
	};
	
private:
	OperandVar sp; // the Stack Pointer register
	SLList<Predicate> generated_preds;
	SLList<Predicate> generated_preds_taken; // if there is a conditional, the taken preds will be saved here
											 // and the not taken preds will stay in generated_preds
	// The actual struct
	SLList<Path>						infeasible_paths;
	SLList<SLList<LabelledPredicate> >	labelled_preds;
	// bool								solverHasBeenCalled;

 	// this is reset before any BB analysis, indicates previously generated preds (in another BB) that have been updated and need
 	// to have their labels list updated (add the next edge to the LabelledPreds struct)
	SLList<SLList<LabelledPredicate>* > updated_preds;
	
	// Private methods
	// analysis.cpp
	void initializeAnalysis();
	SLList<LabelledPredicate> getTopList();
	void setTopList(const SLList<LabelledPredicate>& lps);
	void addElemToTopList(const SLList<LabelledPredicate>& lps);
	
	// analysis_cfg.cpp
	void processBB(BasicBlock *bb);
	void processCFG(CFG *cfg);
	void processEdge(const Edge *edge);
	SLList<LabelledPredicate> labelPredicateList (const SLList<Predicate>& pred_list, const Edge* label);
	
	// analysis_bb.cpp
	void analyzeBB(const BasicBlock *bb);
	bool invalidateVar(const OperandVar& var);
	inline bool invalidateVar(const t::int32& addr) { return invalidateVar(OperandVar(addr)); }
	bool invalidateMem(const OperandMem& addr);
	bool invalidateMem(const OperandVar& var);
	inline bool invalidateMem(const t::int32& addr) { return invalidateMem(OperandVar(addr)); }
	bool invalidateTempVars();
	bool replaceTempVar				 (const OperandVar& temp_var, const Operand& expr);
	bool update 					 (const OperandVar& opd_to_update, const Operand& opd_modifier);
	bool findConstantValueOfVar		 (const OperandVar& var, t::int32& val);
	bool findStackRelativeValueOfVar (const OperandVar& var, t::int32& val);
	bool findValueOfCompVar			 (const OperandVar& var, Operand*& opd_left, Operand*& opd_right);
	Option<OperandMem> getOperandMem (const OperandVar& var);
	void invalidateAllMemory();
	Predicate* getPredicateGeneratedByCondition(sem::inst condition, bool taken);
};

template <class T> io::Output& operator<<(io::Output& out, const SLList<T>& l)
{
	int count = l.count();
	if(count > 10)
	{
		out << "#" << count << "#";
		return out; // just exit if too many items
	}
	
	static int indent = 0;
	bool indented_output = (count > 5);
	
	if(indented_output)
		addIndents(out, indent++);
	out << COLOR_Bold << "[" << COLOR_NoBold;
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
	
	return (out << COLOR_Bold << "]" << COLOR_NoBold);
};

#endif
