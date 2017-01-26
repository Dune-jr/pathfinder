/**
 * Implementing CVC4 as the SMT solver
 */
#include <cvc4/expr/command.h> // getUnsatCoreCommand
#include <cvc4/util/unsat_core.h>
#include <elm/util/BitVector.h>
#include "../operand.h"
#include "../debug.h"
#include "cvc4_smt.h"

using namespace CVC4::kind;
using elm::BitVector;
using CVC4::Expr;

CVC4SMT::CVC4SMT(int flags): SMT(flags), smt(&em), variables(em)
{
	smt.setLogic("QF_LIA"); // Quantifier-Free (no forall, exists...) Linear Integer Arithmetic
	smt.setOption("incremental", CVC4::SExpr("false")); // non incremental
	smt.setOption("produce-unsat-cores", CVC4::SExpr("true"));
	smt.setOption("rewrite-divk", CVC4::SExpr("true"));
	// smt.setOption("dump-unsat-cores", CVC4::SExpr("true"));
	// smt.setOption("produce-proofs", CVC4::SExpr("true"));
	// 
	// smt.setOption("dump", "assertions:pre-everything");
	// smt.setOption("dump-to", "dump.log"); // this is actually global to CVC4... meaning setting it once per pathfinder execution is enough
}

// v1: all VARIABLE_PREFIX "?k"
// v2: INITIAL: "rk", VARIABLE: "?k". Predicates like "?0 = r0 + 1"
void CVC4SMT::initialize(const SLList<LabelledPredicate>& labelled_preds)//, mode_t mode = VARIABLE_PREFIX)
{
	variables.setMode(VARIABLE_PREFIX);
	for(SLList<LabelledPredicate>::Iterator iter(labelled_preds); iter; iter++)
		exprs.addLast(getExpr(iter->pred()));
}

// static int saved_useless_asserts = 0;
void CVC4SMT::initialize(const LocalVariables& lv, const HashTable<Constant, const Operand*, ConstantHash>& mem, DAG& dag)
{
	variables.setMode(INITIAL_PREFIX);
	BitVector used_regs(lv.maxRegisters(), false);

// experimental
#ifndef DONTCHECK_USELESS_ASSERTS
	Vector<const Operand*> v;
	for(HashTable<Constant, const Operand*, ConstantHash>::KeyIterator iter(mem); iter; iter++)
		v.push(dag.mem(*iter));
	const int key_nb = v.count();
	for(HashTable<Constant, const Operand*, ConstantHash>::Iterator iter(mem); iter; iter++)
		for(Vector<const Operand*>::Iter vi(v); vi; )
		{
			if((*iter)->involvesOperand(**vi))
				v.remove(vi); // not useless
			else
				vi++;
		}
	for(int i = 0; i < lv.maxRegisters(); i++) // registers id are [0...n[
		for(Vector<const Operand*>::Iter vi(v); vi; )
		{
 			if(lv(i).involvesOperand(**vi))
				v.remove(vi); // not useless
			else
				vi++;
		}
	ASSERTP(key_nb == v.count(), "finally")
	// cout << v.count() << "/" << key_nb << " useless" << endl;
	// saved_useless_asserts += v.count();
#endif

	for(HashTable<Constant, const Operand*, ConstantHash>::PairIterator iter(mem); iter; iter++)
	{
#ifndef DONTCHECK_USELESS_ASSERTS
		if(v.contains(dag.mem((*iter).fst))) // useless assert
		{
			exprs += elm::none;
			continue;
		}
#endif
		exprs += getExpr(Predicate(CONDOPR_EQ, dag.mem((*iter).fst), (*iter).snd));
 		// exprs += em.mkExpr(EQUAL, dag.mem((*iter).fst), (*iter).snd); // TODO!! I forgot why... I think cuz [SP+8] = [SP+8] + 1. need example
		(*iter).snd->markUsedRegisters(used_regs);
	}
	for(int i = 0; i < lv.maxRegisters(); i++) // registers id are [0...n[
	{
 		if(used_regs[i] || lv[i])
 			if(Option<Expr> expr_right = getExpr(lv(i)))
 				exprs += em.mkExpr(EQUAL, getRegExpr(i), expr_right);
	}
}

// check predicates satisfiability
bool CVC4SMT::checkPredSat()
{
	try {
		// std::time_t timestamp = clock(); // Timestamp before analysis
		for(SLList<Option<Expr> >::Iterator iter(exprs); iter; iter++)
			if(*iter) {
				smt.assertFormula(**iter, true); // second parameter to true for unsat cores
				// std::cout << **iter << endl; // uncomment to print all asserted predicates
			}
		bool isSat = smt.checkSat(em.mkConst(true), true).isSat(); // check satisfability, the second parameter enables unsat cores
		
		if(!isSat) {
			if(dbg_&0x1)
				std::cout << "[[[" << smt.getUnsatCore() << "]]]" << endl;
			if(dbg_&0x2)
				{ char c; cin >> c; cin >> c; }
			if(dbg_&0x4)
				abort();
		}

		// timestamp = (clock()-timestamp)*1000*1000/CLOCKS_PER_SEC;
		// smt.getStatistics().flushInformation((std::ostream&)std::cout);
		/*
		std::cerr << "DEBUG:\t" <<
			exprs.count() << "\t" <<
			smt.getStatistic("smt::SmtEngine::processAssertionsTime").getValue() << "\t" <<
			smt.getStatistic("smt::SmtEngine::theoryPreprocessTime").getValue() << "\t" <<
			smt.getStatistic("smt::SmtEngine::solveTime").getValue() << "\t" <<
			timestamp <<
		endl;
		*/
		return isSat;
	}
	catch(CVC4::LogicException e)
	{
#ifdef DBG_WARNINGS
		DBGW("non-linear call to CVC4, defaulting to SAT:")
		std::cerr << e;
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
	if(!p.isComplete() || (flags&Analysis::SMT_CHECK_LINEAR && !p.isLinear(!(flags&Analysis::ALLOW_NONLINEAR_OPRS))))
		return elm::none;
	return em.mkExpr(getKind(p), *getExpr(p.leftOperand()), *getExpr(p.rightOperand()));
}

Option<Expr> CVC4SMT::getExpr(const Operand& o)
{
	if(flags&Analysis::SMT_CHECK_LINEAR && !o.isLinear(!(flags&Analysis::ALLOW_NONLINEAR_OPRS)))
		return elm::none;
	// cout << "\e[0;92m" << (const string)(_ << o) << "\e[0;m" << " (" << o.isLinear() << ")" << endl;
	CVC4OperandVisitor visitor(em, variables);
	if(!o.accept(visitor))
		return elm::none;
	return elm::some(visitor.result());
}

Expr CVC4SMT::getRegExpr(t::int32 reg_id)
{
	return variables.getExpr(em, OperandVar(reg_id), VARIABLE_PREFIX);
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
			abort();
	}
}
