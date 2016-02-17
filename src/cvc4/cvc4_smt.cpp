/*
 * Implementing CVC4 as the SMT solver
 */
#include <cvc4/expr/command.h> // getUnsatCoreCommand
#include <cvc4/util/unsat_core.h>
#include <elm/genstruct/SLList.h>
#include <elm/util/BitVector.h>
#include "../operand.h"
#include "../debug.h"
#include "cvc4_smt.h"

using namespace CVC4::kind;
using elm::BitVector;
using CVC4::Expr;

CVC4SMT::CVC4SMT(): smt(&em), variables(em)
{
	smt.setLogic("QF_LIA"); // Quantifier-Free (no forall, exists...) Linear Integer Arithmetic
	smt.setOption("incremental", CVC4::SExpr("false")); // non incremental
	smt.setOption("produce-unsat-cores", CVC4::SExpr("true"));
	smt.setOption("rewrite-divk", CVC4::SExpr("true"));
	// smt.setOption("dump-unsat-cores", CVC4::SExpr("true"));
	// smt.setOption("produce-proofs", CVC4::SExpr("true"));
	// smt.setOption("dump", "assertions:pre-everything");
	// smt.setOption("dump-to", "dump.log"); // this is actually global to CVC4... meaning setting it once per pathfinder execution is enough
}

// check predicates satisfiability
bool CVC4SMT::checkPredSat(const SLList<LabelledPredicate>& labelled_preds)
{
	// get a SLList<Option<Expr> > out of a SLList<LabelledPredicate> in order to know which LP matches which expr // TODO: this is superfluous with Z3
	for(SLList<LabelledPredicate>::Iterator iter(labelled_preds); iter; iter++)
		exprs.addLast(getExpr(iter->pred()));
// elm::cout << "labelled_preds=" << labelled_preds << endl;
	try
	{
		for(SLList<Option<Expr> >::Iterator iter(exprs); iter; iter++)
			if(*iter) {
				smt.assertFormula(**iter, true); // second parameter to true for unsat cores
				// std::cout << "\tassertFormula(" << **iter << ")\n"; // TODO!!!
			}

		bool isSat = smt.checkSat(em.mkConst(true), true).isSat(); // check satisfability, the second parameter enables unsat cores
		return isSat;
	}
	catch(CVC4::LogicException e)
	{
		#ifdef DBG_WARNINGS
			cout << "WARNING: non-linear call to CVC4, defaulted to SAT." << endl;
		#endif
		return true;
	}
}

// get unsat core and build a shortened path accordingly
// rtn false if failure, true otherwise
bool CVC4SMT::retrieveUnsatCore(Analysis::Path& path, const SLList<LabelledPredicate>& labelled_preds, std::basic_string<char>& unsat_core_output)
{
	CVC4::UnsatCore unsat_core = smt.getUnsatCore(); // get an unsat subset of our assumptions
	bool empty = true;
	unsat_core_output = "[";
	for(CVC4::UnsatCore::const_iterator unsat_core_iter = unsat_core.begin(); unsat_core_iter != unsat_core.end(); unsat_core_iter++)
	{
		if(!empty)
			unsat_core_output += ", ";	
		unsat_core_output += (*unsat_core_iter).toString();
		empty = false;
		SLList<LabelledPredicate>::Iterator lp_iter(labelled_preds);
		SLList<Option<Expr> >::Iterator expr_iter(exprs);

		for(; lp_iter; lp_iter++, expr_iter++)
			if(*expr_iter && **expr_iter == *unsat_core_iter)
				path += (*lp_iter).labels();
	}
	unsat_core_output += "]";
	return !empty;
}


Option<Expr> CVC4SMT::getExpr(const Predicate& p)
{
	if(!p.isComplete())
		return elm::none;
	return em.mkExpr(getKind(p), getExpr(p.leftOperand()), getExpr(p.rightOperand()));
}

Option<Expr> CVC4SMT::getExpr(const Operand& o)
{
	if(!o.isComplete())
		return elm::none; // this could cause a crash
		
	CVC4OperandVisitor visitor(em, variables);
	if(!o.accept(visitor))
		return elm::none;
	return elm::some(visitor.result());
}

Kind_t CVC4SMT::getKind(const Predicate& p) const
{
	switch(p.opr())
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
