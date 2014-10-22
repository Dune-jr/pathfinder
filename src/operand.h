#ifndef _OPERAND_H
#define _OPERAND_H

#include <elm/io/Output.h>
#include <elm/util/Option.h>
#include <cvc4/expr/expr.h>
#include <cvc4/expr/type.h>
#include "constant_variables_simplified.h"
// #include "constant_variables.h"

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
	OPERANDMEM_ABSOLUTE, // Const
	OPERANDMEM_RELATIVE, // sp+Const
};

enum operand_state_t
{
	OPERANDSTATE_UNCHANGED=0,
	OPERANDSTATE_UPDATED,
	OPERANDSTATE_INVALID, // assert(INVALID > UPDATED)
};

// TODO: remove this
enum pop_result_t // doAffinePop(Operand* opd_result, Operand* new_opd)
{
	POPRESULT_FAIL,     // stop everything and fail: nothing is filled
	POPRESULT_CONTINUE, // success: opd_result and new_opd are filled
	POPRESULT_DONE,     // last element: only opd_result is filled
};

class OperandConst;
class OperandVar;
class OperandArithExpr;
class OperandMem;

class AffineEquationState
{
public:
	AffineEquationState(t::int32 sp) : _is_negative(false), _delta(0), _sp_counter(0), _var_counter(0), _sp(sp) { }
	// inline bool isNegative() const { return _is_negative; }
	// inline bool isPositive() const { return !_is_negative; }
	inline int delta()       const { return sign()*_delta;     }
	inline int spCounter()   const { return sign()*_sp_counter;  }
	inline int varCounter()  const { return sign()*_var_counter; }
	inline void reverseSign() { _is_negative ^= 1; _delta = -_delta; _sp_counter = -_sp_counter; _var_counter = -_var_counter; }
	inline void addToDelta(int d) { _delta += d; }
	inline void onVarFound(t::int32 var) { if(var==_sp) _sp_counter++; else _var_counter++; }

private:
	inline int sign() const { if(_is_negative) return -1; else return +1; }

	bool _is_negative;
	int _delta;
	int _sp_counter;
	int _var_counter;
	t::int32 _sp;
};

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
	virtual ~Operand() { }
	virtual Operand* copy() const = 0;
	virtual unsigned int countTempVars() const = 0; // this will count a variable several times if it occurs several times
	virtual bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const = 0;
	virtual bool involvesVariable(const OperandVar& opdv) const = 0;
	virtual bool involvesMemoryCell(const OperandMem& opdm) const = 0;
	virtual bool involvesMemory() const = 0;
	virtual operand_state_t updateVar(const OperandVar& opdv, const Operand& opd_modifier) = 0;
	virtual bool isComplete() const = 0;
	virtual bool isAffine(const OperandVar& opdv, const OperandVar& sp) const = 0;
	virtual pop_result_t doAffinePop(Operand*& opd_result, Operand*& new_opd) = 0; // TODO: remove this
	virtual void parseAffineEquation(AffineEquationState& state) const = 0;
	virtual Option<OperandConst> evalConstantOperand() const = 0;
	virtual Option<Operand*> simplify() = 0; // Warning: Option=none does not warrant that nothing has been simplified!
	virtual Option<Operand*> replaceConstants(const ConstantVariablesSimplified& constants) = 0;
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
	~OperandConst();
	
	inline t::int32 value() const { return _value; }
	
	Operand* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const;
	bool involvesVariable(const OperandVar& opdv) const;
	bool involvesMemoryCell(const OperandMem& opdm) const;
	bool involvesMemory() const;
	operand_state_t updateVar(const OperandVar& opdv, const Operand& opd_modifier);
	Option<OperandConst> evalConstantOperand() const;
	Option<Operand*> simplify(); // Warning: Option=none does not warrant that nothing has been simplified!
	Option<Operand*> replaceConstants(const ConstantVariablesSimplified& constants); // warning: Option=none does not warrant that nothing has been replaced!
	pop_result_t doAffinePop(Operand*& opd_result, Operand*& new_opd);
	void parseAffineEquation(AffineEquationState& state) const;
	inline bool isComplete() const { return true; }
	inline bool isAffine(const OperandVar& opdv, const OperandVar& sp) const { return true; }
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
	~OperandVar();
	
	inline t::int32 addr() const { return _addr; }
	inline bool isTempVar() const { return _addr < 0; }
	
	Operand* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const;
	bool involvesVariable(const OperandVar& opdv) const;
	bool involvesMemoryCell(const OperandMem& opdm) const;
	bool involvesMemory() const;
	operand_state_t updateVar(const OperandVar& opdv, const Operand& opd_modifier);
	Option<OperandConst> evalConstantOperand() const;
	Option<Operand*> simplify(); // Warning: Option=none does not warrant that nothing has been simplified!
	Option<Operand*> replaceConstants(const ConstantVariablesSimplified& constants); // warning: Option=none does not warrant that nothing has been replaced!
	pop_result_t doAffinePop(Operand*& opd_result, Operand*& new_opd);
	void parseAffineEquation(AffineEquationState& state) const;
	inline bool isComplete() const { return true; }
	inline bool isAffine(const OperandVar& opdv, const OperandVar& sp) const { return (_addr == opdv.addr()) || (_addr == sp.addr()); }
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
	OperandMem(const OperandConst& opdc, bool relative = false); // relative to true means the memory is relative to the stack pointer
	//OperandMem(const OperandVar& opdv);
	//OperandMem(const OperandVar& opdv, const OperandConst& opdc);
	OperandMem(); // for use in Option
	~OperandMem();
	
	inline bool isRelative() const { return _kind == OPERANDMEM_ABSOLUTE; }
	inline bool isAbsolute() const { return _kind == OPERANDMEM_RELATIVE; }
	// inline bool hasConst() const { return _kind & OPERANDMEMFLAG_HASCONST; }
	// inline bool hasVar()   const { return _kind & OPERANDMEMFLAG_HASVAR;   }
	inline const OperandConst& getConst() const { assert(_opdc); return *_opdc; }
	
	Operand* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const;
	bool involvesVariable(const OperandVar& opdv) const;
	bool involvesMemoryCell(const OperandMem& opdm) const;
	bool involvesMemory() const;
	operand_state_t updateVar(const OperandVar& opdv, const Operand& opd_modifier);
	Option<OperandConst> evalConstantOperand() const;
	Option<Operand*> simplify(); // Warning: Option=none does not warrant that nothing has been simplified!
	Option<Operand*> replaceConstants(const ConstantVariablesSimplified& constants); // warning: Option=none does not warrant that nothing has been replaced!
	pop_result_t doAffinePop(Operand*& opd_result, Operand*& new_opd);
	void parseAffineEquation(AffineEquationState& state) const;
	inline bool isComplete() const { return true; }
	inline bool isAffine(const OperandVar& opdv, const OperandVar& sp) const { return false; }
	inline void accept(OperandVisitor& visitor) const { visitor.visit(*this); }
	inline operand_kind_t kind() const { return OPERAND_MEM; }
	
	bool operator==(const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandMem& o) { return o.print(out); }
	
private:
	OperandConst* _opdc;
	// OperandVar*   _opdv;
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
	~OperandArithExpr();
	
	inline arithoperator_t opr() const { return _opr; }
	inline const Operand& leftOperand() const { return *opd1; }
	inline const Operand& rightOperand() const { assert(isBinary()); return *opd2; }
	
	inline bool isUnary()  const { return _opr <  ARITHOPR_ADD; }
	inline bool isBinary() const { return _opr >= ARITHOPR_ADD; }
	
	Operand* copy() const;
	unsigned int countTempVars() const;
	bool involvesVariable(const OperandVar& opdv) const;
	bool involvesMemoryCell(const OperandMem& opdm) const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand*& expr) const;
	bool involvesMemory() const;
	operand_state_t updateVar(const OperandVar& opdv, const Operand& opd_modifier);
	Option<OperandConst> evalConstantOperand() const;
	Option<Operand*> simplify(); // Warning: Option=none does not warrant that nothing has been simplified!
	Option<Operand*> replaceConstants(const ConstantVariablesSimplified& constants); // warning: Option=none does not warrant that nothing has been replaced!
	pop_result_t doAffinePop(Operand*& opd_result, Operand*& new_opd);
	void parseAffineEquation(AffineEquationState& state) const;
	inline bool isComplete() const { return _opr != ARITHOPR_CMP && opd1->isComplete() && (isUnary() || opd2->isComplete()); }
	inline bool isAffine(const OperandVar& opdv, const OperandVar& sp) const
		{ return ((_opr == ARITHOPR_ADD) || (_opr == ARITHOPR_SUB)) && opd1->isAffine(opdv, sp) && opd2->isAffine(opdv, sp); }
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
io::Output& operator<<(io::Output& out, operand_state_t state);
io::Output& operator<<(io::Output& out, pop_result_t res);

#endif
