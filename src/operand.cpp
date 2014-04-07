#include <elm/io/Output.h>
#include "operand.h"
#include "debug.h"

using namespace elm;

// Operands: Constants
OperandConst::OperandConst(t::int32 value) : _value(value) { }
OperandConst::OperandConst(const OperandConst& opd) : _value(opd._value) { }
Operand* OperandConst::copy() const { return new OperandConst(_value); }
io::Output& OperandConst::print(io::Output& out) const
{
	out << "0x" << io::hex(_value);
	return out;
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
//bool OperandConst::evalConstantOperand(OperandConst& val) const { val = *this; return true; }
bool OperandConst::updateVar(const OperandVar& opdv, const Operand& opd_modifier) { return false; }

// Operands: Variables
OperandVar::OperandVar(t::int32 addr) : addr(addr) { }
OperandVar::OperandVar(const OperandVar& opd) : addr(opd.addr) { }
Operand* OperandVar::copy() const { return new OperandVar(addr); }
io::Output& OperandVar::print(io::Output& out) const
{
	if(!isTempVar())
		out << "?" << addr; // register
	else
		out << "t" << -addr; // temporary
	return out; 
}

bool OperandVar::operator==(const Operand& o) const
{	
	if(o.kind() == kind())
		return addr == ((OperandVar&)o).addr;
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
//bool OperandVar::evalConstantOperand(OperandConst& val) const { return false; }
// since the parent has to do the modification, and var has no child, return false
bool OperandVar::updateVar(const OperandVar& opdv, const Operand& opd_modifier) { return false; }

// Operands: Arithmetic Expressions
OperandArithExpr::OperandArithExpr(arithoperator_t opr_, const Operand& opd1_, const Operand& opd2_)
	: opr(opr_)
{
	opd1 = opd1_.copy();
	opd2 = opd2_.copy();
}
OperandArithExpr::OperandArithExpr(const OperandArithExpr& opd)
	: opr(opd.opr)
{
	opd1 = opd.opd1->copy();
	opd2 = opd.opd2->copy();
}
Operand* OperandArithExpr::copy() const { return new OperandArithExpr(opr, *opd1, *opd2); }
io::Output& OperandArithExpr::print(io::Output& out) const
{
	switch(opr) {
		case ARITHOPR_NEG:
			out << "-(" << *opd1 << ")";
			break;
		case ARITHOPR_ADD:
			out << "(" << *opd1 << " + " << *opd2 << ")";
			break;
		case ARITHOPR_SUB:
			out << "(" << *opd1 << " - " << *opd2 << ")";
			break;
		case ARITHOPR_MUL:
			out << "(" << *opd1 << " * " << *opd2 << ")";
			break;
		case ARITHOPR_DIV:
			out << "(" << *opd1 << " * " << *opd2 << ")";
			break;
		case ARITHOPR_MOD:
			out << "(" << *opd1 << " mod " << *opd2 << ")";
			break;
		case ARITHOPR_CMP:
			out << "(" << *opd1 << " ~ " << *opd2 << ")";
			break;
		default:
			out << "??opr??";
	}
	return out;
}
bool OperandArithExpr::operator==(const Operand& o) const
{
	if(o.kind() == kind())
	{
		OperandArithExpr& o_arith = (OperandArithExpr&)o; // Force conversion
		return (opr == o_arith.opr) && (*opd1 == *(o_arith.opd1)) && (*opd2 == *(o_arith.opd2));
	}
	else
		return false; // Operand types are not matching
}
unsigned int OperandArithExpr::countTempVars() const { return opd1->countTempVars() + opd2->countTempVars(); }
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
/*
// An ArithExpr can be const if all of its children are const!
bool OperandArithExpr::evalConstantOperand(OperandConst& val) const
{
	OperandConst val1(0);
	OperandConst val2(0);
	if(isUnary())
	{
		if(opd1->evalConstantOperand(val1))
		{
			switch(opr)
			{
				case ARITHOPR_NEG:
					val = OperandConst(-1 * val1.value());
					break;
				default:
					assert(false);
			}
			return true;
		}
	}
	else if (opd1->evalConstantOperand(val1) && opd2->evalConstantOperand(val2))
	{
		switch(opr)
		{
			case ARITHOPR_ADD:
				val = OperandConst(val1.value() + val2.value());
				break;
			case ARITHOPR_SUB:
				val = OperandConst(val1.value() - val2.value());
				break;
			case ARITHOPR_MUL:
				val = OperandConst(val1.value() * val2.value());
				break;
			case ARITHOPR_DIV:
				if(val2.value() == 0)
					return false; // TODO hugues: should we raise some alert that there is a bug in the assembly code?
				val = OperandConst(val1.value() / val2.value());
				break;
			case ARITHOPR_MOD:
				val = OperandConst(val1.value() % val2.value());
				break;
			case ARITHOPR_CMP:
				return false; // This can't be evaluated (wouldn't make much sense if this case was matched after the if anyway)
			default:
				assert(false);
		}
		return true;
	}
	return false; // One of the operands includes a variable so we cannot properly evaluate it
}
*/
bool OperandArithExpr::updateVar(const OperandVar& opdv, const Operand& opd_modifier)
{
	bool rtn = false;
	if(*opd1 == opdv)
	{
		opd1 = opd_modifier.copy();
		rtn = true;
	}
	else
		rtn |= opd1->updateVar(opdv, opd_modifier);
		
	if(*opd2 == opdv)
	{
		opd2 = opd_modifier.copy();
		rtn = true;
	}
	else
		rtn |= opd2->updateVar(opdv, opd_modifier);
	
	return rtn;
}
bool OperandArithExpr::isUnary() const { return opr < ARITHOPR_ADD; }
bool OperandArithExpr::isBinary() const { return opr >= ARITHOPR_ADD; }
