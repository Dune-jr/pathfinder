#ifndef _SMT_OPERAND_VISITOR_H
#define _SMT_OPERAND_VISITOR_H

#include "smt_variable_stack.h"

using namespace CVC4::kind;

class SMTOperandVisitor : public OperandVisitor
{
public:
	SMTOperandVisitor(CVC4::ExprManager &em, VariableStack &variables);
	CVC4::Expr result();		
	bool visit(const OperandConst& o);
	bool visit(const OperandVar& o);
	bool visit(const OperandMem &o);
	bool visit(const OperandArithExpr& o);
	
private:
		   Kind_t getKind(arithoperator_t opr);
	inline Kind_t getKind(const OperandArithExpr& o) { return getKind(o.opr()); }

	bool visited;
	CVC4::ExprManager &em;
	VariableStack &variables;
	CVC4::Expr expr;
};

#endif
