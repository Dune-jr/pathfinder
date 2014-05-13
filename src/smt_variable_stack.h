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
	Expr getVariableExpr(const OperandVar& o, CVC4::ExprManager& em);
	
private:
	AVLMap<t::int32, Expr> map;
};

#endif
