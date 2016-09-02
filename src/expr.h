#ifndef EXPR_H
#define EXPR_H

#include <otawa/cfg/Edge.h>
#include <elm/genstruct/Vector.h>
#include <elm/io/Output.h>
#include <elm/util/Option.h>
#include "constant.h"
// #include "constant_variables_simplified.h" // reason for this is recursive inclusion of classes, using a visitor in constant_variables for toPredicates may be cleaner

using otawa::Edge;
using elm::genstruct::SLList;
using elm::genstruct::Vector;

namespace expr {

enum arithoperator_t
{
	// Unary
	NEG, // (-)   Arithmetic negation
	
	// Binary
	ADD, // (+)   Addition
	SUB, // (-)   Subtraction
	MUL, // (*)   Multiplication
	MULH,// (*H)  32 Most significant bits from 64-bit multiplication
	DIV, // (/)   Division
	MOD, // (mod) Modulo
	CMP, // (~)   Special operator to be used with CONDOPR_EQ and a register that indicates
		 //       that it contains information over the comparison of two other registers
};

enum expr_kind_t
{
	CST,	// Constant value
	VAR,	// Variable (register or temporary variable, for now)
	MEM,	// Memory cell
	TOP,	// Unknown value variable
	ARITH,	// Arithmetic Expression
};

enum operandmem_kind_t
{
	ABSOLUTE, // Const
	RELATIVE, // SP+Const
};

class ExprCst;
class ExprVar;
class ExprMem;
class ExprTop;
class ExprArith;
class AffineEquationState;

// The visitor: an abstract class
class ExprVisitor
{
public:
	virtual bool visit(const ExprCst& o) = 0;
	virtual bool visit(const ExprVar& o) = 0;
	virtual bool visit(const ExprMem& o) = 0;
	virtual bool visit(const ExprArith& o) = 0;
};

// Abstract Expr class
class Expr
{
public:
	virtual ~Expr() { }
	virtual Expr* copy() const = 0;
	virtual unsigned int countTempVars() const = 0; // this will count a variable several times if it occurs several times
	virtual bool getIsolatedTempVar(ExprVar& temp_var, Expr const*& expr) const = 0;
	int involvesExpr(const Expr& opd) const { return this->operator==(opd) ? 1 : 0; } // default case, to be overloaded for recursive classes
	virtual int involvesVariable(const ExprVar& opdv) const = 0; // TODO! rewrite using involvesExpr, if the int return thing is not critical?
	virtual Option<Constant> involvesStackBelow(const Constant& stack_limit) const = 0;
	virtual bool involvesMemoryCell(const ExprMem& opdm) const = 0;
	virtual bool involvesMemory() const = 0;
	virtual bool update(const Expr& opd, const Expr& opd_modifier) = 0;
	virtual bool isComplete() const = 0;
	virtual bool isConstant() const = 0;
	virtual bool isLinear() const = 0;
	virtual bool isAffine(const ExprVar& opdv) const = 0;
	virtual void parseAffineEquation(AffineEquationState& state) const = 0;
	virtual Option<ExprCst> evalConstantExpr() const = 0; // all uses commented out?
	virtual Option<Expr*> simplify() = 0; // Warning: Option=none does not warrant that nothing has been simplified!
	// virtual Option<Expr*> replaceConstants(const ConstantVariablesSimplified& constants, Vector<ExprVar>& replaced_vars) = 0;
	virtual bool accept(ExprVisitor& visitor) const = 0;
	virtual expr::expr_kind_t kind() const = 0;
	virtual bool operator==(const Expr& o) const = 0;
	friend inline io::Output& operator<<(io::Output& out, const Expr& o) { return o.print(out); }
	
private:
	virtual io::Output& print(io::Output& out) const = 0;
};

// Constant values
class ExprCst : public Expr
{
public:
	ExprCst(const ExprCst& opd);
	ExprCst(const Constant& value);
	ExprCst(); // for use in Option
	~ExprCst();
	
	inline Constant value() const { return _value; }
	
	Expr* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(ExprVar& temp_var, Expr const*& expr) const;
	// int involvesExpr(const Expr& opd) const;
	int involvesVariable(const ExprVar& opdv) const;
	Option<Constant> involvesStackBelow(const Constant& stack_limit) const;
	bool involvesMemoryCell(const ExprMem& opdm) const;
	bool involvesMemory() const;
	bool update(const Expr& opd, const Expr& opd_modifier);
	Option<ExprCst> evalConstantExpr() const;
	Option<Expr*> simplify(); // Warning: Option=none does not warrant that nothing has been simplified!
	// Option<Expr*> replaceConstants(const ConstantVariablesSimplified& constants, Vector<ExprVar>& replaced_vars); // warning: Option=none does not warrant that nothing has been replaced!
	void parseAffineEquation(AffineEquationState& state) const;
	inline bool isComplete() const { return true; }
	inline bool isConstant() const { return true; }
	inline bool isLinear()   const { return true; }
	inline bool isAffine(const ExprVar& opdv) const { return true; }
	inline bool accept(ExprVisitor& visitor) const { return visitor.visit(*this); }
	inline expr::expr_kind_t kind() const { return expr::CST; }
	ExprCst& operator=(const ExprCst& opd);
	bool operator==(const Expr& o) const;
	friend inline io::Output& operator<<(io::Output& out, const ExprCst& o) { return o.print(out); }
private:
	io::Output& print(io::Output& out) const;

	Constant _value;
};

// Variables
class ExprVar : public Expr
{	
public:
	ExprVar(); // for Vector<ExprVar>
	ExprVar(const ExprVar& opd);
	ExprVar(t::int32 addr);
	// ~ExprVar();
	
	inline t::int32 addr() const { return _addr; }
	inline bool isTempVar() const { return _addr < 0; }
	
	Expr* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(ExprVar& temp_var, Expr const*& expr) const;
	// int involvesExpr(const Expr& opd) const;
	int involvesVariable(const ExprVar& opdv) const;
	Option<Constant> involvesStackBelow(const Constant& stack_limit) const;
	bool involvesMemoryCell(const ExprMem& opdm) const;
	bool involvesMemory() const;
	bool update(const Expr& opd, const Expr& opd_modifier);
	Option<ExprCst> evalConstantExpr() const;
	Option<Expr*> simplify(); // Warning: Option=none does not warrant that nothing has been simplified!
	// Option<Expr*> replaceConstants(const ConstantVariablesSimplified& constants, Vector<ExprVar>& replaced_vars); // warning: Option=none does not warrant that nothing has been replaced!
	void parseAffineEquation(AffineEquationState& state) const;
	inline bool isComplete() const { return true; }
	inline bool isConstant() const { return false; }
	inline bool isLinear()   const { return true; }
	inline bool isAffine(const ExprVar& opdv) const { return _addr == opdv.addr(); }
	inline bool accept(ExprVisitor& visitor) const { return visitor.visit(*this); }
	inline expr::expr_kind_t kind() const { return expr::VAR; }
	ExprVar& operator=(const ExprVar& opd);
	bool operator==(const Expr& o) const;
	friend inline io::Output& operator<<(io::Output& out, const ExprVar& o) { return o.print(out); }	
private:
	io::Output& print(io::Output& out) const;

	t::int32 _addr;
};

class ExprMem : public Expr
{	
public:
	ExprMem(const ExprMem& opd);
	ExprMem(const ExprCst& opdc);
	ExprMem(); // for use in Option
	~ExprMem();
	
	inline const ExprCst& getConst() const { ASSERT(_opdc); return *_opdc; }
	
	Expr* copy() const;
	unsigned int countTempVars() const;
	bool getIsolatedTempVar(ExprVar& temp_var, Expr const*& expr) const;
	// int involvesExpr(const Expr& opd) const;
	int involvesVariable(const ExprVar& opdv) const;
	Option<Constant> involvesStackBelow(const Constant& stack_limit) const;
	bool involvesMemoryCell(const ExprMem& opdm) const;
	bool involvesMemory() const;
	bool update(const Expr& opd, const Expr& opd_modifier);
	Option<ExprCst> evalConstantExpr() const;
	Option<Expr*> simplify(); // Warning: Option=none does not warrant that nothing has been simplified!
	// Option<Expr*> replaceConstants(const ConstantVariablesSimplified& constants, Vector<ExprVar>& replaced_vars); // warning: Option=none does not warrant that nothing has been replaced!
	void parseAffineEquation(AffineEquationState& state) const;
	inline bool isComplete() const { return true; }
	inline bool isConstant() const { return false; }
	inline bool isLinear()   const { return true; }
	inline bool isAffine(const ExprVar& opdv) const { return false; }
	inline bool accept(ExprVisitor& visitor) const { return visitor.visit(*this); }
	inline expr::expr_kind_t kind() const { return expr::MEM; }
	ExprMem& operator=(const ExprMem& opd);
	bool operator==(const Expr& o) const;
	friend inline io::Output& operator<<(io::Output& out, const ExprMem& o) { return o.print(out); }	
private:
	io::Output& print(io::Output& out) const;

	ExprCst* _opdc;
};

// Arithmetic Expressions
class ExprArith : public Expr
{
public:
	ExprArith(const ExprArith& opd);
	ExprArith(expr::arithoperator_t opr, const Expr& opd1_); // unary constructor 
	ExprArith(expr::arithoperator_t opr, const Expr& opd1_, const Expr& opd2_);
	~ExprArith();
	
	inline expr::arithoperator_t opr() const { return _opr; }
	inline const Expr& leftExpr() const { return *opd1; }
	inline const Expr& rightExpr() const { ASSERT(isBinary()); return *opd2; }
	
	inline bool isUnary()  const { return _opr <  expr::ADD; }
	inline bool isBinary() const { return _opr >= expr::ADD; }
	
	Expr* copy() const;
	unsigned int countTempVars() const;
	int involvesExpr(const Expr& opd) const { return opd1->involvesExpr(opd) + opd2->involvesExpr(opd); }
	int involvesVariable(const ExprVar& opdv) const;
	Option<Constant> involvesStackBelow(const Constant& stack_limit) const;
	bool involvesMemoryCell(const ExprMem& opdm) const;
	bool getIsolatedTempVar(ExprVar& temp_var, Expr const*& expr) const;
	bool involvesMemory() const;
	bool update(const Expr& opd, const Expr& opd_modifier);
	Option<ExprCst> evalConstantExpr() const;
	Option<Expr*> simplify(); // Warning: Option=none does not warrant that nothing has been simplified!
	// Option<Expr*> replaceConstants(const ConstantVariablesSimplified& constants, Vector<ExprVar>& replaced_vars); // warning: Option=none does not warrant that nothing has been replaced!
	void parseAffineEquation(AffineEquationState& state) const;
	inline bool isComplete() const { return _opr != expr::CMP && opd1->isComplete() && (isUnary() || opd2->isComplete()); }
	inline bool isConstant() const { return opd1->isConstant() && (isUnary() || opd2->isConstant()); }
	bool isLinear() const;
	inline bool isAffine(const ExprVar& opdv) const
		{ return ((_opr == expr::ADD) || (_opr == expr::SUB)) && opd1->isAffine(opdv) && opd2->isAffine(opdv); }
	inline bool accept(ExprVisitor& visitor) const { return visitor.visit(*this); }
	inline expr::expr_kind_t kind() const { return expr::ARITH; }
	ExprArith& operator=(const ExprArith& opd);
	bool operator==(const Expr& o) const;
	friend inline io::Output& operator<<(io::Output& out, const ExprArith& o) { return o.print(out); }
private:
	io::Output& print(io::Output& out) const;
	
	expr::arithoperator_t _opr;
	Expr* opd1;
	Expr* opd2; // unused if operator is unary
};


/*class AffineEquationState
{
public:
	AffineEquationState() : _is_negative(false), _delta(0), _sp_counter(0), _var_counter(0) { }
	inline int delta()       const { return sign()*_delta;     }
	inline int spCounter()   const { return sign()*_sp_counter;  }
	inline int varCounter()  const { return sign()*_var_counter; }
	inline void reverseSign() { _is_negative ^= 1; _delta = -_delta; _sp_counter = -_sp_counter; _var_counter = -_var_counter; }
	inline void addToDelta(int d) { _delta += d; }
	inline void onVarFound(const ExprVar& var) { _var_counter++; if(_var) assert(*_var == var); else _var = elm::some(var); }
	inline void onSpFound(bool sign = SIGN_POSITIVE) { if(sign == SIGN_POSITIVE) _sp_counter++; else _sp_counter--; }
private:
	inline int sign() const { if(_is_negative) return -1; else return +1; }

	bool _is_negative;
	int _delta;
	int _sp_counter;
	int _var_counter;
	Option<ExprVar> _var; // TODO! remove, this is just to check consistency
};*/

io::Output& operator<<(io::Output& out, expr::expr_kind_t kind);
io::Output& operator<<(io::Output& out, expr::operandmem_kind_t kind);
io::Output& operator<<(io::Output& out, expr::arithoperator_t opr);

}; // expr namespace

#endif