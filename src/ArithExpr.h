#ifndef _ARITHEXPR_H
#define _ARITHEXPR_H

#include "operand.h"

enum arithoperator_t
{
	// Unary
	ARITHOPR_NEG,
	// ARITHOPR_NOT, // TODO: should we implement logical negation?
	
	// Binary
	ARITHOPR_ADD,
	ARITHOPR_SUB,
	ARITHOPR_MUL,
	ARITHOPR_DIV,
	ARITHOPR_MOD,
	// TOOD: Should we add logical operators? (or, and, etc...)
};

class ArithExpr
{
protected:
	arithoperator_t opr;
	Operand opd1;
	Operand opd2; // may be unused
	
public:
	ArithExpr();
	ArithExpr(arithoperator_t opr, Operand opd1, Operand opd2 = Operand());
	bool operator==(const ArithExpr& a) const;
	bool isUnary() const;
	bool isBinary() const;
	friend std::ostream& operator<<(std::ostream& out, const ArithExpr& a);
};

#endif 
