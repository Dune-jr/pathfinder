/*
 * Implementing CVC4 as the SMT solver
 */
#ifndef _CVC4_CVC4_SMT_H
#define _CVC4_CVC4_SMT_H

#include <cvc4/expr/expr_manager.h>
#include <cvc4/smt/smt_engine.h>
#include "../analysis.h" // Analyis::Path
#include "../smt.h" // abstract SMT class inheritance
#include "cvc4_operand_visitor.h"
#include "cvc4_variable_stack.h"

using namespace elm::genstruct;
using namespace CVC4::kind;
using CVC4::Expr;

class CVC4SMT : public SMT
{
public:
	CVC4SMT();
	
private:
	CVC4::ExprManager em;
	CVC4::SmtEngine smt;
	CVC4VariableStack variables;
	SLList<Option<Expr> > exprs;

	// SMT virtual pure methods
	bool checkPredSat(const SLList<LabelledPredicate>& labelled_preds);
	bool retrieveUnsatCore(Analysis::Path& path, const SLList<LabelledPredicate>& labelled_preds, std::basic_string<char>& unsat_core_output);
	// bool checkPredSat(const SLList<LabelledPredicate>& labelled_preds);
	Option<Expr> getExpr(const Predicate& p);
	Option<Expr> getExpr(const Operand& o);
	Kind_t getKind(const Predicate& p) const;
	
	const CVC4::Type integer; // Z // TODO!! try with rationals instead to improve computation time!
};

#endif
