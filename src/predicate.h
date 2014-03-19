#ifndef _PREDICATE_H
#define _PREDICATE_H

#include <elm/io/Output.h>
#include "operand.h"

using namespace elm;

// Conditional operator enum
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

// Printing conditional operators
io::Output& operator<<(io::Output& out, const condoperator_t& opr);

// Predicate class
class Predicate
{
protected:
	condoperator_t opr; // operator
	Operand& opd1; // left operand
	Operand& opd2; // right operand

public:
	Predicate(condoperator_t opr, Operand& opd1, Operand& opd2);
	bool isUnary() const; // No unary conditions yet though?
	bool isBinary() const;
	
	bool operator== (const Predicate& p) const;
	friend io::Output& operator<<(io::Output& out, const Predicate& p);
};

#endif

