#include <elm/io/Output.h>
#include "predicate.h"
#include "debug.h"

using namespace elm;

Predicate::Predicate(condoperator_t opr, Operand& opd1, Operand& opd2) : _opr(opr)
{
	_opd1 = opd1.copy();
	_opd2 = opd2.copy();
}

// Test if the predicate contains (recursively) the variable represented by opdv
bool Predicate::isInvolvedVariable(const OperandVar& opdv) const
{
	return _opd1->isInvolvedVariable(opdv) || _opd2->isInvolvedVariable(opdv);
}

// returns true if something was updated
bool Predicate::updateVar(const OperandVar& opdv, const Operand& opd_modifier)
{	
	// we need to replace the OperandVar children
	// (they can't do it on their own since the parent has to do the modification)
	bool rtn = false;
	if(*_opd1 == opdv)
	{
		_opd1 = opd_modifier.copy();
		DBG(COLOR_IYel << "true flag here!")
		rtn = true;
	}
		
	if(*_opd2 == opdv)
	{
		_opd2 = opd_modifier.copy();
		DBG(COLOR_IYel << "true flag here!")
		rtn = true;
	}
	
	rtn |= _opd1->updateVar(opdv, opd_modifier);
	rtn |= _opd2->updateVar(opdv, opd_modifier);
	
	return rtn;
}

bool Predicate::operator==(const Predicate& p) const
{
	return (_opr == p._opr) && (*_opd1 == *(p._opd1)) && (*_opd2 == *(p._opd2));
}

io::Output& operator<<(io::Output& out, const condoperator_t& opr)
{	
	switch(opr) {
		case CONDOPR_LT:
			out << "<";
			break;
		case CONDOPR_LE:
			out << "<=";
			break;
		case CONDOPR_EQ:
			out << "=";
			break;
		case CONDOPR_NE:
			out << "!=";
			break;
		default:
			out << "???";
	}
	return out;
}

io::Output& operator<<(io::Output& out, const Predicate& p)
{
	// Only binary conditional operators in our implementation
	out << "(" << *(p._opd1) << " " << p._opr << " " << *(p._opd2) << ")"; // (... * ...)
	return out;
}
