#ifndef _OPERAND_H
#define _OPERAND_H

#include <elm/io.h>
#include <elm/io/Output.h> // TODO try and remove one of the two
#include <cvc4/expr/expr.h>
#include <cvc4/expr/type.h>

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
	ARITHOPR_CMP, // (~)   Special operator to be used with CONDOPR_EQ and a register that indicates
                  //       that it contains information over the comparison of two other registers
	// Should we add bit shifting operators? // Not needed, we can represent this with the existing
	//   operators, it's just a * 2^n, n being a constant it can be reduced
	// Should we add logical operators? (or, and, etc...)
};

enum operand_kind_t
{
	OPERAND_CONST,     // Constant value
	OPERAND_VAR,       // Variable (register or temporary variable, for now)
	OPERAND_ARITHEXPR, // Arithmetic Expression
};

// Abstract Operand class
class Operand
{	
public:
	virtual Operand* copy() const = 0;
	virtual unsigned int countTempVars() const = 0; // this will count a variable several times if it occurs several times
	virtual bool getIsolatedTempVar(class OperandVar& temp_var, Operand*& expr) const = 0;
	virtual bool involvesVariable(const class OperandVar& opdv) const = 0;
	virtual bool updateVar(const class OperandVar& opdv, const Operand& opd_modifier) = 0;
	virtual operand_kind_t kind() const = 0;
	virtual bool isComplete() const = 0;
	virtual void accept(class OperandVisitor& visitor) const = 0;
	
	virtual bool operator==(const Operand& o) const = 0;
	friend io::Output& operator<<(io::Output& out, const Operand& o) { return o.print(out); }
	
private:
	virtual io::Output& print(io::Output& out) const = 0;
};

// The visitor: an abstract class
class OperandVisitor
{
public:
	virtual void visit(const class OperandConst& o) = 0;
	virtual void visit(const class OperandVar& o) = 0;
	virtual void visit(const class OperandArithExpr& o) = 0;
};

// Constant values
class OperandConst : public Operand
{
public:
	OperandConst(const OperandConst& opd);
	OperandConst(t::int32 value);
	
	inline t::int32 value() const { return _value; }
	
	Operand* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const;
	bool involvesVariable(const OperandVar& opdv) const;
	bool updateVar(const OperandVar& opdv, const Operand& opd_modifier);
	inline operand_kind_t kind() const { return OPERAND_CONST; }
	inline bool isComplete() const { return true; }
	inline void accept(OperandVisitor& visitor) const { visitor.visit(*this); }
	
	bool operator==(const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandConst& o) { return o.print(out); }
	
private:
	t::int32 _value;
	io::Output& print(io::Output& out) const;
};

// Variables
class OperandVar : public Operand
{	
public:
	OperandVar(const OperandVar& opd);
	OperandVar(t::int32 addr);
	
	inline t::int32 getAddr() const { return addr; }
	inline bool isTempVar() const { return addr < 0; }
	
	Operand* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const;
	bool involvesVariable(const OperandVar& opdv) const;
	bool updateVar(const OperandVar& opdv, const Operand& opd_modifier);
	inline operand_kind_t kind() const { return OPERAND_VAR; }
	inline bool isComplete() const { return true; }
	inline void accept(OperandVisitor& visitor) const { visitor.visit(*this); }
	
	bool operator==(const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandVar& o) { return o.print(out); }
	
private:
	t::int32 addr;
	io::Output& print(io::Output& out) const;
};

// Arithmetic Expressions
class OperandArithExpr : public Operand
{
public:
	OperandArithExpr(arithoperator_t opr, const Operand& opd1_, const Operand& opd2_);
	OperandArithExpr(const OperandArithExpr& opd);
	
	inline arithoperator_t opr() const { return _opr; }
	inline const Operand& leftOperand() const { return *opd1; }
	inline const Operand& rightOperand() const { assert(isBinary()); return *opd2; }
	
	bool isUnary() const;
	bool isBinary() const;
	
	Operand* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const;
	bool involvesVariable(const OperandVar& opdv) const;
	bool updateVar(const OperandVar& opdv, const Operand& opd_modifier);
	inline operand_kind_t kind() const { return OPERAND_ARITHEXPR; }
	inline bool isComplete() const { return _opr != ARITHOPR_CMP && opd1->isComplete() && opd2->isComplete(); }
	inline void accept(OperandVisitor& visitor) const { visitor.visit(*this); }
	
	bool operator==(const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandArithExpr& o) { return o.print(out); }
	
private:
	arithoperator_t _opr;
	Operand* opd1;
	Operand* opd2; // unused if operator is unary
		
	io::Output& print(io::Output& out) const;
};

io::Output& operator<<(io::Output& out, operand_kind_t kind);
io::Output& operator<<(io::Output& out, arithoperator_t opr);

#endif
