#include <elm/io/Output.h>

#include "operand.h"
#include "ArithExpr.h"

using namespace elm;

OperandConst::OperandConst(t::uint32 value) : value(value) {}
io::Output& OperandConst::print(io::Output& out) const { out << value; return out; }
bool OperandConst::operator==(const Operand2& o) const
{
	if(o.kind() == kind())
		return value == ((OperandConst&)o).value;
	else
		return false; // Operand types are not matching
}

OperandVar::OperandVar(t::uint32 addr) : addr(addr) {}
io::Output& OperandVar::print(io::Output& out) const { out << addr; return out; }
bool OperandVar::operator==(const Operand2& o) const
{
	if(o.kind() == kind())
		return addr == ((OperandVar&)o).addr;
	else
		return false; // Operand types are not matching
}

OperandArithExpr::OperandArithExpr(arithoperator_t opr, Operand2& opd1, Operand2& opd2)
	: opr(opr), opd1(opd1), opd2(opd2) {}
	
io::Output& OperandArithExpr::print(io::Output& out) const
{
	switch(opr)
	{
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
		default:
			out << "???";	
	}
	return out;
}

bool OperandArithExpr::operator==(const Operand2& o) const
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

/* ------------ Old junk ------------  */
bool Operand::operator==(const Operand& opd) const
{
	if (kind != opd.kind)
		return false;
	switch(kind)
	{
		case OPERAND_CONST:
			return (*((int*)value) == *((int*)opd.value));
			break;
		case OPERAND_VAR:
			return (*((unsigned int*)value) == *((unsigned int*)opd.value));
			break;
		case OPERAND_ARITHEXPR:
			return (*((ArithExpr*)value) == *((ArithExpr*)opd.value));
			return true;
			break;
		default:
			return false;
	}
}

io::Output& operator<<(io::Output& out, const Operand& opd)
{
	switch(opd.kind)
	{
		case OPERAND_CONST:
			out << *((int*)opd.value);
			break;
		case OPERAND_VAR:
			out << "@0x" << io::hex(io::uppercase(*((unsigned int*)opd.value)));
			break;
		case OPERAND_ARITHEXPR:
			out << *((ArithExpr*)opd.value);
			break;
		default:
			out << "???";
	}
	return out;
}

Operand::Operand() { kind = OPERAND_UNDEF; } // Manually set kind to undef (unnecessary)
Operand::Operand(operand_kind_t kind, void* value) : kind(kind), value(value) {}
Operand::Operand(int value) // kind: CONST
{
	int* pValue = new(int);
	*pValue = value;
	this->kind = OPERAND_CONST;
	this->value = (void*)pValue;
}

Operand::Operand(unsigned int value) // kind: VAR
{
	unsigned int* pValue = new(unsigned int);
	*pValue = value;
	this->kind = OPERAND_VAR;
	this->value = (void*)pValue;
}
Operand::Operand(ArithExpr* value) // kind: ARITHEXPR
{
	this->kind = OPERAND_ARITHEXPR;
	this->value = (void*)value;
}
