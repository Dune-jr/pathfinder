#ifndef _OPERAND_H
#define _OPERAND_H

#include <elm/io.h> // iostream
#include <iostream>

using namespace elm;

enum operand_kind_t
{
	OPERAND_UNDEF,     // Undefined
	OPERAND_CONST,     // Constant
	OPERAND_VAR,       // Variable
	OPERAND_ARITHEXPR, // Arithmetic Expression
};

class ArithExpr;

// Abstract class
class Operand2
{
	virtual operand_kind_t kind() const = 0;
};

class OperandConst : public Operand2
{
private:
	t::uint32 value;

public:
	OperandConst(t::uint32 value);
	inline operand_kind_t kind() const { return OPERAND_CONST; }
};

class OperandVar : public Operand2
{
private:
	t::uint32 value;
	
public:
	OperandVar(t::uint32 value);
	inline operand_kind_t kind() const { return OPERAND_VAR; }
};

class OperandArithExpr : public Operand2
{
private:
	t::uint32 value;
	
public:
	OperandArithExpr(t::uint32 value);
	inline operand_kind_t kind() const { return OPERAND_ARITHEXPR; }
};

// Operands of predicates (can be constant or variables)
class Operand
{
private:
	operand_kind_t kind;
	void* value;
	// We may have to add a "label" for the case where it is a variable, in the future
	
public:
	Operand();
	Operand(operand_kind_t kind, void* value);
	Operand(int value); // Constant
	Operand(unsigned int value); // Variable
	Operand(ArithExpr* e); // An arithmetic expression
	// TODO: we are forced here to use Operand* because Operand is incomplete type (not yet defined...)
	//       should we do it this way or the other way around? (we include arithexpr from this file)
	
	bool operator==(const Operand& p) const;
	friend std::ostream& operator<<(std::ostream& out, const Operand& o);
};

#endif
