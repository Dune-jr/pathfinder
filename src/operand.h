#ifndef _OPERAND_H
#define _OPERAND_H

#include <elm/io.h>
#include <elm/io/Output.h>

using namespace elm;

enum arithoperator_t
{
	// Unary
	ARITHOPR_NEG, // (-)   Arithmetic negation
	// ARITHOPR_NOT, // TODO (later): should we implement logical negation?
	
	// Binary
	ARITHOPR_ADD, // (+)   Addition
	ARITHOPR_SUB, // (-)   Substraction
	ARITHOPR_MUL, // (*)   Multiplication
	ARITHOPR_DIV, // (/)   Division
	ARITHOPR_MOD, // (mod) Modulo
	// TODO (later): Should we add logical operators? (or, and, etc...)
};

enum operand_kind_t
{
	OPERAND_UNDEF,     // Undefined
	OPERAND_CONST,     // Constant
	OPERAND_VAR,       // Variable
	OPERAND_ARITHEXPR, // Arithmetic Expression
};

// Abstract Operand class
class Operand
{
private:
	virtual io::Output& print(io::Output& out) const = 0;
	
public:
	virtual operand_kind_t kind() const = 0;
	virtual bool operator==(const Operand& o) const = 0;
	friend io::Output& operator<<(io::Output& out, const Operand& o) { return o.print(out); }
};

// Constant values
class OperandConst : public Operand
{
private:
	t::uint32 value;
	io::Output& print(io::Output& out) const;

public:
	OperandConst(t::uint32 value);
	
	inline operand_kind_t kind() const { return OPERAND_CONST; }
	bool operator==(const Operand& o) const;
	// TODO: This is... the opposite of pretty
	friend io::Output& operator<<(io::Output& out, const OperandConst& o) { return o.print(out); }
};

// Variables
class OperandVar : public Operand
{
private:
	t::uint32 addr;
	io::Output& print(io::Output& out) const;
	
public:
	OperandVar(t::uint32 addr);
	
	inline operand_kind_t kind() const { return OPERAND_VAR; }
	bool operator==(const Operand& o) const;
	friend io::Output& operator<<(io::Output& out, const OperandVar& o) { return o.print(out); }
};

// Arithmetic Expressions
class OperandArithExpr : public Operand
{
private:
	arithoperator_t opr;
	Operand& opd1;
	Operand& opd2; // unused if operator is unary	
	io::Output& print(io::Output& out) const;
	
public:
	OperandArithExpr(arithoperator_t opr, Operand& opd1, Operand& opd2);
	bool isUnary() const;
	bool isBinary() const;
	
	inline operand_kind_t kind() const { return OPERAND_ARITHEXPR; }
	bool operator==(const Operand& o) const;
	friend io::Output& operator<<(io::Output& out, const OperandArithExpr& o) { return o.print(out); }
};

#endif
