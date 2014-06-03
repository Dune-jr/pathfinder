#include "smt_variable_stack.h"
#include <cvc4/expr/expr_manager.h>
#include <elm/string/String.h>
#include <elm/util/Option.h>
#include "analysis.h"

using CVC4::Expr;

VariableStack::VariableStack() { }

Expr VariableStack::getExpr(CVC4::ExprManager& em, const OperandVar& o)
{
	const t::int32 addr = o.addr();
	if(Option<Expr> opt_expr = varmap.get(addr))
		return *opt_expr; // already in the stack
	else
	{	// not in stack, create it
		elm::String label = _ << o;
		Expr expr = em.mkVar(label.chars(), em.integerType());
		varmap.put(addr, expr);
		return expr;
	}
}

//
Expr VariableStack::getExpr(CVC4::ExprManager& em, const OperandMem& o) //, const Expr& expr_addr)
{
	if(o.hasConst())
	{	// case: absolute addr
		const t::int32 addr = o.getConst().value();
		if(Option<Expr> opt_expr = memmap_absolute.get(addr))
			return *opt_expr; // already in the stack
		else
		{	// not in stack, create it
			elm::String label = _ << o;
			Expr expr = em.mkVar(label.chars(), em.integerType());
			memmap_absolute.put(addr, expr);
			return expr;
		}
	}
	else
	{	// case: relative or variable (no const) addr
		relative_address addr;
		if(o.hasConst())
			addr.constant = o.getConst().value();
		else
			addr.constant = 0;
		addr.variable = o.getVar().addr();
		if(Option<Expr> opt_expr = memmap_relative.get(addr))
			return *opt_expr; // already in the stack
		else
		{	// not in stack, create it
			elm::String label = _ << o;
			Expr expr = em.mkVar(label.chars(), em.integerType());
			memmap_relative.put(addr, expr);
			return expr;
		}
	}
}

// bool VariableStack::operator>(relative_address x, relative_address y)
// {
// 	// arbitrary order: variables come first
// 	if(x.variable == y.variable)
// 		return x.constant > y.constant;
// 	return x.variable > y.variable;
// }