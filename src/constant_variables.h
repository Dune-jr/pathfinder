#ifndef _CONSTANT_VARIABLES_H
#define _CONSTANT_VARIABLES_H

#include <elm/avl/Set.h>
#include <elm/genstruct/SLList.h>
#include <otawa/cfg/Edge.h>
#include <otawa/sem/inst.h>
#include "constant_variables_simplified.h"
#include "labelled_predicate.h"
#include "debug.h"

using namespace otawa;
using namespace elm::genstruct; 
using namespace elm::avl;

class ConstantVariables {
public:
	class LabelledValue
	{
	public:
		LabelledValue() : _exists(false), _labels(NULL) { }
		LabelledValue(const LabelledValue& lv) : _val(lv._val), _updated(lv._updated), _exists(lv._exists),
			_labels(lv._labels ? new Set<Edge*>(*lv._labels) : NULL) { }
		LabelledValue(const Constant& val, const Set<Edge*>& labels, bool updated = true)
			: _val(val), _updated(updated), _exists(true), _labels(new Set<Edge*>(labels)) { }
		~LabelledValue() { delete _labels; }
		inline Constant val() const { return _val; }
		inline const Set<Edge*>& labels() const { return *_labels; }
		inline bool isUpdated() const { return _updated; }
		inline bool exists() const { return _exists; }
		inline void addLabel(Edge* label) { ASSERT(_labels); _labels->add(label); }
		// inline void clearLabels() { _labels.clear(); }
		inline void setUpdatedFlag(bool updated = true) { _updated = updated; }
		LabelledValue& operator=(const LabelledValue& lv);
		operator bool() const { return _exists; }
		bool operator==(const LabelledValue& lv) const;
		inline bool operator!=(const LabelledValue& lv) const { return !(lv == *this); }
		friend io::Output& operator<<(io::Output& out, const LabelledValue& lv) { return lv.print(out); }

	protected:
		Constant _val;
		bool _updated;
		bool _exists;
	private:
		Set<Edge*>* _labels;

		io::Output& print(io::Output& out) const;
	}; // LabelledValue class

	ConstantVariables(); // invalid
	ConstantVariables(unsigned int max_tempvars, unsigned int max_registers);
	ConstantVariables(const ConstantVariables& cv);
	~ConstantVariables();
	void clear();
	inline unsigned int maxTempVars() const { return _max_tempvars; }
	inline unsigned int maxRegisters() const { return _max_registers; }
	inline bool isValid() const { return _max_registers && _max_tempvars; }
	bool isConstant(const OperandVar& opdv) const;
	Constant getValue(const OperandVar& opdv) const; // this must not be called if !isConstant(opdv)...
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
	void label(Edge* label);
	void merge(const SLList<ConstantVariables>& cvl);
	elm::String printChanges() const;
	SLList<LabelledPredicate> toPredicates() const;
	ConstantVariablesSimplified toSimplified() const;
	inline Constant operator[](const OperandVar& opdv) const { return getValue(opdv); }
	ConstantVariables& operator=(const ConstantVariables& cv);
	bool operator==(const ConstantVariables& cv) const; // strict
	bool sameValuesAs(const ConstantVariables& cv) const; // less strict (only values)
	inline bool operator!=(const ConstantVariables& cv) const { return !(*this == cv); }
	friend io::Output& operator<<(io::Output& out, const ConstantVariables& cv) { return cv.print(out); }

private:
	Option<LabelledValue>* tempvars;
	Option<LabelledValue>* registers;
	unsigned int _max_tempvars;
	unsigned int _max_registers;

	Option<LabelledValue>& getCell(const OperandVar& opdv) const;
	io::Output& print(io::Output& out) const;
}; // ConstantVariables class


#endif
