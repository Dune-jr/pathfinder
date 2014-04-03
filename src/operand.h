#ifndef _OPERAND_H
#define _OPERAND_H

#include <elm/io.h>
#include <elm/io/Output.h>

using namespace elm;

enum arithoperator_t
{
	// Unary
	ARITHOPR_NEG, // (-)   Arithmetic negation
	// ARITHOPR_NOT, // Should we implement bit inversion? // actually it's just 2^n - x - 1 !
	
	// Binary
	ARITHOPR_ADD, // (+)   Addition
	ARITHOPR_SUB, // (-)   Substraction
	ARITHOPR_MUL, // (*)   Multiplication
	ARITHOPR_DIV, // (/)   Division
	ARITHOPR_MOD, // (mod) Modulo
	// Should we add bit shifting operators? // Not needed, we can represent this with the existing
	//   operators, it's just a * 2^n, n being a constant it can be reduced
	// Should we add logical operators? (or, and, etc...)
	
	ARITHOPR_CMP, // (~) Special operator to be used with CONDOPR_EQ and a register, that indicates
                  //     that it contains information over the comparison of two other registers
};

enum operand_kind_t
{
	OPERAND_CONST,     // Constant
	OPERAND_VAR,       // Variable (register or addr, for now)
	OPERAND_ARITHEXPR, // Arithmetic Expression
};

class OperandConst;
class OperandVar;

// Abstract Operand class
class Operand
{
private:
	virtual io::Output& print(io::Output& out) const = 0;
	
public:
	virtual Operand* copy() const = 0;
	virtual bool isInvolvedVariable(const OperandVar& opdv) const = 0;
	virtual bool evalConstantOperand(OperandConst& val) const = 0;
	virtual bool updateVar(const OperandVar& opdv, const Operand& opd_modifier) = 0;
	virtual operand_kind_t kind() const = 0;
	virtual bool operator==(const Operand& o) const = 0;
	friend io::Output& operator<<(io::Output& out, const Operand& o) { return o.print(out); }
};

// Constant values
class OperandConst : public Operand
{
private:
	t::int32 _value;
	io::Output& print(io::Output& out) const;

public:
	OperandConst(const OperandConst& opd);
	OperandConst(t::int32 value);
	
	inline t::int32 value() { return _value; }
	
	Operand* copy() const;
	bool isInvolvedVariable(const OperandVar& opdv) const;
	bool evalConstantOperand(OperandConst& val) const;
	bool updateVar(const OperandVar& opdv, const Operand& opd_modifier);
	inline operand_kind_t kind() const { return OPERAND_CONST; }
	bool operator==(const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandConst& o) { return o.print(out); }
};

// Variables
class OperandVar : public Operand
{
private:
	t::int32 addr;
	io::Output& print(io::Output& out) const;
	
public:
	OperandVar(const OperandVar& opd);
	OperandVar(t::int32 addr);
	
	Operand* copy() const;
	bool isInvolvedVariable(const OperandVar& opdv) const;
	bool evalConstantOperand(OperandConst& val) const;
	bool updateVar(const OperandVar& opdv, const Operand& opd_modifier);
	inline operand_kind_t kind() const { return OPERAND_VAR; }
	bool operator==(const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandVar& o) { return o.print(out); }
};

// Arithmetic Expressions
class OperandArithExpr : public Operand
{
private:
	arithoperator_t opr;
	Operand* opd1;
	Operand* opd2; // unused if operator is unary	
	io::Output& print(io::Output& out) const;
public:
	OperandArithExpr(arithoperator_t opr, const Operand& opd1, const Operand& opd2); // TODO: shouldn't this be const Operand& opd1?
	OperandArithExpr(const OperandArithExpr& opd);
	
	bool isUnary() const;
	bool isBinary() const;
	
	Operand* copy() const;
	bool isInvolvedVariable(const OperandVar& opdv) const;
	bool evalConstantOperand(OperandConst& val) const;
	bool updateVar(const OperandVar& opdv, const Operand& opd_modifier);
	inline operand_kind_t kind() const { return OPERAND_ARITHEXPR; }
	bool operator==(const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandArithExpr& o) { return o.print(out); }
};

#endif
