#include <cvc4/expr/expr_manager.h>
#include "cvc4_operand_visitor.h"

using namespace CVC4::kind;
using CVC4::Expr;

CVC4OperandVisitor::CVC4OperandVisitor(CVC4::ExprManager &em_, CVC4VariableStack &variables_)
	: visited(false), em(em_), variables(variables_) { }

Expr CVC4OperandVisitor::result()
{
	assert(visited);
	return expr;
}

bool CVC4OperandVisitor::visit(const class OperandConst& o)
{
	if(!o.value().isValid())
		return false; // fail
	if((o.value().isAbsolute()))
		expr = em.mkConst(CVC4::Rational(o.value().val()));
	if((o.value().isRelative()))
		expr = em.mkExpr(o.value().isRelativePositive() ? PLUS : MINUS, em.mkConst(CVC4::Rational(o.value().val())), variables.getExprSP());
	visited = true;
	return true;
}

bool CVC4OperandVisitor::visit(const class OperandVar& o)
{
	expr = variables.getExpr(em, o);
	visited = true;
	return true;
}

bool CVC4OperandVisitor::visit(const class OperandMem& o)
{
	//o.addr().accept(*this); // fetch expr from address of o
	if(!o.getConst().value().isValid())
		return false; // fail
	expr = variables.getExpr(em, o);//, expr);
	visited = true;
	return true;
}

bool CVC4OperandVisitor::visit(const class OperandTop& o)
{
	ASSERT(false);
	// expr = variables.getExpr(em, o);
	visited = true;
	return true;
}

bool CVC4OperandVisitor::visit(const class OperandArithExpr& o)
{
	if(!o.isComplete())
		return false; // fail
	Kind_t kind = getKind(o.opr());
	if(!o.leftOperand().accept(*this))
		return false;
	Expr expr_left = expr;
	
	if(o.isBinary())
	{
		if(!o.rightOperand().accept(*this))
			return false;
		Expr expr_right = expr;
		if(o.opr() == ARITHOPR_MULH) // special case: divide result by 2^32
			expr = em.mkExpr(getKind(ARITHOPR_DIV), em.mkExpr(kind, expr_left, expr_right), em.mkConst(CVC4::Rational(0x100000000ul)));
		else
			expr = em.mkExpr(kind, expr_left, expr_right);
	}
	else
		expr = em.mkExpr(kind, expr_left); // this is the unary version of mkExpr
		
	visited = true;
	return true;
}

Kind_t CVC4OperandVisitor::getKind(arithoperator_t opr)
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
		case ARITHOPR_MULH:
			return MULT; // will be handled separately by caller
			break;
		case ARITHOPR_DIV:
			// return DIVISION; // real version
			// return INTS_DIVISION_TOTAL;
			return INTS_DIVISION; // this is a version without interpreted division by 0
			break;
		case ARITHOPR_MOD:
			// return INTS_MODULUS_TOTAL;
			return INTS_MODULUS; // this is a version without interpreted division by 0
			break;
		case ARITHOPR_CMP:
			ASSERTP(false, "non-complete operator");
		default:
			ASSERT(false);
	}
}

