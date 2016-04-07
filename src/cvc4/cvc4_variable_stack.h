#ifndef _CVC4_CVC4_VARIABLE_STACK_H
#define _CVC4_CVC4_VARIABLE_STACK_H

#include <elm/genstruct/AVLMap.h>
#include <cvc4/expr/expr_manager.h>
#include "../operand.h"

using CVC4::Expr;
using elm::genstruct::AVLMap;

class CVC4VariableStack
{
public:
	CVC4VariableStack(CVC4::ExprManager& em);
	Expr getExpr(CVC4::ExprManager& em, const OperandVar& o);
	Expr getExpr(CVC4::ExprManager& em, const OperandMem& o); //, const Expr& expr_addr);
	inline Expr getExprSP() const { return expr_sp; }

private:
	AVLMap<t::int32, Expr> varmap; // registers, tempvars (these should be invalidated prior to SMT call though)
	AVLMap<Constant, Expr> memmap; // relative addresses in memory
	const CVC4::Type integer; // Z
	Expr expr_sp;
};

#endif
