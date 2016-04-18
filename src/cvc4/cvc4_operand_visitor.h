#ifndef _CVC4_CVC4_OPERAND_VISITOR_H
#define _CVC4_CVC4_OPERAND_VISITOR_H

#include "cvc4_variable_stack.h"

using namespace CVC4::kind;

class CVC4OperandVisitor : public OperandVisitor
{
public:
	CVC4OperandVisitor(CVC4::ExprManager &em, CVC4VariableStack &variables);
	CVC4::Expr result();		
	bool visit(const class OperandConst& o);
	bool visit(const class OperandVar& o);
	bool visit(const class OperandMem &o);
	bool visit(const class OperandArithExpr& o);
	
private:
		   Kind_t getKind(arithoperator_t opr);
	inline Kind_t getKind(const OperandArithExpr& o) { return getKind(o.opr()); }

	bool visited;
	CVC4::ExprManager &em;
	CVC4VariableStack &variables;
	CVC4::Expr expr;
};

#endif
