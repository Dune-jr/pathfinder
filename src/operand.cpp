/*
 * Defines our operands
 */
#include <elm/io/Output.h>
#include "operand.h"
#include "debug.h"

using namespace elm;
// using namespace elm::avl;

/**
 * @class Operand
 * @brief Abstract class for any operand of a Predicate
 */
/**
 * @class OperandConst
 * @brief A constant operand, which can be relative to SP0
 */
/**
 * @class OperandVar
 * @brief A variable in the program analysis, which can be a register or a temporary variable
 */
/**
 * @class OperandMem
 * @brief A memory cell, identified by its address (absolute or relative to SP0)
 */
/**
 * @class OperandArithExpr
 * @brief An operand composed of (an)other operand(s) and a binary or unary operator
 */

/**
 * @fn Operand* Operand::copy() const;
 * @brief Copy the Operand
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
 * @fn bool Operand::involvesMemory() const;
 * @brief Check for any reference to a value in the stack or the heap in the Operand
 */
/**
 * @fn bool Operand::update(const Operand& opd, const Operand& opd_modifier);
 * @brief
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
 * @brief
 */
/**
 * @fn Option<OperandConst> Operand::evalConstantOperand() const;
 * @brief
 * @warning Deprecated, bound to be removed in the future
 */
/**
 * @fn Option<Operand*> Operand::simplify();
 * @brief Simplify the operand, may return a new equivalent Operand to replace the current one with
 * @warning If elm::none is returned, it is not safe to assume that nothing has been simplified!
 */
/**
 * @fn Option<Operand*> Operand::replaceConstants(const ConstantVariablesSimplified& constants, Vector<OperandVar>& replaced_vars);
 * @brief
 */
/**
 * @fn bool Operand::accept(OperandVisitor& visitor) const;
 * @brief
 */
/**
 * @fn operand_kind_t Operand::kind() const;
 * @brief Returns an operand kind, in order to characterize each implementation of Operand
 */

// Operands: Constants
OperandConst::OperandConst(const OperandConst& opd) : _value(opd._value) { }
OperandConst::OperandConst(const Constant& value) : _value(value) { }
OperandConst::OperandConst() : _value(0) { }
OperandConst::~OperandConst() { }
Operand* OperandConst::copy() const { return new OperandConst(_value); }
io::Output& OperandConst::print(io::Output& out) const
{
	// if(_value >= 64 || _value <= -63) // print large values in hex
	// 	return (out << "0x" << io::hex(_value));
	return (out << _value);
}
OperandConst& OperandConst::operator=(const OperandConst& opd) { _value = opd._value; return *this; }
bool OperandConst::operator==(const Operand& o) const
{
	if(o.kind() == kind())
		return _value == ((OperandConst&)o)._value;
	else
		return false; // Operand types are not matching
}
unsigned int OperandConst::countTempVars() const { return 0; }
bool OperandConst::getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const
{
	expr = this; // Assume we are the expr
	return false; // We haven't found an isolated tempvar
}
int OperandConst::involvesVariable(const OperandVar& opdv) const { return 0; }
bool OperandConst::involvesMemoryCell(const OperandMem& opdm) const { return false; }
Option<Constant> OperandConst::involvesStackBelow(const Constant& stack_limit) const { return elm::none; }
bool OperandConst::involvesMemory() const { return false; }
bool OperandConst::update(const Operand& opd, const Operand& opd_modifier) { return false; }
// pop_result_t OperandConst::doAffinePop(Operand*& opd_result, Operand*& new_opd) { opd_result = this->copy(); return POPRESULT_DONE; }
void OperandConst::parseAffineEquation(AffineEquationState& state) const
{
	ASSERT(_value.isValid());
	state.addToDelta(_value.val());
	if(_value.isRelative())
		state.onSpFound(_value.sign());
}
Option<OperandConst> OperandConst::evalConstantOperand() const
{
	if(!_value.isValid())
		return none;
	return some(*this);
}
Option<Operand*> OperandConst::simplify() { return none; }
Option<Operand*> OperandConst::replaceConstants(const ConstantVariablesSimplified& constants, Vector<OperandVar>& replaced_vars) { return none; }

// Operands: Variables
OperandVar::OperandVar() : _addr(777) { }
OperandVar::OperandVar(const OperandVar& opd) : _addr(opd._addr) { }
OperandVar::OperandVar(t::int32 addr) : _addr(addr) { }
// OperandVar::~OperandVar() { }
Operand* OperandVar::copy() const { return new OperandVar(_addr); }
io::Output& OperandVar::print(io::Output& out) const
{
	if(!isTempVar())
		out << "?" << _addr; // register
	else
		out << "t" << -_addr; // temporary
	return out; 
}
OperandVar& OperandVar::operator=(const OperandVar& opd){ _addr = opd._addr; return *this; }
bool OperandVar::operator==(const Operand& o) const
{	
	if(o.kind() == kind())
		return _addr == ((OperandVar&)o)._addr;
	else
		return false; // Operand types are not matching
}
unsigned int OperandVar::countTempVars() const { return isTempVar() ? 1 : 0; }
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
int OperandVar::involvesVariable(const OperandVar& opdv) const
{
	return (int)(opdv == *this);
}
bool OperandVar::involvesMemoryCell(const OperandMem& opdm) const { return false; }
Option<Constant> OperandVar::involvesStackBelow(const Constant& stack_limit) const { return elm::none; }
bool OperandVar::involvesMemory() const { return false; }
// since the parent has to do the modification, and var has no child, return false
bool OperandVar::update(const Operand& opd, const Operand& opd_modifier) { return false; }
// pop_result_t OperandVar::doAffinePop(Operand*& opd_result, Operand*& new_opd) { opd_result = this->copy(); return POPRESULT_DONE; }
void OperandVar::parseAffineEquation(AffineEquationState& state) const { state.onVarFound(this->_addr); }
Option<OperandConst> OperandVar::evalConstantOperand() const { return none; }
Option<Operand*> OperandVar::simplify() { return none; }
Option<Operand*> OperandVar::replaceConstants(const ConstantVariablesSimplified& constants, Vector<OperandVar>& replaced_vars)
{
	if(constants.isConstant(this->_addr))
	{
		replaced_vars.push(*this);
		return new OperandConst(constants[this->_addr]);
	}
	return none;
}

// Operands: Memory
OperandMem::OperandMem(const OperandConst& opdc)
{
	_opdc = new OperandConst(opdc);
}
OperandMem::OperandMem(const OperandMem& opd) // : _kind(opd._kind)
{
	_opdc = new OperandConst(opd.getConst());
}
OperandMem::OperandMem() : _opdc(NULL) { }
OperandMem::~OperandMem()
{
	if(_opdc) delete _opdc;
}
Operand* OperandMem::copy() const
{
	return new OperandMem(*_opdc);
}
io::Output& OperandMem::print(io::Output& out) const
{
	out << "[";
	return (out << *_opdc << "]");
}
OperandMem& OperandMem::operator=(const OperandMem& opd) { delete _opdc; _opdc = new OperandConst(*(opd._opdc)); return *this; }
bool OperandMem::operator==(const Operand& o) const
{	// untested so far	
	if(o.kind() != kind())
		return false; // Operand types are not matching
	OperandMem& o_mem = (OperandMem&)o; // Force conversion
	return *_opdc == o_mem.getConst();
}
unsigned int OperandMem::countTempVars() const { return 0; }
bool OperandMem::getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const
{
	expr = this; // Assume we are the expr
	return false; // We haven't found an isolated tempvar
}
int OperandMem::involvesVariable(const OperandVar& opdv) const { return 0; }
bool OperandMem::involvesMemoryCell(const OperandMem& opdm) const {	return *this == opdm; }
Option<Constant> OperandMem::involvesStackBelow(const Constant& stack_limit) const 
{
	if(_opdc->value().isRelative() && (_opdc->value().val() < stack_limit.val()))
		return elm::some(_opdc->value());
	return elm::none;
}
bool OperandMem::involvesMemory() const { return true; }
bool OperandMem::update(const Operand& opd, const Operand& opd_modifier) { return false; }
// pop_result_t OperandMem::doAffinePop(Operand*& opd_result, Operand*& new_opd) { return POPRESULT_FAIL; }
void OperandMem::parseAffineEquation(AffineEquationState& state) const { ASSERT(false); } // should never happen
Option<OperandConst> OperandMem::evalConstantOperand() const { return none; }
Option<Operand*> OperandMem::simplify() { return none; }
Option<Operand*> OperandMem::replaceConstants(const ConstantVariablesSimplified& constants, Vector<OperandVar>& replaced_vars) { return none; }
 
// Operands: Top
int OperandTop::next_id = 0;
OperandTop::OperandTop(bool identified) : id(identified ? next_id++ : -1) { ASSERT(next_id >= 0); }
OperandTop::OperandTop(const OperandTop& opd) : id(opd.id) { }
Operand* OperandTop::copy() const
{
	return new OperandTop(id);
}
io::Output& OperandTop::print(io::Output& out) const
{
	return (out << "T" << id);
}
OperandTop& OperandTop::operator=(const OperandTop& opd)
{
	id = opd.id;
	return *this;
}
bool OperandTop::operator==(const Operand& o) const
{
	if(o.kind() != kind())
		return false; // Operand types are not matching
	OperandTop& o_top = (OperandTop&)o; // Force conversion
	return o_top.id == id;
}
unsigned int OperandTop::countTempVars() const { return 0; }
bool OperandTop::getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const
{
	expr = this; // Assume we are the expr
	return false; // We haven't found an isolated tempvar
}
int OperandTop::involvesVariable(const OperandVar& opdv) const { return 0; }
bool OperandTop::involvesMemoryCell(const OperandMem& opdm) const {	return false; }
Option<Constant> OperandTop::involvesStackBelow(const Constant& stack_limit) const { return elm::none; }
bool OperandTop::involvesMemory() const { return false; }
bool OperandTop::update(const Operand& opd, const Operand& opd_modifier) { ASSERT(false); return false; }
void OperandTop::parseAffineEquation(AffineEquationState& state) const { ASSERT(false); } // should never happen (for Top too?)
Option<OperandConst> OperandTop::evalConstantOperand() const { return none; }
Option<Operand*> OperandTop::simplify() { return none; }
Option<Operand*> OperandTop::replaceConstants(const ConstantVariablesSimplified& constants, Vector<OperandVar>& replaced_vars) { return none; }
 
// Operands: Arithmetic Expressions
OperandArithExpr::OperandArithExpr(arithoperator_t opr, const Operand& opd1_)
	: _opr(opr)
{
	opd1 = opd1_.copy();
}
OperandArithExpr::OperandArithExpr(arithoperator_t opr, const Operand& opd1_, const Operand& opd2_)
	: _opr(opr)
{
	opd1 = opd1_.copy();
	opd2 = opd2_.copy();
}
OperandArithExpr::OperandArithExpr(const OperandArithExpr& x)
	: _opr(x._opr)
{
	opd1 = x.opd1->copy();
	if(x.isBinary())
		opd2 = x.opd2->copy();
}
OperandArithExpr::~OperandArithExpr()
{
	delete opd1;
	if(isBinary())
		delete opd2;
}
Operand* OperandArithExpr::copy() const
{
	if(isUnary())
		return new OperandArithExpr(_opr, *opd1);	
	return new OperandArithExpr(_opr, *opd1, *opd2);
}
io::Output& OperandArithExpr::print(io::Output& out) const
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
OperandArithExpr& OperandArithExpr::operator=(const OperandArithExpr& opd)
{
	_opr = opd._opr;
	delete opd1;
	opd1 = opd.opd1->copy();
	if(isBinary())
	{
		delete opd2;
		opd2 = opd.opd2->copy();
	}
	return *this;
}
bool OperandArithExpr::operator==(const Operand& o) const
{
	if(o.kind() != kind())
		return false; // Operand types are not matching
	OperandArithExpr& o_arith = (OperandArithExpr&)o; // Force conversion
	return (_opr == o_arith._opr) && (*opd1 == *(o_arith.opd1)) && (isUnary() || *opd2 == *(o_arith.opd2));
}
unsigned int OperandArithExpr::countTempVars() const
{
	if(isUnary())
		return opd1->countTempVars();
	return opd1->countTempVars() + opd2->countTempVars();
}
bool OperandArithExpr::getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const
{
	expr = this;
	return false;
}
int OperandArithExpr::involvesVariable(const OperandVar& opdv) const
{
	if(isUnary())
		return opd1->involvesVariable(opdv);
	return opd1->involvesVariable(opdv) + opd2->involvesVariable(opdv);
}
Option<Constant> OperandArithExpr::involvesStackBelow(const Constant& stack_limit) const
{
	if(isUnary())
		return opd1->involvesStackBelow(stack_limit);
	if(const Option<Constant>& opd1_rtn = opd1->involvesStackBelow(stack_limit))
		return opd1_rtn;
	else
		return opd2->involvesStackBelow(stack_limit);
}
bool OperandArithExpr::involvesMemoryCell(const OperandMem& opdm) const
{
	if(isUnary())
		return opd1->involvesMemoryCell(opdm);
	return opd1->involvesMemoryCell(opdm) || opd2->involvesMemoryCell(opdm);
}
bool OperandArithExpr::involvesMemory() const
{
	if(isUnary())
		return opd1->involvesMemory();
	return opd1->involvesMemory() || opd2->involvesMemory();	
}
bool OperandArithExpr::update(const Operand& opd, const Operand& opd_modifier)
{
	bool rtn = false;
	if(*opd1 == opd)
	{
		delete opd1;
		opd1 = opd_modifier.copy();
		rtn = true;
	}
	else if(opd1->update(opd, opd_modifier))
		rtn = true;
	if(isUnary())
		return rtn; // stop here
	if(*opd2 == opd)
	{
		delete opd2;
		opd2 = opd_modifier.copy();
		rtn = true;
	}
	else if(opd2->update(opd, opd_modifier))
		rtn = true;
	return rtn;
}
/*
// TO*DO handle ARITHOPR_NEG _and_ replace in isAffine() the (_opr == +) || (_opr == -) statements by adding the NEG case! Handle unary everywhere!
pop_result_t OperandArithExpr::doAffinePop(Operand*& opd_result, Operand*& new_opd)
{
	// it's important to pop the items to the right side of the operator (otherwise problems with operator -)
	pop_result_t result = opd2->doAffinePop(opd_result, new_opd);
	switch(result)
	{
		case POPRESULT_FAIL:
			return POPRESULT_FAIL;
		case POPRESULT_DONE: // opd2 is an OperandConst / OperandVar, so case (.1.) + X
			// opd_result already contains X
			// and new_opd will contain (.1.)
			new_opd = opd1->copy();
			switch(_opr)
			{
				case ARITHOPR_ADD: // this is all fine and easy
					return POPRESULT_CONTINUE;
				case ARITHOPR_SUB:
					if(opd_result->kind() == CST)
					{
						opd_result = new OperandConst(-((OperandConst*)opd_result)->value()); // opposite of the value
						return POPRESULT_CONTINUE;
					}
					else // 
V
VAR
						return POPRESULT_FAIL; // we do not handle X - t1 or X - sp (TO*DO: is it bad? maybe we should improve this)
				default:
					return POPRESULT_FAIL; // this case shouldn't happen
			}
		case POPRESULT_CONTINUE: // opd2 is an OperandArithExpr, so case (.1.) +- (.2.)
			// opd_result already contains some X into (.2.)
			// new_opd already contains the new righ-handside (.2.)
			new_opd = new OperandArithExpr(_opr, *opd1, *new_opd); // so return (.1.) +- (.2.)
			switch(_opr)
			{
				case ARITHOPR_ADD: // all fine and easy, again
					return POPRESULT_CONTINUE;
				case ARITHOPR_SUB:
					if(opd_result->kind() == CST)
					{
						opd_result = new OperandConst(-((OperandConst*)opd_result)->value()); // we have to take opposite of the value every time
						return POPRESULT_CONTINUE;
					}
					// example of why we had  to do this: if we have (...) - (t1 + 2)
					// (t1+2).doAffinePop() will return +2 even though it's actually -2
					else // 
V
VAR
						return POPRESULT_FAIL; // TO*DO: maybe improve this, see above
				default:
					return POPRESULT_FAIL; // this case shouldn't happen
			}
	}
	return POPRESULT_FAIL;
}
*/

void OperandArithExpr::parseAffineEquation(AffineEquationState& state) const
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
Option<OperandConst> OperandArithExpr::evalConstantOperand() const
{
	Option<OperandConst> val1 = opd1->evalConstantOperand();
	Option<OperandConst> val2 = isUnary() ? elm::none : opd2->evalConstantOperand();
	if(val1 && (val2 || isUnary()))
	{
#		define V1 (*val1).value().val()
#		define V2 (*val2).value().val()
		switch(_opr)
		{
			case ARITHOPR_NEG:
				return some(OperandConst(-V1));
			case ARITHOPR_ADD:
				return some(OperandConst(V1 + V2));
			case ARITHOPR_SUB:
				return some(OperandConst(V1 - V2));
			case ARITHOPR_MUL:
				return some(OperandConst(V1 * V2));
			case ARITHOPR_MULH:
				return some(OperandConst(Constant( (t::int64(V1)*t::int64(V2))/t::int64(0x100000000ul) )));
			case ARITHOPR_DIV:
				if(V2 == 0)
					return none;
				return some(OperandConst(V1 / V2));
			case ARITHOPR_MOD:
				if(V2 == 0)
					return none;
				return some(OperandConst(V1 % V2));
			case ARITHOPR_CMP:
				return none; // This can't be evaluated (wouldn't make much sense if this case was matched after the if anyway)
		}
#		undef V1 
#		undef V2
	}
	return none; // one of the operands is not constant or we failed to evaluate it
}
// Warning: Option=none does not warrant that nothing has been simplified!
Option<Operand*> OperandArithExpr::simplify()
{
	// before anything, test our groundness
	if(Option<OperandConst> val = evalConstantOperand())
	{
		if((*val).value().isValid())
			return some((Operand*)((*val).copy()));
	}
	if(isUnary())
	{
		if(Option<Operand*> o = opd1->simplify())
			opd1 = *o;
		return none; // if there was anything to simplify, it would have been done earlier with evalConstantOperand
	}
	// binary case
	// simplify operands first
	if(Option<Operand*> o = opd1->simplify())
		opd1 = *o;
	if(Option<Operand*> o = opd2->simplify())
		opd2 = *o;

	// reminder: our arithexpr shouldn't be const (case handled by the earlier evalConstantOperand test)
	// test neutrals
	bool opd1_is_constant = opd1->kind() == CST;
	bool opd2_is_constant = opd2->kind() == CST;
	Constant opd1_val, opd2_val;
	if(opd1_is_constant) opd1_val = ((OperandConst*)opd1)->value();
	if(opd2_is_constant) opd2_val = ((OperandConst*)opd2)->value();
	switch(_opr)
	{
		case ARITHOPR_ADD:
			if(opd1_is_constant && opd1_val == 0)
				return some(opd2->copy()); // [x / 0 + x]
		case ARITHOPR_SUB:
			if(opd2_is_constant && opd2_val == 0)
				return some(opd1->copy()); // [x / x +- 0]
			break;
		case ARITHOPR_MUL:
			if(opd2_is_constant && opd2_val == 1)
				return some(opd1->copy()); // [x / 1 * x]
		case ARITHOPR_MULH:
			break; // nothing to do
		case ARITHOPR_DIV:
		case ARITHOPR_MOD:
			if(opd1_is_constant && opd1_val == 1)
				return some(opd2->copy()); // [x / x */% 1]
			break;
		case ARITHOPR_CMP:
			return none;
		case ARITHOPR_NEG:
			ASSERT(false); // unary operators should have been handled earlier
	}
	// additional tests
	// TODO: test [x + y / x - -y] and vice-versa
	switch(_opr)
	{
		case ARITHOPR_ADD:
			if(opd1->kind() == ARITH && ((OperandArithExpr*)opd1)->opr() == ARITHOPR_NEG)
				return ((Operand*) new OperandArithExpr(ARITHOPR_SUB, *opd2, ((OperandArithExpr*)opd1)->leftOperand()))->simplify(); // [y - x / -x + y]
			if(opd2->kind() == ARITH && ((OperandArithExpr*)opd2)->opr() == ARITHOPR_NEG)
				return ((Operand*) new OperandArithExpr(ARITHOPR_SUB, *opd1, ((OperandArithExpr*)opd2)->leftOperand()))->simplify(); // [x - y / -y + x]
			break;
		case ARITHOPR_SUB:
			if(opd1_is_constant && opd1_val == 0)
				return some((Operand*) new OperandArithExpr(ARITHOPR_NEG, *opd2)); // [-x / 0 - x]
			if(*opd1 == *opd2)
				return some((Operand*) new OperandConst(0)); // [0 / x - x]
			break;
		case ARITHOPR_MUL:
		case ARITHOPR_MULH:
			if(opd2_is_constant && opd2_val == 0)
				return some((Operand*) new OperandConst(0)); // [0 / x*0]
		case ARITHOPR_DIV:
		case ARITHOPR_MOD:
			if(opd1_is_constant && opd1_val == 0) 
				return some((Operand*) new OperandConst(0)); // [0 / 0*/%x]
			break;
		case ARITHOPR_CMP:
			return none;
		case ARITHOPR_NEG:
			ASSERT(false); // unary operators should have been handled earlier
	}
	return none;
}
Option<Operand*> OperandArithExpr::replaceConstants(const ConstantVariablesSimplified& constants, Vector<OperandVar>& replaced_vars)
{
	if(Option<Operand*> o = opd1->replaceConstants(constants, replaced_vars))
	{
		delete opd1;
		opd1 = *o;
	}
	if(isBinary())
	{
		if(Option<Operand*> o = opd2->replaceConstants(constants, replaced_vars))
		{
			delete opd2;
			opd2 = *o;
		}
	}
	return none;
}

bool OperandArithExpr::isLinear() const
{
	switch(_opr)
	{
		// linear iff all operands are linear
		case ARITHOPR_NEG:
		case ARITHOPR_ADD:
		case ARITHOPR_SUB:
			return opd1->isLinear() && (isUnary() || opd2->isLinear());

		// linear iff any operand is constant
		case ARITHOPR_MUL:
			return opd1->isConstant() || opd2->isConstant();

		// linear iff right operand is constant
		case ARITHOPR_DIV:
		case ARITHOPR_MOD:
			return opd2->isConstant();

		// hopeless
		case ARITHOPR_MULH:
		case ARITHOPR_CMP:
		default:
			return false;
	}
}

/**
 * @class AffineEquationState
 * @brief Provide tools to analyse affine equations.
 * For a given variable var, try to find an integer constant &Delta; such that var = +-SP + &Delta; 
 */

io::Output& operator<<(io::Output& out, operand_kind_t kind)
{
	switch(kind)
	{		
		case CST:
			out << "(CONST)";
			break;
		case VAR:
			out << "(VAR)";
			break;
		case MEM:
			out << "(MEM)";
			break;
		case TOP:
			out << "(TOP)";
			break;
		case ARITH:
			out << "(ARITH)";
			break;
	}
	return out;
}
io::Output& operator<<(io::Output& out, operandmem_kind_t kind)
{
	switch(kind)
	{
		case OPERANDMEM_ABSOLUTE:
			out << "(ABSOLUTE)";
			break;
		case OPERANDMEM_RELATIVE:
			out << "(RELATIVE)";
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
	}
	return out;
}
