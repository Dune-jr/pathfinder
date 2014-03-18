#include "ArithExpr.h"
#include "operand.h" 

bool ArithExpr::operator==(const ArithExpr& a) const
{
	// Of course, we do not accept 2*x = x+x and such, we use the semantical equality (aka Leibniz equality)
	return (opr == a.opr) && (opd1 == a.opd1) && (opd2 == a.opd2);
}

std::ostream& operator<<(std::ostream& out, const ArithExpr& a)
{
	switch(a.opr)
	{
		case ARITHOPR_NEG:
			out << "-(" << a.opd1 << ")";
			break;
		case ARITHOPR_ADD:
			out << "(" << a.opd1 << " + " << a.opd2 << ")";
			break;
		case ARITHOPR_SUB:
			out << "(" << a.opd1 << " - " << a.opd2 << ")";
			break;
		case ARITHOPR_MUL:
			out << "(" << a.opd1 << " * " << a.opd2 << ")";
			break;
		case ARITHOPR_DIV:
			out << "(" << a.opd1 << " * " << a.opd2 << ")";
			break;
		case ARITHOPR_MOD:
			out << "(" << a.opd1 << " mod " << a.opd2 << ")";
			break;
		default:
			out << "???";	
	}
	return out;
}

ArithExpr::ArithExpr() {}

ArithExpr::ArithExpr(arithoperator_t opr, Operand opd1, Operand opd2)
	: opr(opr), opd1(opd1), opd2(opd2) {}

bool ArithExpr::isUnary() const { return opr < ARITHOPR_ADD; }
bool ArithExpr::isBinary() const { return opr >= ARITHOPR_ADD; }
