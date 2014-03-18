#include "operand.h"
#include "ArithExpr.h"

bool Operand::operator==(const Operand& opd) const
{
	if (kind != opd.kind)
		return false;
	switch(kind)
	{
		case CONST:
			return (*((int*)value) == *((int*)opd.value));
			break;
		case VAR:
			return (*((unsigned int*)value) == *((unsigned int*)opd.value));
			break;
		case EXPR:
			return (*((ArithExpr*)value) == *((ArithExpr*)opd.value));
			return true;
			break;
		default:
			return false;
	}
}

std::ostream& operator<<(std::ostream& out, const Operand& opd)
{
	switch(opd.kind)
	{
		case CONST:
			out << *((int*)opd.value);
			break;
		case VAR:
			out << std::hex << std::uppercase; // Turn on caps hex mode (ex: 0xA3BFF)
			out << "@0x" << *((unsigned int*)opd.value);
			out << std::nouppercase << std::dec; // Turn off caps hex mode
			break;
		case EXPR:
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
	this->kind = CONST;
	this->value = (void*)pValue;
}

// TODO: Okay, this is a hack, but i think we're not gonna using int in the end, 
// so let's not waste energy in merging this constructor with the CONST one
Operand::Operand(unsigned int value) // kind: VAR
{
	// TODO: maybe add a label in here later on
	unsigned int* pValue = new(unsigned int);
	*pValue = value;
	this->kind = VAR;
	this->value = (void*)pValue;
}
Operand::Operand(ArithExpr* value) // kind: ARITHEXPR
{
	this->kind = EXPR;
	this->value = (void*)value;
}
