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
	Analysis(CFG *cfg, int sp_id, unsigned int max_tempvars, unsigned int max_registers);
	
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

	class ConstantVariables {
	private:
		Option<t::int32>* tempvars;
		Option<t::int32>* registers;
		unsigned int _max_tempvars;
		unsigned int _max_registers;

		Option<t::int32>& getCell(const OperandVar& opdv) const;
		io::Output& print(io::Output& out) const;

	public:
		ConstantVariables(unsigned int max_tempvars, unsigned int max_registers);
		ConstantVariables(const ConstantVariables& cv);
		// ~ConstantVariables(); // TODO
		inline unsigned int maxTempVars() const { return _max_tempvars; }
		inline unsigned int maxRegisters() const { return _max_registers; }
			   bool isConstant(const OperandVar& opdv) const;
		inline bool isConstant(t::int32 var_id) const { return isConstant(OperandVar(var_id)); }
			   t::int32 getValue(const OperandVar& opdv) const; // this must not be called if !isConstant(opdv)...
		inline t::int32 getValue(t::int32 var_id) const { return getValue(OperandVar(var_id)); }
			   void set(const OperandVar& opdv, t::int32 val);
		inline void set(const t::int32 var_id, OperandConst opdc) { set(OperandVar(var_id), opdc.value()); }
		inline void set(const t::int32 var_id, t::int32 val) { set(OperandVar(var_id), val); }
		inline void set(const OperandVar& opdv, OperandConst opdc) { set(opdv, opdc.value()); }
			   void invalidate(const OperandVar& opdv);
		inline void invalidate(t::int32 var_id) { invalidate(OperandVar(var_id)); }
		void invalidateTempVars();
		inline t::int32 operator[](const OperandVar& opdv) const { return getValue(opdv); }
		inline t::int32 operator[](t::int32 var_id) const { return getValue(var_id); }
		ConstantVariables& operator=(const ConstantVariables& cv);
		friend io::Output& operator<<(io::Output& out, const ConstantVariables& cv) { return cv.print(out); }
	};

	enum
	{
		KEEP_CONSTANT_INFO = false,
		INVALIDATE_CONSTANT_INFO = true,
	};
	
private:
	OperandVar sp; // the Stack Pointer register
	ConstantVariables constants; // Remember in an array the variables that have been identified to a constant (e.g. t2 = 4)

	SLList<Predicate> generated_preds;
	SLList<Predicate> generated_preds_taken; // if there is a conditional, the taken preds will be saved here and the not taken preds will stay in generated_preds
	
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
	bool invalidateVar(const OperandVar& var, bool invalidate_constant_info = true);
	inline bool invalidateVar(const t::int32& addr, bool invalidate_constant_info = true) { return invalidateVar(OperandVar(addr), invalidate_constant_info); }
	bool invalidateMem(const OperandMem& addr);
	bool invalidateMem(const OperandVar& var);
	inline bool invalidateMem(const t::int32& addr) { return invalidateMem(OperandVar(addr)); }
	bool invalidateTempVars();
	bool replaceTempVar(const OperandVar& temp_var, const Operand& expr);
	bool update(const OperandVar& opd_to_update, const Operand& opd_modifier);
	Option<t::int32> findConstantValueOfVar(const OperandVar& var); // changed to a simple lookup to "constants"
	bool findConstantValueOfVar_old(const OperandVar& var, t::int32& val); // old version may be better?... think about a case where t1 is sp + 4 + 2 + 6
	Option<t::int32> findStackRelativeValueOfVar(const OperandVar& var);
	bool findValueOfCompVar(const OperandVar& var, Operand*& opd_left, Operand*& opd_right);
	Option<OperandMem> getOperandMem(const OperandVar& var);
	void invalidateAllMemory();
	Predicate* getPredicateGeneratedByCondition(sem::inst condition, bool taken);
	inline bool isConstant(t::int32 var_id)		  const { return constants.isConstant(var_id); }
	inline bool isConstant(const OperandVar& var) const { return constants.isConstant(var); }
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
