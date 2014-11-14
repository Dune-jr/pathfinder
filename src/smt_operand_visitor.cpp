#include <cvc4/expr/expr_manager.h>
#include "smt_operand_visitor.h"

using namespace CVC4::kind;
using CVC4::Expr;

SMTOperandVisitor::SMTOperandVisitor(CVC4::ExprManager &em_, VariableStack &variables_)
	: visited(false), em(em_), variables(variables_) { }

Expr SMTOperandVisitor::result()
{
	assert(visited);
	return expr;
}

bool SMTOperandVisitor::visit(const class OperandConst& o)
{
	if(!o.value().isValid())
		return false; // fail
	if((o.value().isAbsolute()))
		expr = em.mkConst(CVC4::Rational(o.value().val()));
	if((o.value().isRelative()))
		expr = em.mkExpr(o.value().isPositive() ? PLUS : MINUS, em.mkConst(CVC4::Rational(o.value().val())), variables.getExprSP());
	visited = true;
	return true;
}

bool SMTOperandVisitor::visit(const class OperandVar& o)
{
	expr = variables.getExpr(em, o);
	visited = true;
	return true;
}

bool SMTOperandVisitor::visit(const class OperandMem& o)
{
	//o.addr().accept(*this); // fetch expr from address of o
	if(!o.getConst().value().isValid())
		return false; // fail
	expr = variables.getExpr(em, o);//, expr);
	visited = true;
	return true;
}

bool SMTOperandVisitor::visit(const class OperandArithExpr& o)
{
	if(!o.isComplete())
		return false; // fial
	Kind_t kind = getKind(o.opr());
	if(!o.leftOperand().accept(*this))
		return false;
	Expr expr_left = expr;
	
	if(o.isBinary())
	{
		if(!o.rightOperand().accept(*this))
			return false;
		Expr expr_right = expr;	
		expr = em.mkExpr(kind, expr_left, expr_right);
	}
	else
		expr = em.mkExpr(kind, expr_left); // this is the unary version of mkExpr
		
	visited = true;
	return true;
}

Kind_t SMTOperandVisitor::getKind(arithoperator_t opr)
{
	switch(opr)
	{
		case ARITHOPR_NEG:
			return UMINUS;
			break;
		case ARITHOPR_ADD:
			return PLUS;			
			break;
		case ARITHOPR_SUB:
			return MINUS;
			break;
		case ARITHOPR_MUL:
			return MULT;
			break;
		case ARITHOPR_DIV:
			// return INTS_DIVISION_TOTAL;
			return INTS_DIVISION; // this is a version without interpreted division by 0
			break;
		case ARITHOPR_MOD:
			// return INTS_MODULUS_TOTAL;
			return INTS_MODULUS; // this is a version without interpreted division by 0
			break;
		default:
			// non-complete operator!
			assert(false);
	}
}

