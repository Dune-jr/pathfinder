#include <elm/io/Output.h>
#include "predicate.h"
#include "debug.h"

using namespace elm;

Predicate::Predicate(condoperator_t opr, Operand& opd1, Operand& opd2) : _opr(opr)
{
	_opd1 = opd1.copy();
	_opd2 = opd2.copy();
}

/**
 * @fn inline bool Predicate::isIdent(void);
 * Test if the predicate is identity (X = X)
 */

// Test if the predicate contains (recursively) the variable represented by opdv
bool Predicate::involvesVariable(const OperandVar& opdv) const
{
	return _opd1->involvesVariable(opdv) || _opd2->involvesVariable(opdv);
}

unsigned int Predicate::countTempVars() const
{
	return _opd1->countTempVars() + _opd2-> countTempVars();
}

 // we are looking for a t1 = X or X = t1 pattern, with X being ground
bool Predicate::getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const
{
	if(_opr != CONDOPR_EQ) // not an equality, abort
		return false;
	if(countTempVars() != 1) // no temp vars, several temp vars, or several occurrences of the same temp var
		return false; // this could use an improvement
		
	bool rtn1 = _opd1->getIsolatedTempVar(temp_var, expr);
	bool rtn2 = _opd2->getIsolatedTempVar(temp_var, expr);
	
	if(!rtn1 && !rtn2)
		return false;
	
	if(rtn1 && rtn2) // both are isolated tempvars, therefore temp_var <- _opd2 and expr <- [???]
		expr = _opd1->copy();
	
	return true;
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
		rtn = true;
	}
	else
		rtn |= _opd1->updateVar(opdv, opd_modifier);		
		
	if(*_opd2 == opdv)
	{
		_opd2 = opd_modifier.copy();
		rtn = true;
	}
	else
		rtn |= _opd2->updateVar(opdv, opd_modifier);
	
	return rtn;
}

bool Predicate::operator==(const Predicate& p) const
{
	return (_opr == p._opr) && (*_opd1 == *p._opd1) && (*_opd2 == *p._opd2);
}

io::Output& operator<<(io::Output& out, const condoperator_t& opr)
{	
	switch(opr) {
		case CONDOPR_LT:
			out << "<";
			break;
		case CONDOPR_LE:
#			ifndef NO_UTF8
				out << "≤";
#			else
				out << "<=";
#			endif
			break;
		case CONDOPR_EQ:
			out << "=";
			break;
		case CONDOPR_NE:
#			ifndef NO_UTF8
				out << "≠";
#			else
				out << "!=";
#			endif
			break;
		default:
			out << "???";
	}
	return out;
}

io::Output& operator<<(io::Output& out, const Predicate& p)
{
	// Only binary conditional operators in our implementation
	return (out << *(p._opd1) << " " << p._opr << " " << *(p._opd2));
}
