/*
 *	Defines operands
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
 
#include <elm/io/Output.h>
#include "operand.h"
#include "DAG.h"
#include "../debug.h"

// using namespace elm;

/**
 * @class Operand
 * @author Jordy Ruiz 
 * @brief Abstract class for any operand of a Predicate
 */
/**
 * @class OperandConst
 * @author Jordy Ruiz 
 * @brief A constant operand, which can be relative to SP0
 */
/**
 * @class OperandVar
 * @author Jordy Ruiz 
 * @brief A variable in the program analysis, which can be a register or a temporary variable
 */
/**
 * @class OperandMem
 * @author Jordy Ruiz 
 * @brief A memory cell, identified by its address (absolute or relative to SP0)
 */
/**
 * @class OperandArith
 * @author Jordy Ruiz 
 * @brief An operand composed of (an)other operand(s) and a binary or unary operator
 */

/**
 * @fn unsigned int Operand::countTempVars() const;
 * @brief Count the number of temporary variables in the Operand
 * @warning This will count a variable several times if it occurs several times
 */
/**
 * @fn bool Operand::getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const;
 * @brief Check if the Operand is a single temporary variable (example "t3"). If true, copies itself in <b>temp_var</b>, if false, copies itself in <b>expr</b>
 * @return True if the Operand is a single temporary variable
 */
/**
 * @fn int Operand::involvesVariable(const OperandVar& opdv) const;
 * @brief Check for any reference to a variable in the Operand
 */
/**
 * @fn Option<Constant> Operand::involvesStackBelow(const Constant& stack_limit) const;
 * @brief Check for any reference (OperandMem) to the value in an address in the stack, below the given stack_limit
 * @return The reference in the stack, elm::none if none was found
 */
/**
 * @fn bool Operand::involvesMemoryCell(const OperandMem& opdm) const;
 * @brief Check for any reference to a given memory cell in the Operand
 * @param opdm Memory cell to look for in the operand
 */
/**
 * @fn const Operand* Operand::involvesMemory() const;
 * @brief Check for any reference to a value in the stack or the heap in the Operand.
 * @return NULL if nothing was found, or the first OperandMem found
 */
/**
 * @fn int Operand::count(void) const
 * @brief Counts the operands
 * @return The operand count as an integer
 */
/**
 * @fn Option<const Operand*> Operand::update(DAG& dag, const Operand* opd, const Operand* opd_modifier) const;
 * @brief replaces all opd by opd_modifier
 */
/**
 * @fn bool Operand::isComplete() const;
 * @brief Check whether the Operand is complete, as opposed to a temporary predicate (such as ?0 ~ 1)
 */
/**
 * @fn bool Operand::isAffine(const OperandVar& opdv) const;
 * @brief Check whether the Operand is affine regarding to opdv, that is, it does not contain any reference to the heap, the stack or another variable than opdv 
 */
/**
 * @fn void Operand::parseAffineEquation(AffineEquationState& state) const;
 * @brief Assuming this is an affine equation, describes it by filling the state given in parameter
 */
/**
 * @fn Option<Constant> Operand::evalConstantOperand() const;
 * @brief returns a constant if the operand could be identified to one, none otherwise
 * @warning Deprecated, bound to be removed in the future
 */
/**
 * @fn Option<const Operand*> Operand::simplify(DAG& dag) const;
 * @brief Simplify the operand, may return a new equivalent Operand to replace the current one with
 * @warning If elm::none is returned, it is not safe to assume that nothing has been simplified!
 */
/**
 * @fn const Operand* Operand::replaceConstants(const ConstantVariablesCore& constants, Vector<OperandVar>& replaced_vars);
 * @brief Replace variables that can be replaced by constants in this, using constants, and add the replaced vars to the list replaced_vars
 */
/**
 * @fn bool Operand::accept(OperandVisitor& visitor) const;
 * @brief Calls visit(*this) of visitor
 */
/**
 * @fn operand_kind_t Operand::kind() const;
 * @brief Returns an operand kind, in order to characterize each implementation of Operand
 */

OperandTop const* const Top = new OperandTop(-1);

Operand::Iter::Iter(const Operand* o, const int selector_flags) : q(), flags(selector_flags)
	{ q.push(o); check(); }

void Operand::Iter::next()
{
	if(!q)
		return;
	const Operand* cur = q.pop();
	if(cur->kind() == ARITH)
	{
		if(cur->toArith().isBinary())
			q.push(cur->toArith().right());
		q.push(cur->toArith().left());
	}
	// else no children, do nothing
	check();
}

/**
 * @fn 		   inline void Operand::Iter::check()
 * @brief      Check that the current item is OK, else do next
 */
inline void Operand::Iter::check()
{
	if(q && ! (flags & (1 << q.first()->kind())) ) // item is unwanted
		next();
}

// Operands: Constants
OperandConst::OperandConst(const OperandConst& opd) : _value(opd._value) { }
OperandConst::OperandConst(const Constant& value) : _value(value) { }
OperandConst::OperandConst() : _value(0) { }
OperandConst::~OperandConst() { }
// Operand* OperandConst::copy() const { return new OperandConst(_value); }
io::Output& OperandConst::print(io::Output& out) const { return out << _value; }
OperandConst& OperandConst::operator=(const OperandConst& opd) { _value = opd._value; return *this; }
bool OperandConst::operator<(const Operand& o) const
{
	if(kind() > o.kind())
		return true;
	if(kind() < o.kind())
		return false;
	return _value < o.toConst()._value;
}
bool OperandConst::getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const
{
	expr = this; // Assume we are the expr
	return false; // We haven't found an isolated tempvar
}
Option<Constant> OperandConst::involvesStackBelow(const Constant& stack_limit) const { return elm::none; }
Option<const Operand*> OperandConst::update(DAG& dag, const Operand* opd, const Operand* opd_modifier) const { return (this == opd) ? some(opd_modifier) : none; }
void OperandConst::parseAffineEquation(AffineEquationState& state) const
{
	ASSERT(_value.isValid());
	state.addToDelta(_value.val());
	if(_value.isRelative())
		state.onSpFound(_value.sign());
}
Option<Constant> OperandConst::evalConstantOperand() const
{
	if(!_value.isValid())
		return none;
	return some(_value);
}
Option<const Operand*> OperandConst::simplify(DAG& dag) const { return none; }
const Operand* OperandConst::replaceConstants(DAG& dag, const ConstantVariablesCore& constants, Vector<OperandVar>& replaced_vars) const
	{ return this; }

// Operands: Variables
OperandVar::OperandVar() : _addr(777) { }
OperandVar::OperandVar(const OperandVar& opd) : _addr(opd._addr) { }
OperandVar::OperandVar(t::int32 addr) : _addr(addr) { }
// OperandVar::~OperandVar() { }
// Operand* OperandVar::copy() const { return new OperandVar(_addr); }
io::Output& OperandVar::print(io::Output& out) const
{
	if(!isTempVar())
		out << "?" << _addr; // register
	else
		out << "t" << -_addr; // temporary
	return out; 
}
OperandVar& OperandVar::operator=(const OperandVar& opd){ _addr = opd._addr; return *this; }
bool OperandVar::operator<(const Operand& o) const
{
	if(kind() > o.kind())
		return true;
	if(kind() < o.kind())
		return false;
	return _addr < o.toVar()._addr;
}
bool OperandVar::operator==(const Operand& o) const
{
	return (o.kind() == kind()) && _addr == ((OperandVar&)o)._addr;
}
bool OperandVar::getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const
{
	if(isTempVar()) // temporary
	{
		temp_var = *this; // we found an isolated tempvar
		return true;
	}
	else // register
	{
		expr = this;
		return false;
	}
}
Option<Constant> OperandVar::involvesStackBelow(const Constant& stack_limit) const { return elm::none; }
const Operand* OperandVar::involvesMemory() const { return NULL; }
// since the parent has to do the modification, and var has no child, return false
Option<const Operand*> OperandVar::update(DAG& dag, const Operand* opd, const Operand* opd_modifier) const
	{ return (this == opd) ? some(opd_modifier) : none; }
void OperandVar::parseAffineEquation(AffineEquationState& state) const { state.onVarFound(*this); }
Option<Constant> OperandVar::evalConstantOperand() const { return none; }
Option<const Operand*> OperandVar::simplify(DAG& dag) const { return none; }
const Operand* OperandVar::replaceConstants(DAG& dag, const ConstantVariablesCore& constants, Vector<OperandVar>& replaced_vars) const
{
	if(constants.isConstant(_addr))
	{
		replaced_vars.push(*this);
		return dag.cst(constants[_addr]);
	}
	return this;
}

// Operands: Memory
OperandMem::OperandMem(const OperandConst& opdc) : _opdc(opdc) { }
OperandMem::OperandMem(const OperandMem& opd) : _opdc(opd.addr()) { }
OperandMem::OperandMem() { }
io::Output& OperandMem::print(io::Output& out) const
	{ return out << "[" << _opdc << "]"; }
OperandMem& OperandMem::operator=(const OperandMem& opd)
	{ _opdc = opd._opdc; return *this; }
bool OperandMem::operator<(const Operand& o) const
{
	if(kind() > o.kind())
		return true;
	if(kind() < o.kind())
		return false;
	return _opdc.value() < o.toMem()._opdc.value();
}
bool OperandMem::getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const
{
	expr = this; // Assume we are the expr
	return false; // We haven't found an isolated tempvar
}
int OperandMem::involvesVariable(const OperandVar& opdv) const
	{ return 0; }
bool OperandMem::involvesMemoryCell(const OperandMem& opdm) const
	{ return *this == opdm; }
const Operand* OperandMem::involvesMemory() const
	{ return this; }
Option<const Operand*> OperandMem::update(DAG& dag, const Operand* opd, const Operand* opd_modifier) const
	{ return (this == opd) ? some(opd_modifier) : none; }
void OperandMem::parseAffineEquation(AffineEquationState& state) const
	{ state.onVarFound(*this); }
Option<Constant> OperandMem::evalConstantOperand() const
	{ return none; }
Option<const Operand*> OperandMem::simplify(DAG& dag) const
	{ return none; }
const Operand* OperandMem::replaceConstants(DAG& dag, const ConstantVariablesCore& constants, Vector<OperandVar>& replaced_vars) const
	{ return this; }
Option<Constant> OperandMem::involvesStackBelow(const Constant& stack_limit) const 
{
	if(_opdc.value().isRelative() && (_opdc.value().val() < stack_limit.val()))
		return elm::some(_opdc.value());
	return elm::none;
}
 
// Operands: Top
// OperandTop::OperandTop() : id(next_id++) { }
OperandTop::OperandTop(int id) : id(id) { }
OperandTop::OperandTop(const OperandTop& opd) : id(opd.id) { }
OperandTop::OperandTop(const OperandTop& opd, int offset) : id(opd.id + offset) { }
io::Output& OperandTop::print(io::Output& out) const
{
	if(id == -1)
		return out << "T"; // unidentified Top
	else
		return out << "T" << id;
}
OperandTop& OperandTop::operator=(const OperandTop& opd)
{
	id = opd.id;
	return *this;
}
bool OperandTop::operator<(const Operand& o) const
{
	if(kind() > o.kind())
		return true;
	if(kind() < o.kind())
		return false;
	return id < o.toTop().id;
}
bool OperandTop::getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const
{
	expr = this; // Assume we are the expr
	return false; // We haven't found an isolated tempvar
}
Option<const Operand*> OperandTop::update(DAG& dag, const Operand* opd, const Operand* opd_modifier) const
	{ return (this == opd) ? some(opd_modifier) : none; }
Option<Constant> OperandTop::evalConstantOperand() const { return none; }
Option<const Operand*> OperandTop::simplify(DAG& dag) const { return none; }
const Operand* OperandTop::replaceConstants(DAG& dag, const ConstantVariablesCore& constants, Vector<OperandVar>& replaced_vars) const
	{ return this; }

// Operands: Induction variables
io::Output& OperandIter::print(io::Output& out) const
	{ return out << (done?'N':'I') << IntFormat((elm::t::intptr)lid).hex(); }

bool OperandIter::operator<(const Operand& o) const {
	if(kind() > o.kind())
		return true;
	if(kind() < o.kind())
		return false;
	if(done < static_cast<const OperandIter&>(o).done)
		return true;
	if(done > static_cast<const OperandIter&>(o).done)
		return false;
	return lid < static_cast<const OperandIter&>(o).lid;
}
 
// Operands: Arithmetic expressions
OperandArith::OperandArith(arithoperator_t opr, const Operand* opd1_, const Operand* opd2_)
	: _opr(opr), opd1(opd1_), opd2(opd2_) { }
io::Output& OperandArith::print(io::Output& out) const
{
	if(isUnary())
	{
		out << _opr;
		if(opd1->kind() == ARITH)
			out << "(" << *opd1 << ")";
		else
			out << *opd1;
	}
	else
	{
		if(opd1->kind() == ARITH)
			out << "(" << *opd1 << ")";
		else
			out << *opd1;
		out << " " << _opr << " ";
		if(opd2->kind() == ARITH)
			out << "(" << *opd2 << ")";
		else
			out << *opd2;
	}
	return out;
}
bool OperandArith::operator<(const Operand& o) const
{
	if(kind() > o.kind())
		return true;
	if(kind() < o.kind())
		return false;
	if(isBinary() < o.toArith().isBinary())
		return true;
	if(isBinary() > o.toArith().isBinary())
		return false;
	if(*opd1 < *(o.toArith().opd1))
		return true;
	// *opd1 >= *o.opd1
	if(isBinary() && *opd1 == *(o.toArith().opd1))
		return *opd2 < *(o.toArith().opd2);
	return false; // > or == but unary
}
bool OperandArith::operator==(const Operand& o) const
{
	OperandArith& o_arith = (OperandArith&)o; // Force conversion
	return (o.kind() == kind()) && (_opr == o_arith._opr) && (*opd1 == *(o_arith.opd1)) && (isUnary() || *opd2 == *(o_arith.opd2));
}
unsigned int OperandArith::countTempVars() const
{
	if(isUnary())
		return opd1->countTempVars();
	return opd1->countTempVars() + opd2->countTempVars();
}
bool OperandArith::getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const
{
	expr = this;
	return false;
}
int OperandArith::involvesVariable(const OperandVar& opdv) const
{
	if(isUnary())
		return opd1->involvesVariable(opdv);
	return opd1->involvesVariable(opdv) + opd2->involvesVariable(opdv);
}
Option<Constant> OperandArith::involvesStackBelow(const Constant& stack_limit) const
{
	if(isUnary())
		return opd1->involvesStackBelow(stack_limit);
	if(const Option<Constant>& opd1_rtn = opd1->involvesStackBelow(stack_limit))
		return opd1_rtn;
	else
		return opd2->involvesStackBelow(stack_limit);
}
bool OperandArith::involvesMemoryCell(const OperandMem& opdm) const
{
	if(isUnary())
		return opd1->involvesMemoryCell(opdm);
	return opd1->involvesMemoryCell(opdm) || opd2->involvesMemoryCell(opdm);
}
const Operand* OperandArith::involvesMemory() const
{
	if(isUnary())
		return opd1->involvesMemory();
	else if(const Operand* rtn = opd1->involvesMemory())
		return rtn;
	else
		return opd2->involvesMemory();
}
Option<const Operand*> OperandArith::update(DAG& dag, const Operand* opd, const Operand* opd_modifier) const
{
	bool rtn = false;
	const Operand *n1 = opd1, *n2 = opd2;
	if(opd1 == opd)
	{
		n1 = opd_modifier;
		rtn = true;
	}
	else if(Option<const Operand*> new_opd1 = opd1->update(dag, opd, opd_modifier))
	{
		n1 = *new_opd1;
		rtn = true;
	}
	if(isBinary())
	{
		if(opd2 == opd)
		{
			n2 = opd_modifier;
			rtn = true;
		}
		else if(Option<const Operand*> new_opd2 = opd2->update(dag, opd, opd_modifier))
		{
			n2 = *new_opd2;
			rtn = true;
		}
		return rtn ? some(dag.autoOp(_opr, n1, n2)) : none;
	}
	else
		return rtn ? some(dag.autoOp(_opr, n1)) : none;		
}

void OperandArith::parseAffineEquation(AffineEquationState& state) const
{
	switch(_opr)
	{
		case ARITHOPR_NEG:
			state.reverseSign();
			opd1->parseAffineEquation(state);
			state.reverseSign();
			return;
		case ARITHOPR_ADD:
			opd1->parseAffineEquation(state);
			opd2->parseAffineEquation(state);
			return;
		case ARITHOPR_SUB:
			opd1->parseAffineEquation(state);
			state.reverseSign();
			opd2->parseAffineEquation(state);
			state.reverseSign();
			return;
		default:
			ASSERTP(false, "not affine");
	}
}

// An ArithExpr can be const if all of its children are const!
Option<Constant> OperandArith::evalConstantOperand() const
{
	Option<Constant> val1 = opd1->evalConstantOperand();
	Option<Constant> val2 = isUnary() ? elm::none : opd2->evalConstantOperand();
	if(val1 && (val2 || isUnary()))
	{
#		define VAL1 (*val1)//.val()
#		define VAL2 (*val2)//.val()
		switch(_opr)
		{
			case ARITHOPR_NEG:
				return -VAL1;
			case ARITHOPR_ADD:
				return VAL1 + VAL2;
			case ARITHOPR_SUB:
				return VAL1 - VAL2;
			case ARITHOPR_MUL:
				return VAL1 * VAL2;
			case ARITHOPR_MULH:
				return Constant( (t::int64(VAL1.val())*t::int64(VAL2.val())) / t::int64(0x100000000ul) );
			case ARITHOPR_DIV:
				if(VAL2 == 0)
					return none;
				return VAL1 / VAL2;
			case ARITHOPR_MOD:
				if(VAL2 == 0)
					return none;
				return VAL1 % VAL2;
			case ARITHOPR_CMP:
				return none; // This can't be evaluated (wouldn't make much sense if this case was matched after the if anyway)
		}
#		undef VAL1 
#		undef VAL2
	}
	return none; // one of the operands is not constant or we failed to evaluate it
}

/**
 * @fn elm::Pair<const Operand*, Constant> OperandArith::extractAdditiveConstant(DAG& dag) const;
 * @brief Extracts a constant from a sum (and removes it from this)
 * @warning Always true: if x.extract() == (o1, k1), then x = o1 + k1 or x = k1 (o1 NULL)
 * @return First operand is the new this (can be NULL), second operand is the extracted constant (can be NULL)
 */
elm::Pair<const Operand*, Constant> OperandArith::extractAdditiveConstant(DAG& dag) const
{
	const Operand *o1, *o2;
	Constant k1, k2;
	let(o1, k1) = opd1->extractAdditiveConstant(dag);
	if(isUnary())
	{
		ASSERT(_opr == ARITHOPR_NEG);
		ASSERTP(o1, "TODO")
		return pair(dag.autoOp(_opr, o1), -k1);
	}
	let(o2, k2) = opd2->extractAdditiveConstant(dag);
	if(_opr == ARITHOPR_ADD)
	{
		const Operand* rtn;
		if(o1)
		{
			if(o2)
			{
				if(o1 == opd1 && o2 == opd2) // id
					rtn = this; // optimize a call to dag
				else
					rtn = dag.add(o1, o2);
			}
			else // !o2
				rtn = o1;
		}
		else
		{
			if(o2) // !o1
				rtn = o2;
			else // !o1, !o2
				rtn = NULL;
		}
		return pair(rtn, k1 + k2);
	}
	else // TODO! support ARITHOR_SUB too
		return pair(static_cast<const Operand*>(this), Constant(0));
}

// Warning: Option=none does not warrant that nothing has been simplified!
Option<const Operand*> OperandArith::simplify(DAG& dag) const
{
	// before anything, test our groundness
	if(Option<Constant> val = evalConstantOperand())
		if((*val).isValid())
			return some(dag.cst(*val));
	if(isUnary())
	{
		if(Option<const Operand*> o = opd1->simplify(dag))
			return some(dag.autoOp(_opr, *o));
		return none; // if there was anything to simplify, it would have been done earlier with evalConstantOperand
	}
	// binary case
	// simplify operands first
	const Operand *new_opd1 = opd1, *new_opd2 = opd2;
	if(Option<const Operand*> o = opd1->simplify(dag))
		new_opd1 = *o;
	if(Option<const Operand*> o = opd2->simplify(dag))
		new_opd2 = *o;

	// reminder: our arithexpr shouldn't be const (case handled by the earlier evalConstantOperand test)
	// test neutrals
	bool opd1_is_constant = new_opd1->isAConst();
	bool opd2_is_constant = new_opd2->isAConst();
	Constant opd1_val, opd2_val;
	if(opd1_is_constant) opd1_val = new_opd1->toConstant();
	if(opd2_is_constant) opd2_val = new_opd2->toConstant();
	switch(_opr)
	{
		case ARITHOPR_ADD:
			if(opd1_is_constant && opd1_val == 0)
				return some(new_opd2); // [x / 0 + x]
		case ARITHOPR_SUB:
			if(opd2_is_constant && opd2_val == 0)
				return some(new_opd1); // [x / x +- 0]
			break;
		case ARITHOPR_MUL:
			if(opd2_is_constant && opd2_val == 1)
				return some(new_opd1); // [x / 1 * x]
		case ARITHOPR_MULH:
			break; // nothing to do
		case ARITHOPR_DIV:
		case ARITHOPR_MOD:
			if(opd1_is_constant && opd1_val == 1)
				return some(new_opd2); // [x / x */% 1]
			break;
		case ARITHOPR_CMP:
			return none;
		case ARITHOPR_NEG:
			crash(); // unary operators should have been handled earlier
	}
	// additional tests
	// TODO: test [x + y / x - -y] and vice-versa
	switch(_opr)
	{
		case ARITHOPR_ADD:
			if(opd1->kind() == ARITH && opd1->toArith().opr() == ARITHOPR_NEG)
				return (dag.sub(opd2, opd1->toArith().left()))->simplify(dag); // [y - x / -x + y]
			if(opd2->kind() == ARITH && opd2->toArith().opr() == ARITHOPR_NEG)
				return (dag.sub(opd1, opd2->toArith().left()))->simplify(dag); // [x - y / -y + x]
			break;
		case ARITHOPR_SUB:
			if(opd1_is_constant && opd1_val == 0)
				return some(dag.neg(opd2)); // [-x / 0 - x]
			if(*opd1 == *opd2)
				return some(dag.cst(0)); // [0 / x - x]
			break;
		case ARITHOPR_MUL:
		case ARITHOPR_MULH:
			if(opd2_is_constant && opd2_val == 0)
				return some(dag.cst(0)); // [0 / x*0]
		case ARITHOPR_DIV:
		case ARITHOPR_MOD:
			if(opd1_is_constant && opd1_val == 0) 
				return some(dag.cst(0)); // [0 / 0*/%x]
			break;
		case ARITHOPR_CMP:
			return none;
		case ARITHOPR_NEG:
			crash(); // unary operators should have been handled earlier
	}
	return none;
}
const Operand* OperandArith::replaceConstants(DAG& dag, const ConstantVariablesCore& constants, Vector<OperandVar>& replaced_vars) const
{
	return dag.autoOp(
		_opr,
		opd1->replaceConstants(dag, constants, replaced_vars),
		isBinary() ? opd2->replaceConstants(dag, constants, replaced_vars) : NULL);
}

/**
 * @fn bool OperandArith::isLinear(bool only_linear_opr) const
 * @brief      Determines if the operandlinear.
 * @param      only_linear_opr  Only allow linear operators
 * @return     True if linear, False otherwise.
 */
bool OperandArith::isLinear(bool only_linear_opr) const
{
	switch(_opr)
	{
		// linear iff all operands are linear
		case ARITHOPR_NEG:
			return opd1->isLinear(only_linear_opr);
		case ARITHOPR_ADD:
		case ARITHOPR_SUB:
			return opd1->isLinear(only_linear_opr) && opd2->isLinear(only_linear_opr);

		// linear iff any operand is constant
		case ARITHOPR_MUL:
			return !only_linear_opr && (opd1->isConstant() || opd2->isConstant());

		// linear iff right operand is constant
		case ARITHOPR_DIV:
		case ARITHOPR_MOD:
			// return !only_linear_opr && opd2->isConstant();
			return !only_linear_opr && opd2->isConstant(); // just mod & div should be no problem // TODO! test we don't lose IPs because of that

		// hopeless
		case ARITHOPR_MULH:
		case ARITHOPR_CMP:
		default:
			return false;
	}
}

/**
 * @class AffineEquationState
 * @author Jordy Ruiz 
 * @brief Provide tools to analyse affine equations.
 * For a given variable var, try to find an integer constant &Delta; such that var = +-SP + &Delta; 
 */

io::Output& operator<<(io::Output& out, const operand_kind_t kind)
{
	switch(kind)
	{		
		case CST:	out << "CST";	break;
		case VAR:	out << "VAR";	break;
		case MEM:	out << "MEM";	break;
		case TOP:	out << "TOP";	break;
		case ITER:  out << "ITER";	break;
		case ARITH: out << "ARITH";	break;
	}
	return out;
}
io::Output& operator<<(io::Output& out, operandmem_kind_t kind)
{
	switch(kind)
	{
		case OPERANDMEM_ABSOLUTE:
			out << "ABSOLUTE";
			break;
		case OPERANDMEM_RELATIVE:
			out << "RELATIVE";
			break;
	}
	return out;
}
io::Output& operator<<(io::Output& out, arithoperator_t opr)
{
	switch(opr)
	{
		case ARITHOPR_NEG:
			out << "-";
			break;
		case ARITHOPR_ADD:
			out << "+";
			break;
		case ARITHOPR_SUB:
#			ifndef NO_UFT8
				out << "−";
#			else
				out << "-";
#			endif
			break;
		case ARITHOPR_MUL:
			out << "*";
			break;
		case ARITHOPR_MULH:
			out << "*H";
			break;
		case ARITHOPR_DIV:
			out << "/";
			break;
		case ARITHOPR_MOD:
			out << "mod";
			break;
		case ARITHOPR_CMP:
			out << "~";
			break;
	}
	return out;
}
