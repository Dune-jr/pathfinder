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
	VariableStack(CVC4::ExprManager& em);
	Expr getExpr(CVC4::ExprManager& em, const OperandVar& o);
	Expr getExpr(CVC4::ExprManager& em, const OperandMem& o); //, const Expr& expr_addr);
	inline Expr getExprSP() const { return expr_sp; }

private:
	// outdated: lonely vars such as [?13] are identified as [?13+0] (makes sense that the SMT doesn't differentiate these two)
	AVLMap<t::int32, Expr> varmap; // registers, tempvars (these should be invalidated prior to SMT call though)
	// AVLMap<t::int32, Expr> memmap_absolute; // absolute addresses in memory
	// AVLMap<t::int32, Expr> memmap_relative; // relative addresses in memory
	AVLMap<Constant, Expr> memmap; // relative addresses in memory
	Expr expr_sp;

	/*
	struct relative_address
	{	// Example for [?13+0x8]
		t::int32 variable; // 13
		t::int32 constant; // 0x8

		inline bool operator==(relative_address x) const
		{
			return (variable == x.variable) && (constant == x.constant);
		}
		inline bool operator>(relative_address x) const
		{
			// arbitrary order: variables come first
			if(variable > x.variable)
				return constant > x.constant;
			return variable > x.variable;
		}
	};
	*/
};

#endif
