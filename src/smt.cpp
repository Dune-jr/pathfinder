// Interfacing with the SMT solver

#include <elm/genstruct/SLList.h>
#include "smt.h"
#include "analysis.h"
#include "debug.h"

// TODO: TEST Expr simplify(const Expr& e) 

SMT::SMT(): smt(&em), integer(em.integerType())
{
	smt.setLogic("QF_LIRA"); // Linear Incremental Arith Logic?
}

bool SMT::seekInfeasiblePaths(const SLList<Analysis::LabelledPredicate>& labelled_preds, SLList<Analysis::Path>& infeasible_paths)
{
	if(checkPredSat(labelled_preds))
		return false; // no inconsistency found
	
	//smt.push();
	//for(SLList<Analysis::LabelledPredicate>::Iterator iter(labelled_preds); iter; iter++)
	//{
		//Predicate pred = (*iter).pred();
		//const Edge* label = (*iter).label();
	//}	
	//smt.pop();
	
	return false; // TODO
}

// check predicates satisfiability
bool SMT::checkPredSat(const SLList<Analysis::LabelledPredicate>& labelled_preds)
{
	for(SLList<Analysis::LabelledPredicate>::Iterator iter(labelled_preds); iter; iter++)
	{
		Predicate pred = (*iter).pred();		
		CVC4::Expr expr;
		if(getExpr(&expr, pred))
		{
			DBG_STD(COLOR_IRed << "Assumption: " << expr)
			smt.assertFormula(expr);
		}
	}
	smt.push();
	// check satisfability
	CVC4::Result result = smt.checkSat(em.mkConst(true));
	DBG_STD(COLOR_IRed << "CheckSat: " << result)
	smt.pop();
	return result.isSat(); // TODO
}

bool SMT::getExpr(CVC4::Expr* expr, const Predicate& p)
{
	const Operand& opd1 = p.leftOperand();
	const Operand& opd2 = p.rightOperand();
	if(!(opd1.isComplete() && opd2.isComplete()))
		return false;
	
	CVC4::kind::Kind_t kind = getKind(p);
	*expr = em.mkExpr(kind, getExpr(opd1), getExpr(opd2));
	return true;
}

// must not be called if operand is not complete!
CVC4::Expr SMT::getExpr(const Operand& o)
{
	assert(o.isComplete());
	// TODO: this is sooo dirty, but I just can't find a proper way to do it!
	switch(o.kind())
	{
		case OPERAND_CONST:
			return getExpr((const OperandConst&) o);
		case OPERAND_VAR:
			return getExpr((const OperandVar&) o);
		case OPERAND_ARITHEXPR:
			return getExpr((const OperandArithExpr&) o);
		default:
			assert(false);
	}
}

CVC4::Expr SMT::getExpr(const OperandConst& o)
{
	return em.mkConst(CVC4::Rational(o.value()));
}

CVC4::Expr SMT::getExpr(const OperandVar& o)
{	
	// TODO: do we need unicity for labels?
	otawa::String label = _ << o;
	return em.mkVar(label.chars(), integer);
}
	
// must not be called if o is a ~ b (non-complete operand)
CVC4::Expr SMT::getExpr(const OperandArithExpr& o)
{
	assert(o.isComplete());
	CVC4::kind::Kind_t kind = getKind(o.opr());
	CVC4::Expr expr_left = getExpr(o.leftOperand());
	if(o.isBinary())
	{
		CVC4::Expr expr_right = getExpr(o.leftOperand());
		return em.mkExpr(kind, expr_left, expr_right);
	}
	else
	{
		CVC4::kind::Kind_t kind = getKind(o.opr());
		return em.mkExpr(kind, expr_left); // this is the unary version of mkExpr
	}
}

CVC4::kind::Kind_t SMT::getKind(arithoperator_t opr)
{
	switch(opr)
	{
		case ARITHOPR_NEG:
			return CVC4::kind::UMINUS;
			break;
		case ARITHOPR_ADD:
			return CVC4::kind::PLUS;			
			break;
		case ARITHOPR_SUB:
			return CVC4::kind::MINUS;
			break;
		case ARITHOPR_MUL:
			return CVC4::kind::MULT;
			break;
		case ARITHOPR_DIV:
			// return CVC4::kind::INTS_DIVISION_TOTAL;
			return CVC4::kind::INTS_DIVISION; // this is a version without interpreted division by 0
			break;
		case ARITHOPR_MOD:
			// return CVC4::kind::INTS_MODULUS_TOTAL;
			return CVC4::kind::INTS_MODULUS; // this is a version without interpreted division by 0
			break;
		default:
			// non-complete operator!
			assert(false);
	}
}

CVC4::kind::Kind_t SMT::getKind(condoperator_t opr)
{
	switch(opr)
	{
		case CONDOPR_LT:
			return CVC4::kind::LT;
		case CONDOPR_LE:
			return CVC4::kind::LEQ;
		case CONDOPR_EQ:
			return CVC4::kind::EQUAL;
		case CONDOPR_NE:
			return CVC4::kind::DISTINCT;
		default:
			assert(false);
	}
}

/*
SMT::~SMT()
{
	delete smt;
}
*/
