#ifndef _SMT_VARIABLE_STACK
#define _SMT_VARIABLE_STACK

#include <cvc4/expr/expr_manager.h>
#include <elm/genstruct/SLList.h>
#include "operand.h"

using namespace elm::genstruct;

class VariableStack
{
public:
	VariableStack();
	
	class VariableExpr
	{
	public:
		VariableExpr(t::int32 addr, CVC4::Expr expr);
		inline t::int32	  addr() const { return _addr; }
		inline CVC4::Expr expr() const { return _expr; }
		
	private:
		t::int32 _addr;
		CVC4::Expr _expr;
	};
	
	CVC4::Expr getVariableExpr(const OperandVar& o, CVC4::ExprManager& em);
	
private:
	SLList<VariableExpr> var_list;
};

#endif
