#ifndef _PREDICATE_H
#define _PREDICATE_H

#include <elm/io/Output.h>
#include "operand.h"
#include "ArithExpr.h"

using namespace elm;

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

io::Output& operator<<(io::Output& out, const condoperator_t& opr);

class Predicate
{
protected:
	condoperator_t opr; // operator
	Operand2& opd1;
	Operand2& opd2;

public:
	Predicate(condoperator_t opr, Operand2& opd1, Operand2& opd2);
	bool isUnary() const; // No unary conditions?
	bool isBinary() const;
	
	bool operator== (const Predicate& p) const;
	friend io::Output& operator<<(io::Output& out, const Predicate& p);
};

#endif

