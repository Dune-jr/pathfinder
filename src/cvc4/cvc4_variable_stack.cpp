#include <cvc4/expr/expr_manager.h>
#include <elm/string/String.h>
#include <elm/util/Option.h>
#include "../analysis.h"
#include "cvc4_variable_stack.h"

using CVC4::Expr;

CVC4VariableStack::CVC4VariableStack(CVC4::ExprManager& em) : integer(em.integerType())
{
	expr_sp = em.mkVar("SP", integer);
}

Expr CVC4VariableStack::getExpr(CVC4::ExprManager& em, const OperandVar& o)
{
	const t::int32 addr = o.addr();
	if(Option<Expr> opt_expr = varmap.get(addr))
		return *opt_expr; // already in the stack
	else
	{	// not in stack, create it
		elm::String label = _ << o;
		Expr expr = em.mkVar(label.chars(), integer);
		varmap.put(addr, expr);
		return expr;
	}
}

// o must have valid addr
Expr CVC4VariableStack::getExpr(CVC4::ExprManager& em, const OperandMem& o) //, const Expr& expr_addr)
{
	const Constant& addr = o.getConst().value();
	ASSERT(addr.isValid());
	int sp_factor = 0;
	if(addr.isRelative())
		sp_factor += (addr.isRelativePositive()) ? 1 : -1;

	if(Option<Expr> opt_expr = memmap.get(addr))
		return *opt_expr; // already in the stack
	else
	{	// not in stack, create it
		elm::String label;
		switch(sp_factor)
		{	// these must be unique labels
			case -1:
				label = _ << "[-SP+" << addr.val() << "]";
				break;
			case 0:
				label = _ << "[" << addr.val() << "]";
				break;
			case +1:
				label = _ << "[SP+" << addr.val() << "]";
				break;
			default:
				ASSERT(false);
		}
		Expr expr = em.mkVar(label.chars(), integer);
		memmap.put(addr, expr);
		return expr;
	}
}
