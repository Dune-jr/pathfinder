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

// using namespace elm::genstruct;
using namespace CVC4::kind;
using CVC4::Expr;

class CVC4SMT : public SMT
{
public:
	CVC4SMT(int flags);
	static inline elm::String name() { return "cvc4"; }
	
private:
	CVC4::ExprManager em;
	CVC4::SmtEngine smt;
	CVC4VariableStack variables;
	SLList<Option<Expr> > exprs;

	// SMT virtual pure methods
	void initialize(const SLList<LabelledPredicate>& labelled_preds);
	void initialize(const LocalVariables& lv, const HashTable<Constant, const Operand*, ConstantHash>& mem, DAG& dag);
	bool checkPredSat();
	bool retrieveUnsatCore(Analysis::Path& path, const SLList<LabelledPredicate>& labelled_preds, std::basic_string<char>& unsat_core_output);
	// bool checkPredSat(const SLList<LabelledPredicate>& labelled_preds);
	Option<Expr> getExpr(const Predicate& p);
	Option<Expr> getExpr(const Operand& o);
	inline Expr getRegExpr(t::int32 reg_id)
		{ return variables.getExpr(em, OperandVar(reg_id), VARIABLE_PREFIX); }
	inline Expr getMemExpr(Constant mem_id)
		{ return variables.getExpr(em, OperandMem(mem_id), VARIABLE_PREFIX); }
	Kind_t getKind(const Predicate& p) const;

};

#endif
