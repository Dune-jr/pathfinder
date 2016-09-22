#include "constant_variables.h"
#include "debug.h"

/**
 * @class ConstantVariables
 * @brief Contains accessible information on constant variables of the abstract interpretation analysis
 */

// TODO! return *
Set<Edge*> ConstantVariables::getLabels(const OperandVar& opdv) const
{
	ASSERT(getCell(opdv));
	return getCell(opdv).labels();
}

Set<Edge*> ConstantVariables::getLabels(const OperandVar& opdv1, const OperandVar& opdv2) const
{
	LabelledValue& k1 = getCell(opdv1), k2 = getCell(opdv2);
	ASSERT(k1 && k2);
	Set<Edge*> labels = k1.labels();
	labels.addAll(k2.labels());
	return labels;
}

ConstantVariables::LabelledValue ConstantVariables::getLabelledValue(const OperandVar& opdv) const
{
	ASSERT(getCell(opdv));
	return getCell(opdv);
}

// reset and set the value (the label list is emptied)
void ConstantVariables::set(const OperandVar& opdv, const LabelledValue& lval)
{
	invalidate(opdv);
	if(lval.val().isValid())
	{
		DBG(color::IPur() << DBG_SEPARATOR << color::IGre() << " + " << opdv << "==" << OperandConst(lval.val()))
		getCell(opdv) = lval;
	}
}

// reset and set the value (the label list is emptied)
void ConstantVariables::set(const OperandVar& opdv, const Constant& val, const Set<Edge*>& labels, bool updated_flag)
{
	invalidate(opdv);
	if(val.isValid())
	{
		DBG(color::IPur() << DBG_SEPARATOR << color::IGre() << " + " << opdv << "==" << OperandConst(val))
		getCell(opdv) = LabelledValue(val, labels, updated_flag);
	}
}

// set to a value without changing the labels
void ConstantVariables::update(const OperandVar& opdv, const Constant& val, bool updated_flag)
{
	LabelledValue& k = getCell(opdv);
	if(k)
	{
		DBG(color::IPur() << DBG_SEPARATOR << color::Cya() << " - " << opdv << "==" << OperandConst(k.val()))
		DBG(color::IPur() << DBG_SEPARATOR << color::ICya() << " + " << opdv << "==" << OperandConst(val))
	}
	else
		DBG(color::IPur() << DBG_SEPARATOR << color::IGre() << " + " << opdv << "==" << OperandConst(val))
	k.setVal(val);
	k.setUpdatedFlag(updated_flag);
	// keep labels
}

void ConstantVariables::invalidate(const OperandVar& opdv)
{
	LabelledValue& k = getCell(opdv);
	if(k)
	{
		DBG(color::IPur() << DBG_SEPARATOR << color::IYel() << " - " << opdv << "==" << OperandConst(k.val()))
		k.clear(); // remove k
	}
}

// this function will keep the information by replacing the tempvars by their value (isn't that done already???)
bool ConstantVariables::invalidateTempVars()
{
	int changes = 0;
	for(short i = 0; i < _max_tempvars; i++)
	{
		if(tempvars[i])
		{
			tempvars[i].clear();
			changes++;
		}
	}
	if(!changes)
		return false;
	DBG(color::IYel() << "- " << changes << (changes > 1 ? " tempvar predicates" : " tempvar predicate"))
	// DBG(color::IRed() << "State: " << *this)
	return true;
}

/**
 * @fn void ConstantVariables::merge(const SLList<ConstantVariables>& cvl);
 * This uses the current ConstantVariables and changes it to the result of the merge of it with the cvl list parameter
 */
// TODO: we can improve this a lot
void ConstantVariables::merge(const SLList<ConstantVariables>& cvl)
{
	for(SLList<ConstantVariables>::Iterator iter(cvl); iter; iter++)
	{
		const ConstantVariables& cv = *iter;
		ASSERTP(_max_tempvars == cv._max_tempvars && _max_registers == cv._max_registers, "ConstantVariables::merge: format does not match")
		for(short i = 0; i < _max_tempvars; i++)
			if(tempvars[i] && (!cv.tempvars[i] || tempvars[i].val() != (cv.tempvars[i]).val())) // do not compare labels!
				tempvars[i].clear();
		for(short i = 0; i < _max_registers; i++)
			if(registers[i] && (!cv.registers[i] || registers[i].val() != (cv.registers[i]).val())) // do not compare labels!
				registers[i].clear();
	}
	// manage labels
	for(short i = 0; i < _max_tempvars; i++)
		if(tempvars[i])
			tempvars[i] = LabelledValue(tempvars[i].val(), Set<Edge*>::null, true); // it is important to mark them as updated so they can be labelled
	for(short i = 0; i < _max_registers; i++)
		if(registers[i])
			registers[i] = LabelledValue(registers[i].val(), Set<Edge*>::null, true);
}

SLList<LabelledPredicate> ConstantVariables::toPredicates() const
{
	SLList<LabelledPredicate> l;
	// all tempvars should be gone when toPredicates() is used, but we still handle them in case the method is used during the BB analysis
	for(int i = _max_tempvars-1; i >= 0; i--)
	{
		if(tempvars[i])
		{
			OperandVar opd_left(-i-1);
			OperandConst opd_right((tempvars[i]).val());
			l += LabelledPredicate(Predicate(CONDOPR_EQ, opd_left, opd_right), tempvars[i].labels());
		}
	}
	for(int i = _max_registers-1; i >= 0 ; i--)
	{
		if(registers[i])
		{
			OperandVar opd_left(i);
			OperandConst opd_right((registers[i]).val());
			l += LabelledPredicate(Predicate(CONDOPR_EQ, opd_left, opd_right), registers[i].labels());
		}
	}
	return l;
}
