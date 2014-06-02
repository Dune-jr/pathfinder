#ifndef _SMT_VARIABLE_STACK
#define _SMT_VARIABLE_STACK

#include <cvc4/expr/expr_manager.h>
#include <elm/genstruct/SLList.h>
#include <elm/genstruct/AVLMap.h>
#include "operand.h"

using namespace elm::genstruct;
using CVC4::Expr;

class VariableStack
{
public:
	VariableStack();
	Expr getExpr(CVC4::ExprManager& em, const OperandVar& o);
	Expr getExpr(CVC4::ExprManager& em, const OperandMem& o); //, const Expr& expr_addr);
	
private:
	AVLMap<t::int32, Expr> varmap; // tempvars, registers
	AVLMap<const Operand&, Expr> memmap; // content of addresses in memory (stack-relative or absolute)
};

#endif
