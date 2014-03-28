#ifndef _PREDICATE_H
#define _PREDICATE_H

#include <elm/io/Output.h>
#include "operand.h"

using namespace elm;

// Conditional operator enum
enum condoperator_t
{
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
public: // TODO
	condoperator_t _opr; // operator
	Operand *_opd1; // left operand
	Operand *_opd2; // right operand

public:
	// Predicate(const Predicate& p): opr(p.opr), opd1(p.opd1), opd2(p.opd2) { } // TODO: Casse
	Predicate(condoperator_t opr, Operand& opd1, Operand& opd2);
	
	bool operator== (const Predicate& p) const;
	friend io::Output& operator<<(io::Output& out, const Predicate& p);
};

#endif

