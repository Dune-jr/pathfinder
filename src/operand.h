#ifndef _OPERAND_H
#define _OPERAND_H

#include <otawa/cfg/Edge.h>
#include <elm/genstruct/Vector.h>
#include <elm/io/Output.h>
#include <elm/util/BitVector.h>
#include <elm/util/Option.h>
#include "constant.h"
#include "constant_variables_core.h"

using otawa::Edge;
using elm::genstruct::SLList;
// using elm::genstruct::Vector;

enum arithoperator_t
{
	// Unary
	ARITHOPR_NEG, // (-)   Arithmetic negation#define PROCESS_VAL(p) case(p): return out << #p; break;
	// ARITHOPR_NOT, // Should we implement bit inversion? // actually it's just 2^n - x - 1 !
	
	// Binary
	ARITHOPR_ADD, // (+)   Addition
	ARITHOPR_SUB, // (-)   Subtraction
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
	CST,	// Constant value
	VAR,	// Variable (register or temporary variable, for now)
	MEM,	// Memory cell
	TOP,	// Constant of unknown value
	ARITH,	// Arithmetic Expression
};
io::Output& operator<<(io::Output& out, operand_kind_t kind);

enum operandmem_kind_t
{
	OPERANDMEM_ABSOLUTE, // Const
	OPERANDMEM_RELATIVE, // sp+Const
};
io::Output& operator<<(io::Output& out, operandmem_kind_t kind);

class OperandConst;
class OperandVar;
class OperandMem;
class OperandTop;
class OperandArith;
class AffineEquationState;
class DAG;

// The visitor: an abstract class
class OperandVisitor
{
public:
	virtual bool visit(const OperandConst& o) = 0;
	virtual bool visit(const OperandVar& o) = 0;
	virtual bool visit(const OperandMem& o) = 0;
	virtual bool visit(const OperandTop& o) = 0;
	virtual bool visit(const OperandArith& o) = 0;
};

#define NONEW\
	friend class DAG;\
	inline void *operator new(size_t s) { return new char[s]; }

// Abstract Operand class
class Operand
{
public:
	virtual ~Operand() { }
	// virtual Operand* copy() const = 0;
	virtual unsigned int countTempVars() const = 0; // this will count a variable several times if it occurs several times
	virtual bool getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const = 0;
	int involvesOperand(const Operand& opd) const { return this->operator==(opd) ? 1 : 0; } // default case, to be overloaded for recursive classes
	virtual int involvesVariable(const OperandVar& opdv) const = 0; // TODO! rewrite using involvesOperand, if the int return thing is not critical?
	virtual Option<Constant> involvesStackBelow(const Constant& stack_limit) const = 0;
	virtual bool involvesMemoryCell(const OperandMem& opdm) const = 0;
	virtual bool involvesMemory() const = 0;
	virtual Option<const Operand*> update(DAG& dag, const Operand* opd, const Operand* opd_modifier) const = 0;
	virtual void markUsedRegisters(BitVector &uses) const = 0;
	virtual bool isComplete() const = 0;
	virtual bool isConstant() const = 0;
	virtual bool isLinear() const = 0;
	virtual bool isAffine(const OperandVar& opdv) const = 0;
	virtual void parseAffineEquation(AffineEquationState& state) const = 0;
	virtual Option<Constant> evalConstantOperand() const = 0; // all uses commented out?
	virtual Option<const Operand*> simplify(DAG& dag) const = 0; // Warning: Option=none does not warrant that nothing has been simplified!
	virtual const Operand* replaceConstants(DAG& dag, const ConstantVariablesCore& constants, Vector<OperandVar>& replaced_vars) const = 0;
	virtual bool accept(OperandVisitor& visitor) const = 0;
	virtual operand_kind_t kind() const = 0;
	friend inline io::Output& operator<<(io::Output& out, const Operand& o) { return o.print(out); }
	virtual bool operator==(const Operand& o) const = 0;
	virtual bool operator< (const Operand& o) const = 0;
	
	virtual inline const OperandConst& toConst() const { ASSERTP(false, "not an OperandConst: " << *this << " (" << kind() << ")"); }
	virtual inline const Constant& toConstant()  const { ASSERTP(false, "not a Constant: " 		<< *this << " (" << kind() << ")"); }
	virtual inline const OperandVar& toVar() 	 const { ASSERTP(false, "not an OperandVar: " 	<< *this << " (" << kind() << ")"); }
 	virtual inline const OperandMem& toMem() 	 const { ASSERTP(false, "not an OperandMem: " 	<< *this << " (" << kind() << ")"); }
	virtual inline const OperandTop& toTop() 	 const { ASSERTP(false, "not an OperandTop: " 	<< *this << " (" << kind() << ")"); }
	virtual inline const OperandArith& toArith() const { ASSERTP(false, "not an OperandArith: "	<< *this << " (" << kind() << ")"); }
	elm::String toString() const { return _ << *this; }
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
	
	// Operand* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const;
	// int involvesOperand(const Operand& opd) const;
	inline int involvesVariable(const OperandVar& opdv) const;
	Option<Constant> involvesStackBelow(const Constant& stack_limit) const;
	bool involvesMemoryCell(const OperandMem& opdm) const;
	bool involvesMemory() const;
	Option<const Operand*> update(DAG& dag, const Operand* opd, const Operand* opd_modifier) const;
	Option<Constant> evalConstantOperand() const;
	Option<const Operand*> simplify(DAG& dag) const; // Warning: Option=none does not warrant that nothing has been simplified!
	const Operand* replaceConstants(DAG& dag, const ConstantVariablesCore& constants, Vector<OperandVar>& replaced_vars) const; // warning: Option=none does not warrant that nothing has been replaced!
	void parseAffineEquation(AffineEquationState& state) const;
	inline void markUsedRegisters(BitVector& uses) const { }
	inline bool isComplete() const { return true; }
	inline bool isConstant() const { return true; }
	inline bool isLinear()   const { return true; }
	inline bool isAffine(const OperandVar& opdv) const { return true; }
	inline bool accept(OperandVisitor& visitor) const { return visitor.visit(*this); }
	inline operand_kind_t kind() const { return CST; }
	inline operator Constant() const { return _value; }
	OperandConst& operator=(const OperandConst& opd);
	inline bool operator==(const Operand& o) const;
	inline bool operator< (const Operand& o) const;
	inline const OperandConst& toConst() const { return *this; }
	friend inline io::Output& operator<<(io::Output& out, const OperandConst& o) { return o.print(out); }
	inline const Constant& toConstant() const { return _value; }
private:
	NONEW;
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
	
	// Operand* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const;
	// int involvesOperand(const Operand& opd) const;
	inline int involvesVariable(const OperandVar& opdv) const;
	Option<Constant> involvesStackBelow(const Constant& stack_limit) const;
	bool involvesMemoryCell(const OperandMem& opdm) const;
	bool involvesMemory() const;
	Option<const Operand*> update(DAG& dag, const Operand* opd, const Operand* opd_modifier) const;
	Option<Constant> evalConstantOperand() const;
	Option<const Operand*> simplify(DAG& dag) const; // Warning: Option=none does not warrant that nothing has been simplified!
	const Operand* replaceConstants(DAG& dag, const ConstantVariablesCore& constants, Vector<OperandVar>& replaced_vars) const; // warning: Option=none does not warrant that nothing has been replaced!
	void parseAffineEquation(AffineEquationState& state) const;
	inline void markUsedRegisters(BitVector& uses) const { if(!isTempVar()) uses.set(_addr); }
	inline bool isComplete() const { return true; }
	inline bool isConstant() const { return false; }
	inline bool isLinear()   const { return true; }
	inline bool isAffine(const OperandVar& opdv) const { return _addr == opdv.addr(); }
	inline bool accept(OperandVisitor& visitor) const { return visitor.visit(*this); }
	inline operand_kind_t kind() const { return VAR; }
	OperandVar& operator=(const OperandVar& opd);
	bool operator==(const Operand& o) const;
	bool operator< (const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandVar& o) { return o.print(out); }
	const OperandVar& toVar() const { return *this; }
private:
	NONEW;
	io::Output& print(io::Output& out) const;
	t::int32 _addr;
};

class OperandMem : public Operand
{	
public:
	OperandMem(const OperandMem& opd);
	OperandMem(const OperandConst& opdc);
	OperandMem(); // for use in Option
	
	inline const OperandConst& addr() const { return _opdc; }
	
	// Operand* copy() const { return new OperandMem(*this); }
	unsigned int countTempVars() const { return 0; }
	bool getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const;
	// int involvesOperand(const Operand& opd) const;
	inline int involvesVariable(const OperandVar& opdv) const;
	Option<Constant> involvesStackBelow(const Constant& stack_limit) const;
	bool involvesMemoryCell(const OperandMem& opdm) const;
	bool involvesMemory() const;
	Option<const Operand*> update(DAG& dag, const Operand* opd, const Operand* opd_modifier) const;
	Option<Constant> evalConstantOperand() const;
	Option<const Operand*> simplify(DAG& dag) const; // Warning: Option=none does not warrant that nothing has been simplified!
	const Operand* replaceConstants(DAG& dag, const ConstantVariablesCore& constants, Vector<OperandVar>& replaced_vars) const; // warning: Option=none does not warrant that nothing has been replaced!
	void parseAffineEquation(AffineEquationState& state) const;
	inline void markUsedRegisters(BitVector& uses) const { }
	inline bool isComplete() const { return true; }
	inline bool isConstant() const { return false; }
	inline bool isLinear()   const { return true; }
	inline bool isAffine(const OperandVar& opdv) const { return false; }
	inline bool accept(OperandVisitor& visitor) const { return visitor.visit(*this); }
	inline operand_kind_t kind() const { return MEM; }
	OperandMem& operator=(const OperandMem& opd);
	inline bool operator==(const Operand& o) const;
	inline bool operator< (const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandMem& o) { return o.print(out); }
	inline const OperandMem& toMem() const { return *this; }
private:
	NONEW;
	io::Output& print(io::Output& out) const;

	OperandConst _opdc;
};

class OperandTop : public Operand
{
public:
	OperandTop();
	OperandTop(int id);
	OperandTop(const OperandTop& opd);
	
	inline bool isUnidentified() const { return id == -1; } // blank Top
	inline bool isIdentified() const { return !isUnidentified(); }
	
	// Operand* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const;
	// int involvesOperand(const Operand& opd) const;
	inline int involvesVariable(const OperandVar& opdv) const;
	Option<Constant> involvesStackBelow(const Constant& stack_limit) const;
	bool involvesMemoryCell(const OperandMem& opdm) const;
	bool involvesMemory() const;
	Option<const Operand*> update(DAG& dag, const Operand* opd, const Operand* opd_modifier) const;
	Option<Constant> evalConstantOperand() const;
	Option<const Operand*> simplify(DAG& dag) const; // Warning: Option=none does not warrant that nothing has been simplified!
	const Operand* replaceConstants(DAG& dag, const ConstantVariablesCore& constants, Vector<OperandVar>& replaced_vars) const; // warning: Option=none does not warrant that nothing has been replaced!
	void parseAffineEquation(AffineEquationState& state) const;
	inline void markUsedRegisters(BitVector& uses) const { }
	inline bool isComplete() const { return true; }
	inline bool isConstant() const { return false; }
	inline bool isLinear()   const { ASSERT(false); return true; }
	inline bool isAffine(const OperandVar& opdv) const { return false; }
	inline bool accept(OperandVisitor& visitor) const { return visitor.visit(*this); }
	inline operand_kind_t kind() const { return TOP; }
	OperandTop& operator=(const OperandTop& opd);
	inline bool operator==(const Operand& o) const;
	inline bool operator< (const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandTop& o) { return o.print(out); }
	const OperandTop& toTop() const { return *this; }
private:
	// NONEW;
	io::Output& print(io::Output& out) const;

	int id;
	static int next_id;
};
extern OperandTop const* const Top;

// Arithmetic Expressions
class OperandArith : public Operand
{
public:
	// OperandArith(const OperandArith& opd);
	// OperandArith(arithoperator_t opr, const Operand& opd1_); // unary constructor 
	// OperandArith(arithoperator_t opr, const Operand& opd1_, const Operand& opd2_);
	OperandArith(arithoperator_t opr, const Operand* opd1_, const Operand* opd2_ = NULL);
	// ~OperandArith();
	
	inline arithoperator_t opr() const { return _opr; }
	inline const Operand* left() const { return opd1; }
	inline const Operand* right() const { return opd2; }
	inline const Operand& leftOperand() const { return *opd1; }
	inline const Operand& rightOperand() const { ASSERT(isBinary()); return *opd2; }
	
	inline bool isUnary()  const { return _opr <  ARITHOPR_ADD; }
	inline bool isBinary() const { return _opr >= ARITHOPR_ADD; }
	
	// Operand* copy() const;
	unsigned int countTempVars() const;
	int involvesOperand(const Operand& opd) const { return opd1->involvesOperand(opd) + opd2->involvesOperand(opd); }
	inline int involvesVariable(const OperandVar& opdv) const;
	Option<Constant> involvesStackBelow(const Constant& stack_limit) const;
	bool involvesMemoryCell(const OperandMem& opdm) const;
	bool getIsolatedTempVar(OperandVar& temp_var, Operand const*& expr) const;
	bool involvesMemory() const;
	Option<const Operand*> update(DAG& dag, const Operand* opd, const Operand* opd_modifier) const;
	Option<Constant> evalConstantOperand() const;
	Option<const Operand*> simplify(DAG& dag) const; // Warning: Option=none does not warrant that nothing has been simplified!
	const Operand* replaceConstants(DAG& dag, const ConstantVariablesCore& constants, Vector<OperandVar>& replaced_vars) const; // warning: Option=none does not warrant that nothing has been replaced!
	void parseAffineEquation(AffineEquationState& state) const;
	inline void markUsedRegisters(BitVector& uses) const { opd1->markUsedRegisters(uses); if(isBinary()) opd1->markUsedRegisters(uses); }
	inline bool isComplete() const { return _opr != ARITHOPR_CMP && opd1->isComplete() && (isUnary() || opd2->isComplete()); }
	inline bool isConstant() const { return opd1->isConstant() && (isUnary() || opd2->isConstant()); }
	bool isLinear() const;
	inline bool isAffine(const OperandVar& opdv) const
		{ return ((_opr == ARITHOPR_ADD) || (_opr == ARITHOPR_SUB)) && opd1->isAffine(opdv) && opd2->isAffine(opdv); }
	inline bool accept(OperandVisitor& visitor) const { return visitor.visit(*this); }
	inline operand_kind_t kind() const { return ARITH; }
	// OperandArith& operator=(const OperandArith& opd);
	inline bool operator==(const Operand& o) const;
	inline bool operator< (const Operand& o) const;
	friend inline io::Output& operator<<(io::Output& out, const OperandArith& o) { return o.print(out); }
	inline const OperandArith& toArith() const { return *this; }
private:
	NONEW;
	io::Output& print(io::Output& out) const;
	
	arithoperator_t _opr;
	const Operand* opd1;
	const Operand* opd2; // unused if operator is unary
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
	inline void onVarFound(const OperandVar& var) { _var_counter++; /*if(!_var) _var = elm::some(var); else ASSERT(*_var == var);*/ }
	inline void onSpFound(bool sign = SIGN_POSITIVE) { if(sign == SIGN_POSITIVE) _sp_counter++; else _sp_counter--; }
private:
	NONEW;
	inline int sign() const { if(_is_negative) return -1; else return +1; }

	bool _is_negative;
	int _delta;
	int _sp_counter;
	int _var_counter;
	// Option<OperandVar> _var; // removed, this is just to check consistency
};

io::Output& operator<<(io::Output& out, arithoperator_t opr);

#endif
