// Interfacing with the SMT solver

#include <elm/genstruct/SLList.h>
#include "smt.h"
#include "smt_variable_stack.h"
#include "analysis.h"
#include "debug.h"

// TODO: TEST Expr simplify(const Expr& e) 

using CVC4::Expr;
using namespace CVC4::kind;

SMT::SMT(): smt(&em), integer(em.integerType())
{
	smt.setLogic("QF_LIRA"); // Linear Incremental Arith Logic?
}

bool SMT::seekInfeasiblePaths(const SLList<Analysis::LabelledPredicate>& labelled_preds, SLList<Analysis::Path>& infeasible_paths)
{
	if(checkPredSat(labelled_preds))
		return false; // no inconsistency found
	
	//SLList<Analysis::Path> paths;
	//// we need to find out the concerned paths
	//for(SLList<Analysis::LabelledPredicate>::Iterator iter(labelled_preds); iter; iter++)
	//{
		//Analysis::Path current_path;
	//}
	
	return false; // TODO
}

// check predicates satisfiability
bool SMT::checkPredSat(const SLList<Analysis::LabelledPredicate>& labelled_preds)
{
	for(SLList<Analysis::LabelledPredicate>::Iterator iter(labelled_preds); iter; iter++)
	{
		Predicate pred = (*iter).pred();		
		Expr expr;
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

bool SMT::getExpr(Expr* expr, const Predicate& p)
{
	const Operand& opd1 = p.leftOperand();
	const Operand& opd2 = p.rightOperand();
	if(!(opd1.isComplete() && opd2.isComplete()))
		return false;
	
	Kind_t kind = getKind(p);
	*expr = em.mkExpr(kind, getExpr(opd1), getExpr(opd2));
	return true;
}

// must not be called if operand is not complete!
Expr SMT::getExpr(const Operand& o)
{
	assert(o.isComplete());
	SMTOperandVisitor visitor(em, variables);
	o.accept(visitor);
	return visitor.result();
}

Kind_t SMT::getKind(condoperator_t opr)
{
	switch(opr)
	{
		case CONDOPR_LT:
			return LT;
		case CONDOPR_LE:
			return LEQ;
		case CONDOPR_EQ:
			return EQUAL;
		case CONDOPR_NE:
			return DISTINCT;
		default:
			assert(false);
	}
}
