#ifndef _PREDICATE_H
#define _PREDICATE_H

#include <elm/io/Output.h>
#include "operand.h"

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
	Predicate(); // for option
	Predicate(condoperator_t opr, const Operand* opd1, const Operand* opd2);
	// Predicate(condoperator_t opr, const Operand& opd1, const Operand& opd2);
	// Predicate(const Predicate& p);
	~Predicate();
	
	inline condoperator_t opr() const { return (condoperator_t)_opr; }
	inline const Operand* left() const { return _opd1; }
	inline const Operand* right() const { return _opd2; }
	inline const Operand& leftOperand() const { return *_opd1; }
	inline const Operand& rightOperand() const { return *_opd2; }
	
	inline bool isIdent() const    { return (*_opd1 == *_opd2) && _opr == CONDOPR_EQ; }
	inline bool isComplete() const { return _opd1->isComplete() && _opd2->isComplete(); }
	inline bool isConstant() const { return _opd1->isConstant() && _opd2->isConstant(); }
	inline bool isLinear() const   { return _opd1->isLinear() && _opd2->isLinear(); }
	// Predicate* copy() const;
	int involvesOperand(const Operand& opd) const;
	int involvesVariable(const OperandVar& opdv) const;
	Option<Constant> involvesStackBelow(const Constant& stack_limit) const;
	bool involvesMemoryCell(const OperandMem& opdm) const;
	bool involvesMemory() const;
	bool isAffine(const OperandVar& opdv) const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const;
	bool update(DAG& dag, const Operand* opd, const Operand* opd_modifier);

	Predicate& operator=(const Predicate& p);
	bool operator==(const Predicate& p) const;
	inline bool operator!=(const Predicate& p) const { return !(*this == p); }
	friend io::Output& operator<<(io::Output& out, const Predicate& p);
	
private:
	char _opr; // operator, on one byte to save memory
	const Operand *_opd1; // left operand
	const Operand *_opd2; // right operand
};

#endif

