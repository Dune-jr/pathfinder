#include <elm/io/Output.h>
#include "predicate.h"

using namespace elm;

bool Predicate::operator==(const Predicate& p) const
{
	return (opr == p.opr) && (opd1 == p.opd1) && (opd2 == p.opd2);
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
	out << "(" << p.opd1 << " "  << p.opr << " " << p.opd2 << ")"; // (... * ...)
	return out;
}

Predicate::Predicate(condoperator_t opr, Operand& opd1, Operand& opd2)
	: opr(opr), opd1(&opd1), opd2(&opd2) { }

