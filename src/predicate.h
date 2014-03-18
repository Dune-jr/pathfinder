#ifndef _PREDICATE_H
#define _PREDICATE_H

// #include <list>
#include "operand.h"
#include "ArithExpr.h"

enum condoperator_t
{
	// Unary
	// NOT, // !

	// Binary
	CONDOPR_LT, // <
	CONDOPR_LE, // <=
	CONDOPR_EQ, // =
	CONDOPR_NE, // !=
};

std::ostream& operator<<(std::ostream& out, const condoperator_t& opr);

class Predicate
{
protected:
	condoperator_t opr; // operator
	Operand opd1;
	Operand opd2;

public:
	Predicate();
	Predicate(condoperator_t opr, Operand opd1, Operand opd2 = Operand());
	bool isUnary() const; // No unary conditions?
	bool isBinary() const;
	
	bool operator== (const Predicate& p) const;
	friend std::ostream& operator<<(std::ostream& out, const Predicate& p);
};

#endif

