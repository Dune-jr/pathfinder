#include "constant_variables.h"
#include "debug.h"

/**
 * @class ConstantVariables
 * @brief Contains accessible information on constant variables of the abstract interpretation analysis
 */
/**
 * @class ConstantVariables::LabelledValue
 * @brief Constant value labelled by a list of edges and an updated flag
 */
ConstantVariables::ConstantVariables() : tempvars(NULL), registers(NULL), _max_tempvars(0), _max_registers(0) { }

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

ConstantVariables::~ConstantVariables()
{
	delete[] tempvars;
	delete[] registers;
}

void ConstantVariables::clear()
{
	for(unsigned int i = 0; i < _max_tempvars; i++)
		tempvars[i] = none;
	for(unsigned int i = 0; i < _max_registers; i++)
		registers[i] = none;
}

ConstantVariables::LabelledValue& ConstantVariables::LabelledValue::operator=(const LabelledValue& lv)
{
	_val = lv._val;
	_labels = lv._labels;
	_updated = lv._updated;
	return *this;
}

bool ConstantVariables::LabelledValue::operator==(const LabelledValue& lv) const
{
	return _val == lv._val && _updated == lv._updated && _labels == lv._labels; // Set<> has functional operator== and operator!=
}

ConstantVariables& ConstantVariables::operator=(const ConstantVariables& cv)
{
	if(this->isValid())
	{
		// the two ConstantVariables must have the same size!
		ASSERT(_max_tempvars == cv.maxTempVars() && _max_registers == cv.maxRegisters());
		for(unsigned int i = 0; i < _max_tempvars; i++)
			tempvars[i] = cv.tempvars[i];
		for(unsigned int i = 0; i < _max_registers; i++)
			registers[i] = cv.registers[i];
	}
	else if(cv.isValid())
	{
		_max_tempvars = cv.maxTempVars();
		_max_registers = cv.maxRegisters();
		tempvars = new Option<LabelledValue>[cv.maxTempVars()];
		registers = new Option<LabelledValue>[cv.maxRegisters()];
		for(unsigned int i = 0; i < _max_tempvars; i++) // TODOv2: do memcpy here
			tempvars[i] = cv.tempvars[i];
		for(unsigned int i = 0; i < _max_registers; i++)
			registers[i] = cv.registers[i];
	}
	return *this;
}

bool ConstantVariables::operator==(const ConstantVariables& cv) const
{
	if(_max_tempvars != cv._max_tempvars || _max_registers != cv._max_registers)
		return false; // sizes do not match
	for(unsigned int i = 0; i < _max_tempvars; i++)
		if(tempvars[i] != cv.tempvars[i])
			return false;
	for(unsigned int i = 0; i < _max_registers; i++)
		if(registers[i] != cv.registers[i])
			return false;
	return true;
}

bool ConstantVariables::sameValuesAs(const ConstantVariables& cv) const
{
	if(_max_tempvars != cv._max_tempvars || _max_registers != cv._max_registers)
		return false; // sizes do not match
	for(unsigned int i = 0; i < _max_tempvars; i++)
	{
		if(tempvars[i].isOne() != cv.tempvars[i].isOne())
			return false;
		if(tempvars[i].isOne() && (*tempvars[i]).val() != (*cv.tempvars[i]).val())
			return false;
	}
	for(unsigned int i = 0; i < _max_registers; i++)
	{
		if(registers[i].isOne() != cv.registers[i].isOne())
			return false;
		if(registers[i].isOne() && (*registers[i]).val() != (*cv.registers[i]).val())
			return false;
	}
	return true;
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

Constant ConstantVariables::getValue(const OperandVar& opdv) const
{
	Option<LabelledValue>& k = getCell(opdv);
	ASSERT(k.isOne());
	return k.value().val();
}

Set<Edge*> ConstantVariables::getLabels(const OperandVar& opdv) const
{
	Option<LabelledValue>& k = getCell(opdv);
	ASSERT(k.isOne());
	return k.value().labels();
}

Set<Edge*> ConstantVariables::getLabels(const OperandVar& opdv1, const OperandVar& opdv2) const
{
	Option<LabelledValue>& k1 = getCell(opdv1), k2 = getCell(opdv2);
	ASSERT(k1.isOne() && k2.isOne());
	Set<Edge*> labels = k1.value().labels();
	labels.addAll(k2.value().labels());
	return labels;
}

ConstantVariables::LabelledValue ConstantVariables::getLabelledValue(const OperandVar& opdv) const
{
	Option<LabelledValue>& k = getCell(opdv);
	ASSERT(k.isOne());
	return k.value();
}

// reset and set the value (the label list is emptied)
void ConstantVariables::set(const OperandVar& opdv, const LabelledValue& lval)
{
	if(!lval.val().isValid())
		return invalidate(opdv);
	Option<LabelledValue>& k = getCell(opdv);
	invalidate(opdv);
	DBG(color::IPur() << DBG_SEPARATOR << color::IGre() << " + " << opdv << "==" << OperandConst(lval.val()))
	k = some(lval);
}

// reset and set the value (the label list is emptied)
void ConstantVariables::set(const OperandVar& opdv, const Constant& val, const Set<Edge*>& labels, bool updated_flag)
{
	if(!val.isValid())
		return invalidate(opdv);
	Option<LabelledValue>& k = getCell(opdv);
	invalidate(opdv);
	DBG(color::IPur() << DBG_SEPARATOR << color::IGre() << " + " << opdv << "==" << OperandConst(val))
	k = some(LabelledValue(val, labels, updated_flag));
}

// set to a value without changing the labels
void ConstantVariables::update(const OperandVar& opdv, const Constant& val, bool updated_flag)
{
	Option<LabelledValue>& k = getCell(opdv);
	if(k.isOne())
	{
		DBG(color::IPur() << DBG_SEPARATOR << color::Cya() << " - " << opdv << "==" << OperandConst(k.value().val()))
		DBG(color::IPur() << DBG_SEPARATOR << color::ICya() << " + " << opdv << "==" << OperandConst(val))
	}
	else
		DBG(color::IPur() << DBG_SEPARATOR << color::IGre() << " + " << opdv << "==" << OperandConst(val))
	k = some(LabelledValue(val, (*k).labels(), updated_flag));
}

void ConstantVariables::invalidate(const OperandVar& opdv)
{
	Option<LabelledValue>& k = getCell(opdv);
	if(k.isNone())
		return; // nothing to do
	DBG(color::IPur() << DBG_SEPARATOR << color::IYel() << " - " << opdv << "==" << OperandConst(k.value().val()))
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
	DBG(color::IYel() << "- " << changes << (changes > 1 ? " tempvar predicates" : " tempvar predicates"))
	// DBG(color::IRed() << "State: " << *this)
	return true;
}

void ConstantVariables::label(Edge* label)
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

/**
 * @fn void ConstantVariables::merge(const SLList<ConstantVariables>& cvl);
 * This uses the current ConstantVariables and change it to the result of the merge of it with the cvl list parameter
 */
// TODO: we can improve this a lot
void ConstantVariables::merge(const SLList<ConstantVariables>& cvl)
{
	for(SLList<ConstantVariables>::Iterator iter(cvl); iter; iter++)
	{
		const ConstantVariables& cv = *iter;
		ASSERTP(_max_tempvars == cv._max_tempvars && _max_registers == cv._max_registers, "ConstantVariables::merge: format does not match")
		for(unsigned int i = 0; i < _max_tempvars; i++)
			if(tempvars[i] && (!(cv.tempvars[i]) || (*tempvars[i]).val() != (*cv.tempvars[i]).val())) // do not compare labels!
				tempvars[i] = none;
		for(unsigned int i = 0; i < _max_registers; i++)
			if(registers[i] && (!(cv.registers[i]) || (*registers[i]).val() != (*cv.registers[i]).val())) // do not compare labels!
				registers[i] = none;
	}
	// manage labels
	for(unsigned int i = 0; i < _max_tempvars; i++)
		if(tempvars[i])
			tempvars[i] = some(LabelledValue((*tempvars[i]).val(), Set<Edge*>::null, true)); // it is important to mark them as updated so they can be labelled
	for(unsigned int i = 0; i < _max_registers; i++)
		if(registers[i])
			registers[i] = some(LabelledValue((*registers[i]).val(), Set<Edge*>::null, true));
}

// returns a one-line string with pretty printing of changes that occured in the constants during the current block (updated flag)
elm::String ConstantVariables::printChanges() const
{
	elm::String rtn = "[";
	bool first = true;
	/*for(unsigned int i = 0; i < _max_tempvars; i++)
	{
		if(tempvars[i] && (*tempvars[i]).isUpdated())
		{
			if(!first)
				rtn = rtn.concat(elm::String(", "));
			else first = false;
			rtn = rtn.concat(_ << "t" << i+1 << "=" << (*tempvars[i]).val());
		}
	}*/
	for(unsigned int i = 0; i < _max_registers; i++)
	{

		if(registers[i] && (*registers[i]).isUpdated()) 
		{
			if(!first)
				rtn = rtn.concat(elm::String(", "));
			else first = false;
			rtn = rtn.concat(_ << "?" << i << "=" << (*registers[i]).val());
		}
	}
	return rtn.concat(elm::String("]"));
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
		for(Set<Edge*>::Iterator iter(_labels); iter; iter++)
		{
			if(first_time)
				first_time = false;
			else
				out << ", ";
#ifdef v1
			out << (*iter)->source()->number() << "->" << (*iter)->target()->number();
#else
			out << (*iter)->source()->index() << "->" << (*iter)->target()->index(); // v2
#endif
		}
		out << ")";
	}
	if(_updated)
		out << " [u]";
	return out;
}
