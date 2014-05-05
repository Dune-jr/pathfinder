#include "smt_variable_stack.h"
#include <cvc4/expr/expr_manager.h>
#include <elm/string/String.h>
#include "analysis.h"

using CVC4::Expr;

VariableStack::VariableStack() { }

CVC4::Expr VariableStack::getVariableExpr(const OperandVar& o, CVC4::ExprManager& em)
{
	t::int32 addr = o.getAddr();
	for(SLList<VariableExpr>::Iterator iter(var_list); iter; iter++)
	{
		if(addr == (*iter).addr())
			return (*iter).expr();
	}
	
	// variable not already in stack, create it
	elm::String label = _ << o;
	Expr expr = em.mkVar(label.chars(), em.integerType());
	var_list += VariableExpr(addr, expr);
	return expr;
}

VariableStack::VariableExpr::VariableExpr(t::int32 addr, Expr e) : _addr(addr), _expr(e) { }
