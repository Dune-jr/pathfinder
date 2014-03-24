#include <elm/io/Output.h>
#include "operand.h"

using namespace elm;

// Operands: Constants
OperandConst::OperandConst(t::uint32 value) : value(value) {}
io::Output& OperandConst::print(io::Output& out) const { out << value; return out; }
bool OperandConst::operator==(const Operand& o) const
{
	if(o.kind() == kind())
		return value == ((OperandConst&)o).value;
	else
		return false; // Operand types are not matching
}

// Operands: Variables
OperandVar::OperandVar(t::uint32 addr) : addr(addr) {}
io::Output& OperandVar::print(io::Output& out) const
{
	out << "@0x" << io::hex(io::uppercase(addr));
	return out; 
}

bool OperandVar::operator==(const Operand& o) const
{	
	if(o.kind() == kind())
		return addr == ((OperandVar&)o).addr;
	else
		return false; // Operand types are not matching
}

// Operands: Arithmetic Expressions
OperandArithExpr::OperandArithExpr(arithoperator_t opr, Operand& opd1, Operand& opd2)
	: opr(opr), opd1(opd1), opd2(opd2) {}
io::Output& OperandArithExpr::print(io::Output& out) const
{
	switch(opr) {
		case ARITHOPR_NEG:
			out << "-(" << opd1 << ")";
			break;
		case ARITHOPR_ADD:
			out << "(" << opd1 << " + " << opd2 << ")";
			break;
		case ARITHOPR_SUB:
			out << "(" << opd1 << " - " << opd2 << ")";
			break;
		case ARITHOPR_MUL:
			out << "(" << opd1 << " * " << opd2 << ")";
			break;
		case ARITHOPR_DIV:
			out << "(" << opd1 << " * " << opd2 << ")";
			break;
		case ARITHOPR_MOD:
			out << "(" << opd1 << " mod " << opd2 << ")";
			break;
		case ARITHOPR_CMP:
			out << "(" << opd1 << " ~ " << opd2 << ")";
			break;
		default:
			out << "???";	
	}
	return out;
}
bool OperandArithExpr::operator==(const Operand& o) const
{
	if(o.kind() == kind())
	{
		OperandArithExpr& o_arith = (OperandArithExpr&)o; // Force conversion
		return (opr == o_arith.opr) && (opd1 == o_arith.opd1) && (opd2 == o_arith.opd2);
	}
	else
		return false; // Operand types are not matching
}
bool OperandArithExpr::isUnary() const { return opr < ARITHOPR_ADD; }
bool OperandArithExpr::isBinary() const { return opr >= ARITHOPR_ADD; }
