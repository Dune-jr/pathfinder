#include "smt_variable_stack.h"
#include <cvc4/expr/expr_manager.h>
#include <elm/string/String.h>
#include <elm/util/Option.h>
#include "analysis.h"

using CVC4::Expr;

VariableStack::VariableStack() { }

Expr VariableStack::getVariableExpr(const OperandVar& o, CVC4::ExprManager& em)
{
	t::int32 addr = o.getAddr();
	if(Option<Expr> opt_expr = map.get(addr))
		// variable is already in the stack
		return *opt_expr;
	else
	{
		// variable not in stack, create it
		elm::String label = _ << o;
		Expr expr = em.mkVar(label.chars(), em.integerType());
		map.put(addr, expr);
		return expr;
	}
}
