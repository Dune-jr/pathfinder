#include "constant_variables.h"

// ConstantVariables
ConstantVariables::ConstantVariables(unsigned int max_tempvars, unsigned int max_registers)
	: _max_tempvars(max_tempvars), _max_registers(max_registers)
{
	tempvars = new Option<LabelledValue>[max_tempvars];
	registers = new Option<LabelledValue>[max_registers];
}

ConstantVariables::ConstantVariables(const ConstantVariables& cv)
	: _max_tempvars(cv.maxTempVars()), _max_registers(cv.maxRegisters())
{
	tempvars = new Option<LabelledValue>[cv.maxTempVars()];
	registers = new Option<LabelledValue>[cv.maxRegisters()];
	*this = cv;
}

ConstantVariables::LabelledValue& ConstantVariables::LabelledValue::operator=(const LabelledValue& lv)
{
	_val = lv._val;
	_labels = lv._labels;
	_updated = lv._updated;
	return *this;
}

ConstantVariables& ConstantVariables::operator=(const ConstantVariables& cv)
{
	// the two ConstantVariables must have the same size!
	assert(_max_tempvars == cv.maxTempVars());
	assert(_max_registers == cv.maxRegisters());
	for(unsigned int i = 0; i < _max_tempvars; i++)
		tempvars[i] = cv.tempvars[i];
	for(unsigned int i = 0; i < _max_registers; i++)
		registers[i] = cv.registers[i];
	return *this;
}

Option<ConstantVariables::LabelledValue>& ConstantVariables::getCell(const OperandVar& opdv) const
{
	if(opdv.isTempVar())
		return tempvars[-1-opdv.addr()]; // tempvars id start at 1 and are negative
	return registers[opdv.addr()]; // registers ids start at 0 and are positive
}

bool ConstantVariables::isConstant(const OperandVar& opdv) const
{
	return getCell(opdv).isOne();
}

t::int32 ConstantVariables::getValue(const OperandVar& opdv) const
{
	Option<LabelledValue>& k = getCell(opdv);
	assert(k.isOne());
	return k.value().val();
}

// reset and set the value (the label list is emptied)
void ConstantVariables::set(const OperandVar& opdv, t::int32 val, bool updated_flag)
{
	Option<LabelledValue>& k = getCell(opdv);
	invalidate(opdv);
	DBG(COLOR_IPur DBG_SEPARATOR COLOR_IGre " + " << opdv << "==" << OperandConst(val))
	k = some(LabelledValue(val, SLList<const Edge*>::null, updated_flag));
	// DBG(COLOR_IRed << "State: " << *this) // TODO remove
}

// set to a value without changing the labels
void ConstantVariables::update(const OperandVar& opdv, t::int32 val, bool updated_flag)
{
	Option<LabelledValue>& k = getCell(opdv);
	if(k.isOne())
	{
		DBG(COLOR_IPur DBG_SEPARATOR COLOR_Cya " - " << opdv << "==" << OperandConst(k.value().val()))
		DBG(COLOR_IPur DBG_SEPARATOR COLOR_ICya " + " << opdv << "==" << OperandConst(val))
	}
	else
		DBG(COLOR_IPur DBG_SEPARATOR COLOR_IGre " + " << opdv << "==" << OperandConst(val))
	k = some(LabelledValue(val, (*k).labels(), updated_flag));
}

void ConstantVariables::invalidate(const OperandVar& opdv)
{
	Option<LabelledValue>& k = getCell(opdv);
	if(k.isNone())
		return; // nothing to do
	DBG(COLOR_IPur DBG_SEPARATOR COLOR_IYel " - " << opdv << "==" << OperandConst(k.value().val()))
	k = none;
}

// this function will keep the information by replacing the tempvars by their value (isn't that done already???)
bool ConstantVariables::invalidateTempVars()
{
	int changes = 0;
	for(unsigned int i = 0; i < _max_tempvars; i++)
	{
		if(tempvars[i])
		{
			tempvars[i] = none;
			changes++;
		}
	}
	if(!changes)
		return false;
	DBG(COLOR_IYel "- " << changes << (changes > 1 ? " tempvars" : " tempvar"))
	// DBG(COLOR_IRed << "State: " << *this)
	return true;
}

void ConstantVariables::label(const Edge* label)
{
	for(unsigned int i = 0; i < _max_tempvars; i++)
		if(tempvars[i] && (*tempvars[i]).isUpdated())
		{
			LabelledValue lv(*tempvars[i]);
			lv.addLabel(label);
			lv.setUpdatedFlag(false);
			tempvars[i] = some(lv);
		}
	for(unsigned int i = 0; i < _max_registers; i++)
		if(registers[i] && (*registers[i]).isUpdated())
		{
			LabelledValue lv(*registers[i]);
			lv.addLabel(label);
			lv.setUpdatedFlag(false);
			registers[i] = some(lv);
		}
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
			OperandConst opd_right((*tempvars[i+1]).val());
			l += LabelledPredicate(Predicate(CONDOPR_EQ, opd_left, opd_right), (*tempvars[i+1]).labels());
		}
	}
	for(int i = _max_registers-1; i >= 0 ; i--)
	{
		if(registers[i])
		{
			OperandVar opd_left(i);
			OperandConst opd_right((*registers[i]).val());
			l += LabelledPredicate(Predicate(CONDOPR_EQ, opd_left, opd_right), (*registers[i]).labels());
		}
	}
	return l;
}

ConstantVariablesSimplified ConstantVariables::toSimplified() const
{
	ConstantVariablesSimplified cvs(_max_tempvars, _max_registers);
	for(unsigned int i = 0; i < _max_tempvars; i++)
	{
		if(tempvars[i])
			cvs.setTempVar(i, tempvars[i].value().val());
		else cvs.setTempVar(i, none);
	}
	for(unsigned int i = 0; i < _max_registers; i++)
	{
		if(registers[i])
			cvs.setRegister(i, registers[i].value().val());
		else cvs.setRegister(i, none);
	}
	return cvs;
}

io::Output& ConstantVariables::print(io::Output& out) const
{
	out << "[" << endl;
	for(unsigned int i = 0; i < _max_tempvars; i++)
	{
		if(tempvars[i])
			out << "\tt" << i+1 << " = " << *(tempvars[i]) << endl;
	}
	for(unsigned int i = 0; i < _max_registers; i++)
	{
		if(registers[i])
			out << "\t?" << i << " = " << *(registers[i]) << endl;
	}
	return (out << "]");
}

io::Output& ConstantVariables::LabelledValue::print(io::Output& out) const
{
	out << _val;
	if(_labels)
	{
		out << "(";
		bool first_time = true;
		for(SLList<const Edge*>::Iterator iter(_labels); iter; iter++)
		{
			if(first_time)
				first_time = false;
			else
				out << ", ";
			out << (*iter)->source()->number() << "->" << (*iter)->target()->number();
		}
		out << ")";
	}
	if(_updated)
		out << " [u]";
	return out;
}