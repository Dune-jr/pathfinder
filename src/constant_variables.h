#ifndef _CONSTANT_VARIABLES_H
#define _CONSTANT_VARIABLES_H

#include <otawa/cfg/Edge.h>
#include <elm/genstruct/SLList.h>
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
		LabelledValue(t::int32 val, SLList<const Edge*> labels, bool updated = false) : _val(val), _labels(labels), _updated(updated) { }
		inline t::int32 val() const { return _val; }
		inline const SLList<const Edge*>& labels() const { return _labels; }
		inline bool isUpdated() const { return _updated; }
		inline void addLabel(const Edge* label) { if(!_labels.contains(label)) _labels += label; }
		inline void setUpdatedFlag(bool updated = true) { _updated = updated; }
		LabelledValue& operator=(const LabelledValue& lv);
		friend io::Output& operator<<(io::Output& out, const LabelledValue& lv) { return lv.print(out); }

	private:
		t::int32 _val;
		SLList<const Edge*> _labels;
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
	inline unsigned int maxTempVars() const { return _max_tempvars; }
	inline unsigned int maxRegisters() const { return _max_registers; }
		   bool isConstant(const OperandVar& opdv) const;
	inline bool isConstant(t::int32 var_id) const { return isConstant(OperandVar(var_id)); }
		   t::int32 getValue(const OperandVar& opdv) const; // this must not be called if !isConstant(opdv)...
	inline t::int32 getValue(t::int32 var_id) const { return getValue(OperandVar(var_id)); }
		   void set(const OperandVar& opdv, t::int32 val, bool updated_flag = true);
	inline void set(const OperandVar& opdv, OperandConst opdc, bool updated_flag = true) { set(opdv, opdc.value(), updated_flag); }
	inline void set(const t::int32 var_id, t::int32 val, bool updated_flag = true) { set(OperandVar(var_id), val, updated_flag); }
	inline void set(const t::int32 var_id, OperandConst opdc, bool updated_flag = true) { set(OperandVar(var_id), opdc.value(), updated_flag); }
		   void update(const OperandVar& opdv, t::int32 val, bool updated_flag = true);
	inline void update(const OperandVar& opdv, OperandConst opdc, bool updated_flag = true) { update(opdv, opdc.value(), updated_flag); }
	inline void update(const t::int32 var_id, t::int32 val, bool updated_flag = true) { update(OperandVar(var_id), val, updated_flag); }
	inline void update(const t::int32 var_id, OperandConst opdc, bool updated_flag = true) { update(OperandVar(var_id), opdc.value(), updated_flag); }
		   void invalidate(const OperandVar& opdv);
	inline void invalidate(t::int32 var_id) { invalidate(OperandVar(var_id)); }
	bool invalidateTempVars();
	void label(const Edge* label);
	SLList<LabelledPredicate> toPredicates() const;
	ConstantVariablesSimplified toSimplified() const;
	inline t::int32 operator[](const OperandVar& opdv) const { return getValue(opdv); }
	inline t::int32 operator[](t::int32 var_id) const { return getValue(var_id); }
	ConstantVariables& operator=(const ConstantVariables& cv);
	friend io::Output& operator<<(io::Output& out, const ConstantVariables& cv) { return cv.print(out); }
};

#endif