#ifndef _CONSTANT_VARIABLES_H
#define _CONSTANT_VARIABLES_H

#include <elm/avl/Set.h>
#include <elm/genstruct/SLList.h>
#include <otawa/cfg/Edge.h>
#include "debug.h"
#include "constant_variables_core.h"
#include "labelled_predicate.h"

using namespace otawa;
using namespace elm::genstruct; 
using namespace elm::avl;

class ConstantVariables : public ConstantVariablesCore
{
public:
	ConstantVariables() : ConstantVariablesCore() { } // invalid
	ConstantVariables(unsigned int max_tempvars, unsigned int max_registers) : ConstantVariablesCore(max_tempvars, max_registers) { }
	ConstantVariables(const ConstantVariables& cv) : ConstantVariablesCore(cv) { }
	~ConstantVariables() { }

	inline bool isConstant(const OperandVar& opdv) const { return ConstantVariablesCore::isConstant(opdv.addr()); }
	void merge(const SLList<ConstantVariables>& cvl);
	inline Constant getValue(const OperandVar& opdv) const
		{ return ConstantVariablesCore::getValue(opdv.addr()); } // this must not be called if !isConstant(opdv)...
	Set<Edge*> getLabels(const OperandVar& opdv) const; // same
	Set<Edge*> getLabels(const OperandVar& opdv1, const OperandVar& opdv2) const;
	// template<typename First, typename ... OpdVars>
	// 	void getLabelsOf(First head, const OpdVars&... tail) const;
	ConstantVariables::LabelledValue getLabelledValue(const OperandVar& opdv) const; // same
		   void set(const OperandVar& opdv, const LabelledValue& lval);
		   void set(const OperandVar& opdv, const Constant& val, const Set<Edge*>& labels = Set<Edge*>::null, bool updated_flag = true);
	inline void set(const OperandVar& opdv, const OperandConst& opdc, const Set<Edge*>& labels = Set<Edge*>::null, bool updated_flag = true) { set(opdv, opdc.value(), labels, updated_flag); }
		   void update(const OperandVar& opdv, const Constant& val, bool updated_flag = true);
	inline void update(const OperandVar& opdv, const OperandConst& opdc, bool updated_flag = true) { update(opdv, opdc.value(), updated_flag); }
	void invalidate(const OperandVar& opdv);
	inline void invalidateOperand(const Operand& opd)
		{ if(opd.kind() == VAR) invalidate((const OperandVar&)opd); } // doesn't do anything in the case of OperandMem
	bool invalidateTempVars();
	SLList<LabelledPredicate> toPredicates() const;
	inline Constant operator[](const OperandVar& opdv) const { return getValue(opdv); }
	friend io::Output& operator<<(io::Output& out, const ConstantVariables& cv) { return operator<<(out, static_cast<ConstantVariablesCore>(cv)); }

private:
	inline LabelledValue& getCell(const OperandVar& opdv) const
		{ return ConstantVariablesCore::getCell(opdv.addr()); }
}; // ConstantVariables class 

#endif
