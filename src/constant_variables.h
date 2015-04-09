#ifndef _CONSTANT_VARIABLES_H
#define _CONSTANT_VARIABLES_H

#include <elm/genstruct/SLList.h>
#include <otawa/cfg/Edge.h>
#include <otawa/sem/inst.h>
#include "constant_variables_simplified.h"
#include "labelled_predicate.h"
#include "debug.h"

using namespace otawa;
using namespace elm::genstruct; 

// ConstantVariables class
class ConstantVariables {
private:
	// LabelledValue strcut
	class LabelledValue
	{
	public:
		LabelledValue() { }
		LabelledValue(const Constant& val, Set<Edge*> labels, bool updated = false) : _val(val), _labels(labels), _updated(updated) { }
		inline Constant val() const { return _val; }
		inline const Set<Edge*>& labels() const { return _labels; }
		inline bool isUpdated() const { return _updated; }
		inline void addLabel(Edge* label) { if(!_labels.contains(label)) _labels.add(label); }
		// inline void clearLabels() { _labels.clear(); }
		inline void setUpdatedFlag(bool updated = true) { _updated = updated; }
		LabelledValue& operator=(const LabelledValue& lv);
		bool operator==(const LabelledValue& lv) const;
		inline bool operator!=(const LabelledValue& lv) const { return !(lv == *this); }
		friend io::Output& operator<<(io::Output& out, const LabelledValue& lv) { return lv.print(out); }

	private:
		Constant _val;
		Set<Edge*> _labels;
		bool _updated;

		io::Output& print(io::Output& out) const;
	};

	Option<LabelledValue>* tempvars;
	Option<LabelledValue>* registers;
	unsigned int _max_tempvars;
	unsigned int _max_registers;

	Option<LabelledValue>& getCell(const OperandVar& opdv) const;
	io::Output& print(io::Output& out) const;

public:
	ConstantVariables(unsigned int max_tempvars, unsigned int max_registers);
	ConstantVariables(const ConstantVariables& cv);
	~ConstantVariables();
	void clear();
	inline unsigned int maxTempVars() const { return _max_tempvars; }
	inline unsigned int maxRegisters() const { return _max_registers; }
	bool isConstant(const OperandVar& opdv) const;
	Constant getValue(const OperandVar& opdv) const; // this must not be called if !isConstant(opdv)...
		   void set(const OperandVar& opdv, const Constant& val, bool updated_flag = true);
	inline void set(const OperandVar& opdv, const OperandConst& opdc, bool updated_flag = true) { set(opdv, opdc.value(), updated_flag); }
		   void update(const OperandVar& opdv, const Constant& val, bool updated_flag = true);
	inline void update(const OperandVar& opdv, const OperandConst& opdc, bool updated_flag = true) { update(opdv, opdc.value(), updated_flag); }
	void invalidate(const OperandVar& opdv);
	bool invalidateTempVars();
	void label(Edge* label);
	void merge(const SLList<ConstantVariables>& cvl);
	SLList<LabelledPredicate> toPredicates() const;
	ConstantVariablesSimplified toSimplified() const;
	inline Constant operator[](const OperandVar& opdv) const { return getValue(opdv); }
	ConstantVariables& operator=(const ConstantVariables& cv);
	bool operator==(const ConstantVariables& cv) const; // strict
	bool sameValuesAs(const ConstantVariables& cv) const; // less strict (only values)
	inline bool operator!=(const ConstantVariables& cv) const { return !(*this == cv); }
	friend io::Output& operator<<(io::Output& out, const ConstantVariables& cv) { return cv.print(out); }
};

#endif
