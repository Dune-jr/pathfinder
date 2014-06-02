#ifndef _OPERAND_H
#define _OPERAND_H

#include <elm/io/Output.h>
#include <elm/util/Option.h>
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
	OPERAND_MEM, // Arithmetic Expression
};

enum operandmem_kind_t
{
	OPERANDMEMFLAG_HASCONST=0b01,
	OPERANDMEMFLAG_HASVAR=0b10,

	OPERANDMEM_ABSOLUTE=0b01, // Const
	OPERANDMEM_VARIABLE=0b10, // Var
	OPERANDMEM_RELATIVE=0b11, // Const+Var
};

enum operand_state_t
{
	OPERANDSTATE_UNCHANGED=0,
	OPERANDSTATE_UPDATED,
	OPERANDSTATE_INVALID, // assert(INVALID > UPDATED)
};

class OperandConst;
class OperandVar;
class OperandMem;
class OperandArithExpr;

// The visitor: an abstract class
class OperandVisitor
{
public:
	virtual void visit(const OperandConst& o) = 0;
	virtual void visit(const OperandVar& o) = 0;
	virtual void visit(const OperandMem& o) = 0;
	virtual void visit(const OperandArithExpr& o) = 0;
};

// Abstract Operand class
class Operand
{	
public:
	virtual Operand* copy() const = 0;
	virtual unsigned int countTempVars() const = 0; // this will count a variable several times if it occurs several times
	virtual bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const = 0;
	virtual bool involvesVariable(const OperandVar& opdv) const = 0;
	virtual operand_state_t updateVar(const OperandVar& opdv, const Operand& opd_modifier) = 0;
	virtual bool isComplete() const = 0;
	virtual Option<OperandConst> evalConstantOperand() const = 0;
	virtual Option<Operand*> simplify() = 0; // Warning: Option=none does not warrant that nothing has been simplified!
	virtual void accept(OperandVisitor& visitor) const = 0;
	virtual operand_kind_t kind() const = 0;
	
	virtual bool operator==(const Operand& o) const = 0;
	friend io::Output& operator<<(io::Output& out, const Operand& o) { return o.print(out); }
	
private:
	virtual io::Output& print(io::Output& out) const = 0;
};

// Constant values
class OperandConst : public Operand
{
public:
	OperandConst(const OperandConst& opd);
	OperandConst(t::int32 value);
	OperandConst(); // for use in Option
	
	inline t::int32 value() const { return _value; }
	
	Operand* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const;
	bool involvesVariable(const OperandVar& opdv) const;
	operand_state_t updateVar(const OperandVar& opdv, const Operand& opd_modifier);
	Option<OperandConst> evalConstantOperand() const;
	Option<Operand*> simplify(); // Warning: Option=none does not warrant that nothing has been simplified!
	inline bool isComplete() const { return true; }
	inline void accept(OperandVisitor& visitor) const { visitor.visit(*this); }
	inline operand_kind_t kind() const { return OPERAND_CONST; }
	
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
	
	inline t::int32 addr() const { return _addr; }
	inline bool isTempVar() const { return _addr < 0; }
	
	Operand* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const;
	bool involvesVariable(const OperandVar& opdv) const;
	operand_state_t updateVar(const OperandVar& opdv, const Operand& opd_modifier);
	Option<OperandConst> evalConstantOperand() const;
	Option<Operand*> simplify(); // Warning: Option=none does not warrant that nothing has been simplified!
	inline bool isComplete() const { return true; }
	inline void accept(OperandVisitor& visitor) const { visitor.visit(*this); }
	inline operand_kind_t kind() const { return OPERAND_VAR; }
	
	bool operator==(const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandVar& o) { return o.print(out); }
	
private:
	t::int32 _addr;
	io::Output& print(io::Output& out) const;
};

class OperandMem : public Operand
{	
public:
	OperandMem(const OperandMem& opd);
	OperandMem(const OperandConst& opdc);
	OperandMem(const OperandVar& opdv);
	OperandMem(const OperandVar& opdv, const OperandConst& opdc);
	
	inline const OperandConst& getConst() const { assert(_opdc); return *_opdc; }
	inline const OperandVar&   getVar()   const { assert(_opdv); return *_opdv; }
	inline operandmem_kind_t memkind() const { return _kind; }
	inline bool hasConst() const { return _kind & OPERANDMEMFLAG_HASCONST; }
	inline bool hasVar()   const { return _kind & OPERANDMEMFLAG_HASVAR; }
	
	Operand* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const;
	bool involvesVariable(const OperandVar& opdv) const;
	operand_state_t updateVar(const OperandVar& opdv, const Operand& opd_modifier);
	Option<OperandConst> evalConstantOperand() const;
	Option<Operand*> simplify(); // Warning: Option=none does not warrant that nothing has been simplified!
	inline bool isComplete() const { return !hasVar() || _opdv->isComplete(); }
	inline void accept(OperandVisitor& visitor) const { visitor.visit(*this); }
	inline operand_kind_t kind() const { return OPERAND_MEM; }
	
	bool operator==(const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandMem& o) { return o.print(out); }
	
private:
	OperandConst* _opdc;
	OperandVar*   _opdv;
	operandmem_kind_t _kind;
	io::Output& print(io::Output& out) const;
};

// Arithmetic Expressions
class OperandArithExpr : public Operand
{
public:
	OperandArithExpr(const OperandArithExpr& opd);
	OperandArithExpr(arithoperator_t opr, const Operand& opd1_); // unary constructor 
	OperandArithExpr(arithoperator_t opr, const Operand& opd1_, const Operand& opd2_);
	
	inline arithoperator_t opr() const { return _opr; }
	inline const Operand& leftOperand() const { return *opd1; }
	inline const Operand& rightOperand() const { assert(isBinary()); return *opd2; }
	
	inline bool isUnary()  const { return _opr <  ARITHOPR_ADD; }
	inline bool isBinary() const { return _opr >= ARITHOPR_ADD; }
	
	Operand* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const;
	bool involvesVariable(const OperandVar& opdv) const;
	operand_state_t updateVar(const OperandVar& opdv, const Operand& opd_modifier);
	Option<OperandConst> evalConstantOperand() const;
	Option<Operand*> simplify(); // Warning: Option=none does not warrant that nothing has been simplified!
	inline bool isComplete() const { return _opr != ARITHOPR_CMP && opd1->isComplete() && opd2->isComplete(); }
	inline void accept(OperandVisitor& visitor) const { visitor.visit(*this); }
	inline operand_kind_t kind() const { return OPERAND_ARITHEXPR; }
	
	bool operator==(const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandArithExpr& o) { return o.print(out); }
	
private:
	arithoperator_t _opr;
	Operand* opd1;
	Operand* opd2; // unused if operator is unary
		
	io::Output& print(io::Output& out) const;
};

io::Output& operator<<(io::Output& out, operand_kind_t kind);
io::Output& operator<<(io::Output& out, operandmem_kind_t kind);
io::Output& operator<<(io::Output& out, arithoperator_t opr);
// io::Output& operator<<(io::Output& out, operand_state_t state); // TODO

#endif
