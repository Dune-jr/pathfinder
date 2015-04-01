#ifndef _PREDICATE_H
#define _PREDICATE_H

#include <elm/io/Output.h>
#include <cvc4/expr/expr.h>
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
public:
	Predicate(condoperator_t opr, const Operand& opd1, const Operand& opd2);
	Predicate(const Predicate& p);
	~Predicate();
	
	inline condoperator_t opr() const { return _opr; }
	inline Operand& leftOperand() const { return *_opd1; }
	inline Operand& rightOperand() const { return *_opd2; }
	
	inline bool isIdent() const { return (*_opd1 == *_opd2) && _opr == CONDOPR_EQ; }
	inline bool isComplete() const { return _opd1->isComplete() && _opd2->isComplete(); }
	Predicate* copy() const;
	int involvesVariable(const OperandVar& opdv) const;
	bool involvesMemoryCell(const OperandMem& opdm) const;
	bool involvesMemory() const;
	bool isAffine(const OperandVar& opdv, const OperandVar& sp) const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const;
	bool update(const Operand& opd, const Operand& opd_modifier);
		
	Predicate& operator=(const Predicate& p);
	bool operator==(const Predicate& p) const;
	inline bool operator!=(const Predicate& p) const { return !(*this == p); }
	friend io::Output& operator<<(io::Output& out, const Predicate& p);
	
private:
	condoperator_t _opr; // operator
	Operand *_opd1; // left operand
	Operand *_opd2; // right operand
};

#endif
