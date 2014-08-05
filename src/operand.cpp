#include <elm/io/Output.h>
#include "operand.h"

using namespace elm;

// Operands: Constants
OperandConst::OperandConst(const OperandConst& opd) : _value(opd._value) { }
OperandConst::OperandConst(t::int32 value) : _value(value) { }
OperandConst::OperandConst() : _value(0) { }
Operand* OperandConst::copy() const { return new OperandConst(_value); }
io::Output& OperandConst::print(io::Output& out) const
{
	if(_value >= 64 || _value <= -63) // print large values in hex
		return (out << "0x" << io::hex(_value));
	return (out << _value);
}
bool OperandConst::operator==(const Operand& o) const
{
	if(o.kind() == kind())
		return _value == ((OperandConst&)o)._value;
	else
		return false; // Operand types are not matching
}
unsigned int OperandConst::countTempVars() const { return 0; }
bool OperandConst::getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const
{
	expr = this->copy(); // Assume we are the expr
	return false; // We haven't found an isolated tempvar
}
bool OperandConst::involvesVariable(const OperandVar& opdv) const { return false; }
bool OperandConst::involvesMemoryCell(const OperandMem& opdm) const { return false; }
bool OperandConst::involvesMemory() const { return false; }
operand_state_t OperandConst::updateVar(const OperandVar& opdv, const Operand& opd_modifier) { return OPERANDSTATE_UNCHANGED; }
Option<OperandConst> OperandConst::evalConstantOperand() const { return some(*this); }
Option<Operand*> OperandConst::simplify() { return none; }
Option<Operand*> OperandConst::replaceConstants(const ConstantVariablesSimplified& constants) { return none; }

// Operands: Variables
OperandVar::OperandVar(const OperandVar& opd) : _addr(opd._addr) { }
OperandVar::OperandVar(t::int32 addr) : _addr(addr) { }
Operand* OperandVar::copy() const { return new OperandVar(_addr); }
io::Output& OperandVar::print(io::Output& out) const
{
	if(!isTempVar())
		out << "?" << _addr; // register
	else
		out << "t" << -_addr; // temporary
	return out; 
}

bool OperandVar::operator==(const Operand& o) const
{	
	if(o.kind() == kind())
		return _addr == ((OperandVar&)o)._addr;
	else
		return false; // Operand types are not matching
}
unsigned int OperandVar::countTempVars() const { return isTempVar() ? 1 : 0; }
bool OperandVar::getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const
{
	if(isTempVar()) // temporary
	{
		temp_var = *this; // we found an isolated tempvar
		return true;
	}
	else // register
	{
		expr = this->copy();
		return false;
	}
}
bool OperandVar::involvesVariable(const OperandVar& opdv) const
{
	return opdv == *this;
}
bool OperandVar::involvesMemoryCell(const OperandMem& opdm) const { return false; }
bool OperandVar::involvesMemory() const { return false; }
// since the parent has to do the modification, and var has no child, return false
operand_state_t OperandVar::updateVar(const OperandVar& opdv, const Operand& opd_modifier) { return OPERANDSTATE_UNCHANGED; }
Option<OperandConst> OperandVar::evalConstantOperand() const { return none; }
Option<Operand*> OperandVar::simplify() { return none; }
Option<Operand*> OperandVar::replaceConstants(const ConstantVariablesSimplified& constants)
{
	if(constants.isConstant(this->_addr))
		return new OperandConst(constants[this->_addr]);
	return none;
}

// Operands: Memory
OperandMem::OperandMem(const OperandConst& opdc, bool relative)
{
	_opdc = new OperandConst(opdc);
	if(relative)
		_kind = OPERANDMEM_RELATIVE;
	else
		_kind = OPERANDMEM_ABSOLUTE;
}
OperandMem::OperandMem(const OperandMem& opd)
{
	_opdc = new OperandConst(opd.getConst());
	if(opd.isRelative())
		_kind = OPERANDMEM_RELATIVE;
	else
		_kind = OPERANDMEM_ABSOLUTE;
}
OperandMem::OperandMem() : _opdc(NULL), _kind(OPERANDMEM_ABSOLUTE) { }
Operand* OperandMem::copy() const
{
	if(isRelative())
		return new OperandMem(*_opdc, true);
	else
		return new OperandMem(*_opdc);
}
io::Output& OperandMem::print(io::Output& out) const
{
	out << "[";
	if(isRelative())
		out << "sp + ";
	out << *_opdc << "]";
	return out;
}
bool OperandMem::operator==(const Operand& o) const
{	// untested so far	
	if(o.kind() == kind())
		return false; // Operand types are not matching
	OperandMem& o_mem = (OperandMem&)o; // Force conversion
	if(o_mem.isRelative() != isRelative())
		return false;
	if(!(*_opdc == o_mem.getConst()))
		return false;
	return true;
}
unsigned int OperandMem::countTempVars() const { return 0; }
bool OperandMem::getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const
{
	expr = this->copy(); // Assume we are the expr
	return false; // We haven't found an isolated tempvar
}
bool OperandMem::involvesVariable(const OperandVar& opdv) const { return false; }
bool OperandMem::involvesMemoryCell(const OperandMem& opdm) const {	return *this == opdm; }
bool OperandMem::involvesMemory() const { return true; }
operand_state_t OperandMem::updateVar(const OperandVar& opdv, const Operand& opd_modifier)
{
	return OPERANDSTATE_UNCHANGED; // no match
}
/*operand_state_t OperandMem::updateVar(const OperandVar& opdv, const Operand& opd_modifier)
{
	// if: this OperandMem involves a variable, and it matches the variable we have to update
	if((_kind & OPERANDMEMFLAG_HASVAR) && *_opdv == opdv)
	{
		// we have to [opd_modifier / opdv]
		switch(opd_modifier.kind())
		{
			case OPERAND_CONST:
				{
					OperandConst& o = (OperandConst&)opd_modifier;
					// delete _opdv; // TODO (also _opdc)
					if(_kind == OPERANDMEM_RELATIVE)
						_opdc = new OperandConst(_opdc->value() + o.value());
					else // OPERANDMEM_VARIABLE
						_opdc = new OperandConst(o);
					_opdv = NULL;
					_kind = OPERANDMEM_ABSOLUTE;
					return OPERANDSTATE_UPDATED;
				}
				break;
			case OPERAND_VAR:
				{
					OperandVar& o = (OperandVar&)opd_modifier;
					switch(_kind)
					{
						_kind = OPERANDMEM_VARIABLE;
						_opdc = NULL;
						_opdv = new OperandVar(o);
						return OPERANDSTATE_UPDATED;
					}
				}
				break;
			case OPERAND_MEM:
				// double indirection yay
				// TODO: try and identify value of [...] and call updateVal with this new value instead of giving up
				return OPERANDSTATE_UNCHANGED;
			case OPERAND_ARITHEXPR: // this is a frequent case
				{
					// OperandArithExpr& o = (OperandArithExpr&)opd_modifier;
					Operand* o_new;
					OperandArithExpr o_relative = (_kind == OPERANDMEM_RELATIVE) ?
						OperandArithExpr(ARITHOPR_ADD, opd_modifier, *_opdc) :
						OperandArithExpr((OperandArithExpr&)opd_modifier);

					if(Option<Operand*> o_simplified = o_relative.simplify())
					{
						String before_simplification = _ << o_relative;
						o_new = *o_simplified;
						DBG(COLOR_IPur DBG_SEPARATOR " " COLOR_Blu DBG_SEPARATOR COLOR_Cya "%% (" << before_simplification << ") -> (" << *o_new << ")")
					}
					else
						o_new = &o_relative;
					
					switch(o_new->kind())
					{
						case OPERAND_CONST:
							_kind = OPERANDMEM_ABSOLUTE;
							_opdc = (OperandConst*)o_new->copy();
							_opdv = NULL;
							return OPERANDSTATE_UPDATED;
						case OPERAND_VAR:
							_kind = OPERANDMEM_RELATIVE;
							_opdc = NULL;
							_opdv = (OperandVar*)o_new->copy();
							return OPERANDSTATE_UPDATED;
						case OPERAND_ARITHEXPR:
							OperandArithExpr* o_arithexpr = (OperandArithExpr*) o_new;
							operand_kind_t left_kind = o_arithexpr->leftOperand().kind(), right_kind = o_arithexpr->rightOperand().kind();
							if(o_arithexpr->opr() == ARITHOPR_SUB) // try to reduce (t1 - 4) to (t1 + -4)
							{	// TODO! we _NEED_ to try to identify t2 in the case (t1 - t2), that is the most realistic case!
								if(left_kind == OPERAND_VAR && right_kind == OPERAND_CONST)
								{
									_kind = OPERANDMEM_RELATIVE;
									t::int32 new_value = ((const OperandConst&)(o_arithexpr->rightOperand())).value() * -1;
									_opdc = new OperandConst(new_value);
									_opdv = (OperandVar*  )(o_arithexpr->leftOperand().copy());
									return OPERANDSTATE_UPDATED;
								}
								else break;
							}
							if(o_arithexpr->opr() != ARITHOPR_ADD) // do not accept [ ¤ ] when ¤ is not +
								break;
							if(left_kind == OPERAND_ARITHEXPR || right_kind == OPERAND_ARITHEXPR) // do not accept [ (.+.)+. ]
								break;
							if(left_kind == right_kind) // two consts or two vars, invalid
								break;
							_kind = OPERANDMEM_RELATIVE;
							if(left_kind == OPERAND_CONST)
							{
								assert(right_kind == OPERAND_VAR);
								_opdc = (OperandConst*)(o_arithexpr->leftOperand().copy());
								_opdv = (OperandVar*  )(o_arithexpr->rightOperand().copy());
							}
							else
							{
								assert(right_kind == OPERAND_CONST);
								_opdc = (OperandConst*)(o_arithexpr->rightOperand().copy());
								_opdv = (OperandVar*  )(o_arithexpr->leftOperand().copy());
							}
							return OPERANDSTATE_UPDATED;
					}
					return OPERANDSTATE_INVALID;
				}
				break;
		}
		return OPERANDSTATE_UPDATED;
	}
	return OPERANDSTATE_UNCHANGED; // no match
}*/
Option<OperandConst> OperandMem::evalConstantOperand() const { return none; }
Option<Operand*> OperandMem::simplify() { return none; } // TODO: simplify within the [ ], makes more sense even tho it shouldn't be very useful
Option<Operand*> OperandMem::replaceConstants(const ConstantVariablesSimplified& constants) { return none; } // TODO! we really should handle this
 
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
OperandArithExpr::OperandArithExpr(const OperandArithExpr& opd)
	: _opr(opd._opr)
{
	opd1 = opd.opd1->copy();
	opd2 = opd.opd2->copy();
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
		if(opd1->kind() == OPERAND_ARITHEXPR)
			out << "(" << *opd1 << ")";
		else
			out << *opd1;
	}
	else
	{
		if(opd1->kind() == OPERAND_ARITHEXPR)
			out << "(" << *opd1 << ")";
		else
			out << *opd1;
		out << " " << _opr << " ";
		if(opd2->kind() == OPERAND_ARITHEXPR)
			out << "(" << *opd2 << ")";
		else
			out << *opd2;
	}
	return out;
}
bool OperandArithExpr::operator==(const Operand& o) const
{
	if(o.kind() == kind())
	{
		OperandArithExpr& o_arith = (OperandArithExpr&)o; // Force conversion
		return (_opr == o_arith._opr) && (*opd1 == *(o_arith.opd1)) && (isUnary() || *opd2 == *(o_arith.opd2));
	}
	else
		return false; // Operand types are not matching
}
unsigned int OperandArithExpr::countTempVars() const
{
	if(isUnary())
		return opd1->countTempVars();
	return opd1->countTempVars() + opd2->countTempVars();
}
bool OperandArithExpr::getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const
{
	expr = this->copy();
	return false;
}
bool OperandArithExpr::involvesVariable(const OperandVar& opdv) const
{
	if(isUnary())
		return opd1->involvesVariable(opdv);
	return opd1->involvesVariable(opdv) || opd2->involvesVariable(opdv);
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
// TODO: unary fix (is this already fixed?)
operand_state_t OperandArithExpr::updateVar(const OperandVar& opdv, const Operand& opd_modifier)
{
	operand_state_t rtn = OPERANDSTATE_UNCHANGED;
	if(*opd1 == opdv)
	{
		opd1 = opd_modifier.copy();
		rtn = OPERANDSTATE_UPDATED;
	}
	else
	{
		operand_state_t r = opd1->updateVar(opdv, opd_modifier);
		if(r > rtn)
			rtn = r;
	}
	if(isUnary())
		return rtn; // stop here

	if(*opd2 == opdv)
	{
		opd2 = opd_modifier.copy();
		rtn = OPERANDSTATE_UPDATED;
	}
	else
	{
		operand_state_t r = opd2->updateVar(opdv, opd_modifier);
		if(r > rtn)
			rtn = r;
	}
	
	return rtn;
}
// An ArithExpr can be const if all of its children are const!
Option<OperandConst> OperandArithExpr::evalConstantOperand() const
{
	if(isUnary())
	{
		if(Option<OperandConst> val = opd1->evalConstantOperand())
		{
			switch(_opr)
			{
				case ARITHOPR_NEG:
					return some(OperandConst(-1 * (*val).value()));
				default:
					assert(false);
			}
		}
	}
	else if(Option<OperandConst> val1 = opd1->evalConstantOperand())
	{
		if(Option<OperandConst> val2 = opd2->evalConstantOperand())
		{
			switch(_opr)
			{
				case ARITHOPR_ADD:
					return some(OperandConst((*val1).value() + (*val2).value()));
				case ARITHOPR_SUB:
					return some(OperandConst((*val1).value() - (*val2).value()));
				case ARITHOPR_MUL:
					return some(OperandConst((*val1).value() * (*val2).value()));
				case ARITHOPR_DIV:
					if((*val2).value() == 0)
						return none;
					return some(OperandConst((*val1).value() / (*val2).value()));
				case ARITHOPR_MOD:
					if((*val2).value() == 0)
						return none;
					return some(OperandConst((*val1).value() % (*val2).value()));
				case ARITHOPR_CMP:
					return none; // This can't be evaluated (wouldn't make much sense if this case was matched after the if anyway)
				default:
					assert(false);
			}
		}
	}
	return none; // one of the operands is not constant or we failed to evaluate it
}
// Warning: Option=none does not warrant that nothing has been simplified!
Option<Operand*> OperandArithExpr::simplify()
{
	// before anything, test our groudness
	if(Option<OperandConst> val = evalConstantOperand())
	{
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
	bool opd1_is_constant = opd1->kind() == OPERAND_CONST;
	bool opd2_is_constant = opd2->kind() == OPERAND_CONST;
	t::int32 opd1_val, opd2_val;
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
		case ARITHOPR_DIV:
		case ARITHOPR_MOD:
			if(opd1_is_constant && opd1_val == 1)
				return some(opd2->copy()); // [x / x */% 1]
			break;
		case ARITHOPR_CMP:
			return none;
		default:
			ASSERT(false); // unary operators should have been handled earlier
	}
	// additional tests
	// TODO: test [x + y / x - -y] and vice-versa
	switch(_opr)
	{
		case ARITHOPR_ADD:
			if(opd1->kind() == OPERAND_ARITHEXPR && ((OperandArithExpr*)opd1)->opr() == ARITHOPR_NEG)
				return ((Operand*) new OperandArithExpr(ARITHOPR_SUB, *opd2, ((OperandArithExpr*)opd1)->leftOperand()))->simplify(); // [y - x / -x + y]
			if(opd2->kind() == OPERAND_ARITHEXPR && ((OperandArithExpr*)opd2)->opr() == ARITHOPR_NEG)
				return ((Operand*) new OperandArithExpr(ARITHOPR_SUB, *opd1, ((OperandArithExpr*)opd2)->leftOperand()))->simplify(); // [x - y / -y + x]
			break;
		case ARITHOPR_SUB:
			if(opd1_is_constant && opd1_val == 0)
				return some((Operand*) new OperandArithExpr(ARITHOPR_NEG, *opd2)); // [-x / 0 - x]
			if(*opd1 == *opd2)
				return some((Operand*) new OperandConst(0)); // [0 / x - x]
			break;
		case ARITHOPR_MUL:
			if(opd2_is_constant && opd2_val == 0)
				return some((Operand*) new OperandConst(0)); // [0 / x*0]
		case ARITHOPR_DIV:
		case ARITHOPR_MOD:
			if(opd1_is_constant && opd1_val == 0) 
				return some((Operand*) new OperandConst(0)); // [0 / 0*/%x]
			break;
		case ARITHOPR_CMP:
			return none;
		default:
			ASSERT(false); // unary operators should have been handled earlier
	}
	return none;
}
Option<Operand*> OperandArithExpr::replaceConstants(const ConstantVariablesSimplified& constants)
{
	if(Option<Operand*> o = opd1->replaceConstants(constants))
		opd1 = *o;
	if(Option<Operand*> o = opd2->replaceConstants(constants))
		opd2 = *o;
	return none;
}

io::Output& operator<<(io::Output& out, operand_kind_t kind)
{
	switch(kind)
	{		
		case OPERAND_CONST:
			out << "(CONST)";
			break;
		case OPERAND_VAR:
			out << "(VAR)";
			break;
		case OPERAND_MEM:
			out << "(MEM)";
			break;
		case OPERAND_ARITHEXPR:
			out << "(ARITHEXPR)";
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
io::Output& operator<<(io::Output& out, operand_state_t state)
{
	switch(state)
	{
		case OPERANDSTATE_UNCHANGED:
			out << "(UNCHANGED)";
			break;
		case OPERANDSTATE_UPDATED:
			out << "(UPDATED)";
			break;
		case OPERANDSTATE_INVALID:
			out << "(INVALID)";
			break;
	}
	return out;
}
