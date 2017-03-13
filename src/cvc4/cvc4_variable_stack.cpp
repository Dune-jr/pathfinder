#include <cvc4/expr/expr_manager.h>
#include <elm/string/String.h>
#include <elm/util/Option.h>
#include "../analysis.h"
#include "cvc4_variable_stack.h"

using CVC4::Expr;

CVC4VariableStack::CVC4VariableStack(CVC4::ExprManager& em) : integer(em.integerType()), def_mode(VARIABLE_PREFIX)
{
	expr_sp = em.mkVar("SP", integer);
}

Expr CVC4VariableStack::getExpr(CVC4::ExprManager& em, const OperandVar& o, stackmode_t mode)
{
	const stackmode_t effective_mode = mode != DEFAULT ? mode : def_mode;
	ASSERTP(!o.isTempVar(), "getExpr was given a tempvar")
	const t::int32 addr = o.addr();
	if(Option<Expr> opt_expr = regmap.get(getRegId(addr, mode)))
		return *opt_expr; // already in the stack
	else
	{	// not in stack, create it
		elm::String label = _ << getRegChar(effective_mode) << addr;
		Expr expr = em.mkVar(label.chars(), integer);
		regmap.put(getRegId(addr, effective_mode), expr);
		return expr;
	}
}

// o must have valid addr
Expr CVC4VariableStack::getExpr(CVC4::ExprManager& em, const OperandMem& o, stackmode_t mode) //, const Expr& expr_addr)
{
	const Constant& addr = o.addr().value();
	const stackmode_t effective_mode = mode != DEFAULT ? mode : def_mode;
	ASSERT(addr.isValid());
	int sp_factor = 0;
	if(addr.isRelative())
		sp_factor = (addr.isRelativePositive()) ? 1 : -1;

	if(Option<Expr> opt_expr = memmap.get(getMemId(addr, effective_mode)))
		return *opt_expr; // already in the stack
	else
	{	// not in stack, create it
		elm::String label;
		switch(sp_factor)
		{	// these must be unique labels
			case -1:
				ASSERTP(false, "memory cell at -SP+k in SMT call!")
				label = _ << "[-SP+" << addr.val() << "]" << getMemChar(effective_mode);
				break;
			case 0:
				label = _ << "[" << addr.val() << "]" << getMemChar(effective_mode);
				break;
			case +1:
				label = _ << "[SP" << (addr.val() >= 0 ? "+" : "") << addr.val() << "]" << getMemChar(effective_mode);
				break;
			default:
				abort();
		}
		Expr expr = em.mkVar(label.chars(), integer);
		memmap.put(getMemId(addr, effective_mode), expr);
		return expr;
	}
}

Expr CVC4VariableStack::getExpr(CVC4::ExprManager& em, const OperandTop& o)
{
	const t::int32 id = (t::int32)o.getId();
	if(Option<Expr> opt_expr = topmap.get(id))
		return *opt_expr; // already in the stack
	else
	{	// not in stack, create it
		elm::String label = _ << o;
		Expr expr = em.mkVar(label.chars(), integer);
		topmap.put(id, expr);
		return expr;
	}
}

Expr CVC4VariableStack::getExpr(CVC4::ExprManager& em, const OperandIter& o)
{
	t::intptr k = (t::intptr)o.loop();
	if(o.isDone())
		k = ~k;
	if(Option<Expr> opt_expr = itermap.get(k))
		return *opt_expr; // already in the stack
	else
	{	// not in stack, create it
		elm::String label = _ << o;
		Expr expr = em.mkVar(label.chars(), integer);
		itermap.put(k, expr);
		return expr;
	}
}
