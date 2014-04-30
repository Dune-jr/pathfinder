// Interfacing with the SMT solver

#ifndef _SMT_H
#define _SMT_H

#include <cvc4/expr/expr_manager.h>
#include <cvc4/smt/smt_engine.h>
#include "analysis.h"

using namespace elm::genstruct;

class SMT
{
public:
	SMT();
	bool seekInfeasiblePaths(const SLList<Analysis::LabelledPredicate>& labelled_preds, SLList<Analysis::Path>& infeasible_paths);
	bool checkPredSat(const SLList<Analysis::LabelledPredicate>& labelled_preds);
	
private:
	CVC4::ExprManager em;
	CVC4::SmtEngine smt;
	
	bool getExpr(CVC4::Expr* expr, const Predicate& p);
	CVC4::Expr getExpr(const Operand& o);
	CVC4::Expr getExpr(const OperandConst& o);
	CVC4::Expr getExpr(const OperandVar& o);
	CVC4::Expr getExpr(const OperandArithExpr& o);
	
	       CVC4::kind::Kind_t getKind(arithoperator_t opr);
	inline CVC4::kind::Kind_t getKind(const OperandArithExpr& o) { return getKind(o.opr()); }
	       CVC4::kind::Kind_t getKind(condoperator_t opr);
	inline CVC4::kind::Kind_t getKind(const Predicate& p) { return getKind(p.opr()); }
	
	const CVC4::Type integer; // Z
};

#endif
