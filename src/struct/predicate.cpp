/*
 *	
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2006-2018, IRIT UPS.
 * 
 *	OTAWA is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	OTAWA is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OTAWA; if not, write to the Free Software 
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
/*
 * Defines our predicates
 */

#include <elm/io/Output.h>
#include "predicate.h"
#include "../debug.h"

/**
 * @class Predicate
 * @author Jordy Ruiz 
 * @brief A predicate of the abstract analysis
 */
Predicate::Predicate() :_opr(0), _opd1(NULL), _opd2(NULL) { }
Predicate::Predicate(condoperator_t opr, const Operand* opd1, const Operand* opd2) : _opr(opr), _opd1(opd1), _opd2(opd2) { }

/**
 * @fn inline bool Predicate::isIdent(void);
 * Test if the predicate is identity (X = X).
 */
/**
 * @fn inline bool Predicate::isComplete(void);
 * Test that the predicate is representable in arithmetic and isn't some intermediary result generated by
 * the program analysis that does not make sense in a solver (for example X = Y ~ Z).
 */

/**
 * @brief      Checks that the predicate involves an operand, and gives the count
 * @param      opd   The operand
 * @return     The amount of matching operands founds
 */
int Predicate::involvesOperand(const Operand& opd) const
{
	return _opd1->involvesOperand(opd) + _opd2->involvesOperand(opd);
}

/**
 * @brief Test if the predicate contains a variable
 * @details Count (recursively) the occurrences of the variable operand opdv in the predicate.
 * 
 * @param opdv Variable to look for.
 * @return Number of occurrences found.
 */
int Predicate::involvesVariable(const OperandVar& opdv) const
{
	return _opd1->involvesVariable(opdv) + _opd2->involvesVariable(opdv);
}

/**
 * @brief      Checks if a predicate involves memory cells in stack that are below a defined value
 * @param      stack_limit  The stack thresold
 * @return     I think this returns the address of a memory cell that does not match
 */
Option<Constant> Predicate::involvesStackBelow(const Constant& stack_limit) const
{
	if(const Option<Constant>& opd1_rtn = _opd1->involvesStackBelow(stack_limit))
		return opd1_rtn;
	else return _opd2->involvesStackBelow(stack_limit);
}

/**
 * @brief      Checks if the Predicate involves the given memory cell
 * @param      opdm  The memory cell
 * @return     True if the memory cell opdm was found, False otherwise
 */
bool Predicate::involvesMemoryCell(const OperandMem& opdm) const
{
	return _opd1->involvesMemoryCell(opdm) || _opd2->involvesMemoryCell(opdm);
}

/**
 * @brief      Checks if this involves any memory at all
 * @return     I think it returns NULL if it found no memory, and the OperandMem if it found it
 */
const Operand* Predicate::involvesMemory() const
{
	if(const Operand* rtn = _opd1->involvesMemory())
		return rtn;
	else
		return _opd2->involvesMemory();
}

/**
 * @fn inline bool Predicate::isAffine(const OperandVar& opdv, const OperandVar& sp);
 * Test if the predicate only contains + or - operations, only occurrences of opdv, and no memory cells.
 * This does not check that there is only one occurrence of opdv.
 * @param opdv Variable with respect to which the Predicate is relative to.
 */
bool Predicate::isAffine(const OperandVar& opdv) const
{
	return _opr == CONDOPR_EQ && _opd1->isAffine(opdv) && _opd2->isAffine(opdv);
}

/**
 * @brief Count temporary variables.
 * @return Variable count.
 */
unsigned int Predicate::countTempVars() const
{
	return _opd1->countTempVars() + _opd2-> countTempVars();
}

 // we are looking for a t1 = X or X = t1 pattern, with X being ground.
bool Predicate::getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const
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
		expr = _opd1;
	return true;
}

/**
 * @fn bool Predicate::update(DAG& dag, const Operand* opd, const Operand* opd_modifier)
 * @brief      Updates the predicate by replacing all opd with opd_modifier
 *
 * @param      dag           The dag
 * @param      opd           The operand to be replaced
 * @param      opd_modifier  The operand to replace with
 *
 * @return     True if something was updated, False otherwise
 */
bool Predicate::update(DAG& dag, const Operand* opd, const Operand* opd_modifier)
{
	// we need to replace the matching children as they can't do it themselves since the parent has to do the modification
	bool rtn = false;
	if(_opd1 == opd)
	{
		_opd1 = opd_modifier;
		rtn = true;
	}
	else if(Option<const Operand*> new_opd1 = _opd1->update(dag, opd, opd_modifier))
	{
		_opd1 = *new_opd1;
		rtn = true;
	}
	if(_opd2 == opd)
	{
		_opd2 = opd_modifier;
		rtn = true;
	}
	else if(Option<const Operand*> new_opd2 = _opd2->update(dag, opd, opd_modifier))
	{
		_opd2 = *new_opd2;
		rtn = true;
	}
	return rtn;
}

/**
 * @fn bool Predicate::isTautology(void) const
 * @brief      Determines if the predicate is a tautology.
 */
bool Predicate::isTautology(void) const
{
	if(_opr == CONDOPR_EQ)
		return _opd1 == _opd2;
	if(_opr == CONDOPR_LE && _opd1 == _opd2)
		return true;
	else if(_opd1->isAConst() && _opd2->isAConst() && _opd1->toConstant().isAbsolute() && _opd2->toConstant().isAbsolute())
	{
		t::int32 v = _opd1->toConstant().val(), w = _opd2->toConstant().val();
		switch(_opr)
		{
			case CONDOPR_LT:
				return v < w;
			case CONDOPR_LE:
				return v <= w;
			case CONDOPR_NE:
				return v != w;
			default:
				break;
		}
	}
	return false;
}

/**
  * @fn bool Predicate::operator==(const Predicate& p) const;
  * Not a Leibniz equality: (x == y) is considered to be the same as (y == x)
  */
bool Predicate::operator==(const Predicate& p) const
{
	return (_opr == p._opr) &&
		(
			(*_opd1 == *p._opd1 && *_opd2 == *p._opd2) || // operands match respectively
			((_opr == CONDOPR_EQ || _opr == CONDOPR_NE) && (*_opd1 == *p._opd2 && *_opd2 == *p._opd1)) // operator is symetric, and first operands match second operands
		);
}

io::Output& operator<<(io::Output& out, const condoperator_t& opr)
{	
	switch(opr)
	{
		case CONDOPR_LT:
			return out << "<";
		case CONDOPR_EQ:
			return out << "=";
#ifndef NO_UTF8
		case CONDOPR_LE:
			return out << "≤";
		case CONDOPR_NE:
			return out << "≠";
#else
		case CONDOPR_LE:
			return out << "<=";
		case CONDOPR_NE:
			return out << "!=";
#endif
		default:
			crash();
	}
}

io::Output& operator<<(io::Output& out, const Predicate& p)
{
	// Only binary conditional operators in our implementation
	return (out << *(p._opd1) << " " << (condoperator_t)p._opr << " " << *(p._opd2));
}
