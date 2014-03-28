#include <elm/io/Output.h>
#include "predicate.h"

using namespace elm;

bool Predicate::operator==(const Predicate& p) const
{
	return (_opr == p._opr) && (*_opd1 == *(p._opd1)) && (*_opd2 == *(p._opd2));
}

io::Output& operator<<(io::Output& out, const condoperator_t& opr)
{	
	switch(opr) {
		case CONDOPR_LT:
			out << "<";
			break;
		case CONDOPR_LE:
			out << "<=";
			break;
		case CONDOPR_EQ:
			out << "=";
			break;
		case CONDOPR_NE:
			out << "!=";
			break;
		default:
			out << "???";
	}
	return out;
}

io::Output& operator<<(io::Output& out, const Predicate& p)
{
	// Only binary conditional operators in our implementation
	out << "(" << *(p._opd1) << " " << p._opr << " " << *(p._opd2) << ")"; // (... * ...)
	return out;
}

Predicate::Predicate(condoperator_t opr, Operand& opd1, Operand& opd2) : _opr(opr)
{
	_opd1 = opd1.copy();
	_opd2 = opd2.copy();
}
