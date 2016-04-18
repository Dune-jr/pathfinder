#ifndef _OPERAND_H
#define _OPERAND_H

#include <otawa/cfg/Edge.h>
#include <elm/genstruct/Vector.h>
#include <elm/io/Output.h>
#include <elm/util/Option.h>
#include "constant.h"
#include "constant_variables_simplified.h" // reason for this is recursive inclusion of classes, using a visitor in constant_variables for toPredicates may be cleaner

using otawa::Edge;
using elm::genstruct::SLList;
using elm::genstruct::Vector;

enum arithoperator_t
{
	// Unary
	ARITHOPR_NEG, // (-)   Arithmetic negation
	// ARITHOPR_NOT, // Should we implement bit inversion? // actually it's just 2^n - x - 1 !
	
	// Binary
	ARITHOPR_ADD, // (+)   Addition
	ARITHOPR_SUB, // (-)   Substraction
	ARITHOPR_MUL, // (*)   Multiplication
	ARITHOPR_MULH,// (*H)  32 Most significant bits from 64-bit multiplication
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
	OPERAND_MEM, 	   // Arithmetic Expression
};

enum operandmem_kind_t
{
	OPERANDMEM_ABSOLUTE, // Const
	OPERANDMEM_RELATIVE, // sp+Const
};

class OperandConst;
class OperandVar;
class OperandArithExpr;
class OperandMem;
class AffineEquationState;

// The visitor: an abstract class
class OperandVisitor
{
public:
	virtual bool visit(const OperandConst& o) = 0;
	virtual bool visit(const OperandVar& o) = 0;
	virtual bool visit(const OperandMem& o) = 0;
	virtual bool visit(const OperandArithExpr& o) = 0;
};

// Abstract Operand class
class Operand
{
public:
	virtual ~Operand() { }
	virtual Operand* copy() const = 0;
	virtual unsigned int countTempVars() const = 0; // this will count a variable several times if it occurs several times
	virtual bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const = 0;
	int involvesOperand(const Operand& opd) const { return this->operator==(opd) ? 1 : 0; } // default case, to be overloaded for recursive classes
	virtual int involvesVariable(const OperandVar& opdv) const = 0; // TODO! rewrite using involvesOperand, if the int return thing is not critical?
	virtual Option<Constant> involvesStackBelow(const Constant& stack_limit) const = 0;
	virtual bool involvesMemoryCell(const OperandMem& opdm) const = 0;
	virtual bool involvesMemory() const = 0;
	virtual bool update(const Operand& opd, const Operand& opd_modifier) = 0;
	virtual bool isComplete() const = 0;
	virtual bool isAffine(const OperandVar& opdv) const = 0;
	virtual void parseAffineEquation(AffineEquationState& state) const = 0;
	virtual Option<OperandConst> evalConstantOperand() const = 0; // all uses commented out?
	virtual Option<Operand*> simplify() = 0; // Warning: Option=none does not warrant that nothing has been simplified!
	virtual Option<Operand*> replaceConstants(const ConstantVariablesSimplified& constants, Vector<OperandVar>& replaced_vars) = 0;
	virtual bool accept(OperandVisitor& visitor) const = 0;
	virtual operand_kind_t kind() const = 0;
	virtual bool operator==(const Operand& o) const = 0;
	friend inline io::Output& operator<<(io::Output& out, const Operand& o) { return o.print(out); }
	
private:
	virtual io::Output& print(io::Output& out) const = 0;
};

// Constant values
class OperandConst : public Operand
{
public:
	OperandConst(const OperandConst& opd);
	OperandConst(const Constant& value);
	OperandConst(); // for use in Option
	~OperandConst();
	
	inline Constant value() const { return _value; }
	
	Operand* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const;
	// int involvesOperand(const Operand& opd) const;
	int involvesVariable(const OperandVar& opdv) const;
	Option<Constant> involvesStackBelow(const Constant& stack_limit) const;
	bool involvesMemoryCell(const OperandMem& opdm) const;
	bool involvesMemory() const;
	bool update(const Operand& opd, const Operand& opd_modifier);
	Option<OperandConst> evalConstantOperand() const;
	Option<Operand*> simplify(); // Warning: Option=none does not warrant that nothing has been simplified!
	Option<Operand*> replaceConstants(const ConstantVariablesSimplified& constants, Vector<OperandVar>& replaced_vars); // warning: Option=none does not warrant that nothing has been replaced!
	void parseAffineEquation(AffineEquationState& state) const;
	inline bool isComplete() const { return true; }
	inline bool isAffine(const OperandVar& opdv) const { return true; }
	inline bool accept(OperandVisitor& visitor) const { return visitor.visit(*this); }
	inline operand_kind_t kind() const { return OPERAND_CONST; }
	OperandConst& operator=(const OperandConst& opd);
	bool operator==(const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandConst& o) { return o.print(out); }
private:
	io::Output& print(io::Output& out) const;

	Constant _value;
};

// Variables
class OperandVar : public Operand
{	
public:
	OperandVar(); // for Vector<OperandVar>
	OperandVar(const OperandVar& opd);
	OperandVar(t::int32 addr);
	// ~OperandVar();
	
	inline t::int32 addr() const { return _addr; }
	inline bool isTempVar() const { return _addr < 0; }
	
	Operand* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const;
	// int involvesOperand(const Operand& opd) const;
	int involvesVariable(const OperandVar& opdv) const;
	Option<Constant> involvesStackBelow(const Constant& stack_limit) const;
	bool involvesMemoryCell(const OperandMem& opdm) const;
	bool involvesMemory() const;
	bool update(const Operand& opd, const Operand& opd_modifier);
	Option<OperandConst> evalConstantOperand() const;
	Option<Operand*> simplify(); // Warning: Option=none does not warrant that nothing has been simplified!
	Option<Operand*> replaceConstants(const ConstantVariablesSimplified& constants, Vector<OperandVar>& replaced_vars); // warning: Option=none does not warrant that nothing has been replaced!
	void parseAffineEquation(AffineEquationState& state) const;
	inline bool isComplete() const { return true; }
	inline bool isAffine(const OperandVar& opdv) const { return _addr == opdv.addr(); }
	inline bool accept(OperandVisitor& visitor) const { return visitor.visit(*this); }
	inline operand_kind_t kind() const { return OPERAND_VAR; }
	OperandVar& operator=(const OperandVar& opd);
	bool operator==(const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandVar& o) { return o.print(out); }	
private:
	io::Output& print(io::Output& out) const;

	t::int32 _addr;
};

class OperandMem : public Operand
{	
public:
	OperandMem(const OperandMem& opd);
	OperandMem(const OperandConst& opdc);
	OperandMem(); // for use in Option
	~OperandMem();
	
	inline const OperandConst& getConst() const { ASSERT(_opdc); return *_opdc; }
	
	Operand* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const;
	// int involvesOperand(const Operand& opd) const;
	int involvesVariable(const OperandVar& opdv) const;
	Option<Constant> involvesStackBelow(const Constant& stack_limit) const;
	bool involvesMemoryCell(const OperandMem& opdm) const;
	bool involvesMemory() const;
	bool update(const Operand& opd, const Operand& opd_modifier);
	Option<OperandConst> evalConstantOperand() const;
	Option<Operand*> simplify(); // Warning: Option=none does not warrant that nothing has been simplified!
	Option<Operand*> replaceConstants(const ConstantVariablesSimplified& constants, Vector<OperandVar>& replaced_vars); // warning: Option=none does not warrant that nothing has been replaced!
	void parseAffineEquation(AffineEquationState& state) const;
	inline bool isComplete() const { return true; }
	inline bool isAffine(const OperandVar& opdv) const { return false; }
	inline bool accept(OperandVisitor& visitor) const { return visitor.visit(*this); }
	inline operand_kind_t kind() const { return OPERAND_MEM; }
	OperandMem& operator=(const OperandMem& opd);
	bool operator==(const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandMem& o) { return o.print(out); }	
private:
	io::Output& print(io::Output& out) const;

	OperandConst* _opdc;
};

// Arithmetic Expressions
class OperandArithExpr : public Operand
{
public:
	OperandArithExpr(const OperandArithExpr& opd);
	OperandArithExpr(arithoperator_t opr, const Operand& opd1_); // unary constructor 
	OperandArithExpr(arithoperator_t opr, const Operand& opd1_, const Operand& opd2_);
	~OperandArithExpr();
	
	inline arithoperator_t opr() const { return _opr; }
	inline const Operand& leftOperand() const { return *opd1; }
	inline const Operand& rightOperand() const { ASSERT(isBinary()); return *opd2; }
	
	inline bool isUnary()  const { return _opr <  ARITHOPR_ADD; }
	inline bool isBinary() const { return _opr >= ARITHOPR_ADD; }
	
	Operand* copy() const;
	unsigned int countTempVars() const;
	int involvesOperand(const Operand& opd) const { return opd1->involvesOperand(opd) + opd2->involvesOperand(opd); }
	int involvesVariable(const OperandVar& opdv) const;
	Option<Constant> involvesStackBelow(const Constant& stack_limit) const;
	bool involvesMemoryCell(const OperandMem& opdm) const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const;
	bool involvesMemory() const;
	bool update(const Operand& opd, const Operand& opd_modifier);
	Option<OperandConst> evalConstantOperand() const;
	Option<Operand*> simplify(); // Warning: Option=none does not warrant that nothing has been simplified!
	Option<Operand*> replaceConstants(const ConstantVariablesSimplified& constants, Vector<OperandVar>& replaced_vars); // warning: Option=none does not warrant that nothing has been replaced!
	void parseAffineEquation(AffineEquationState& state) const;
	inline bool isComplete() const { return _opr != ARITHOPR_CMP && opd1->isComplete() && (isUnary() || opd2->isComplete()); }
	inline bool isAffine(const OperandVar& opdv) const
		{ return ((_opr == ARITHOPR_ADD) || (_opr == ARITHOPR_SUB)) && opd1->isAffine(opdv) && opd2->isAffine(opdv); }
	inline bool accept(OperandVisitor& visitor) const { return visitor.visit(*this); }
	inline operand_kind_t kind() const { return OPERAND_ARITHEXPR; }
	OperandArithExpr& operator=(const OperandArithExpr& opd);
	bool operator==(const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandArithExpr& o) { return o.print(out); }
private:
	io::Output& print(io::Output& out) const;
	
	arithoperator_t _opr;
	Operand* opd1;
	Operand* opd2; // unused if operator is unary
};


class AffineEquationState
{
public:
	AffineEquationState() : _is_negative(false), _delta(0), _sp_counter(0), _var_counter(0) { }
	inline int delta()       const { return sign()*_delta;     }
	inline int spCounter()   const { return sign()*_sp_counter;  }
	inline int varCounter()  const { return sign()*_var_counter; }
	inline void reverseSign() { _is_negative ^= 1; _delta = -_delta; _sp_counter = -_sp_counter; _var_counter = -_var_counter; }
	inline void addToDelta(int d) { _delta += d; }
	inline void onVarFound(const OperandVar& var) { _var_counter++; if(_var) assert(*_var == var); else _var = elm::some(var); }
	inline void onSpFound(bool sign = SIGN_POSITIVE) { if(sign == SIGN_POSITIVE) _sp_counter++; else _sp_counter--; }
private:
	inline int sign() const { if(_is_negative) return -1; else return +1; }

	bool _is_negative;
	int _delta;
	int _sp_counter;
	int _var_counter;
	Option<OperandVar> _var; // TODO! remove, this is just to check consistency
};

io::Output& operator<<(io::Output& out, operand_kind_t kind);
io::Output& operator<<(io::Output& out, operandmem_kind_t kind);
io::Output& operator<<(io::Output& out, arithoperator_t opr);

#endif
