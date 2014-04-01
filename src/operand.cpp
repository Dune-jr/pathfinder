#include <elm/io/Output.h>
#include "operand.h"
#include "debug.h"

using namespace elm;

// Operands: Constants
OperandConst::OperandConst(t::int32 value) : value(value) { }
OperandConst::OperandConst(const OperandConst& opd) : value(opd.value) { }
Operand* OperandConst::copy() const { return new OperandConst(value); }
io::Output& OperandConst::print(io::Output& out) const
{
	out << "0x" << io::hex(value);
	return out;
}
bool OperandConst::operator==(const Operand& o) const
{
	if(o.kind() == kind())
		return value == ((OperandConst&)o).value;
	else
		return false; // Operand types are not matching
}
bool OperandConst::isInvolvedVariable(const OperandVar& opdv) const { return false; }
bool OperandConst::updateVar(const OperandVar& opdv, const Operand& opd_modifier) { return false; }

// Operands: Variables
OperandVar::OperandVar(t::int32 addr) : addr(addr) { }
OperandVar::OperandVar(const OperandVar& opd) : addr(opd.addr) { }
Operand* OperandVar::copy() const { return new OperandVar(addr); }
io::Output& OperandVar::print(io::Output& out) const
{
	if(addr >= 0)
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
bool OperandVar::isInvolvedVariable(const OperandVar& opdv) const
{
	return opdv == *this; // TODO test
}
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
bool OperandArithExpr::isInvolvedVariable(const OperandVar& opdv) const
{
	if(isUnary())
		return opd1->isInvolvedVariable(opdv);
	return opd1->isInvolvedVariable(opdv) || opd2->isInvolvedVariable(opdv);
}
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
