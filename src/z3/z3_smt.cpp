/*
 * Implementing Z3 as the SMT solver
 */
#include <elm/genstruct/SLList.h>
// #include <elm/util/BitVector.h>
// #include "../operand.h"
#include "../debug.h"
#include "z3_smt.h"

Z3SMT::Z3SMT(): s(c), p(c)
{
	p.set("unsat_core", true);
	s.set(p);
}

// check predicates satisfiability
bool Z3SMT::checkPredSat(const SLList<LabelledPredicate>& labelled_preds)
{
	/*
	// get a SLList<Option<Expr> > out of a SLList<LabelledPredicate> in order to know which LP matches which expr // TODO: this is superfluous with Z3
	for(SLList<LabelledPredicate>::Iterator iter(labelled_preds); iter; iter++)
		exprs.addLast(getExpr(iter->pred()));

	try
	{
		for(SLList<Option<Expr> >::Iterator iter(exprs); iter; iter++)
			if(*iter)
				smt.assertFormula(**iter, true); // second parameter to true for unsat cores

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
	*/
	return true;
}

// get unsat core and build a shortened path accordingly
// rtn false if failure, true otherwise
bool Z3SMT::retrieveUnsatCore(Analysis::Path& path, const SLList<LabelledPredicate>& labelled_preds, std::basic_string<char>& unsat_core_output)
{
	/*
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
	return !empty;
	*/
	return false;
}


    /*
    context c;
    expr x  = c.int_const("x");
    expr y  = c.int_const("y");
    solver s(c);

    // enabling unsat core tracking
    params p(c);
    p.set("unsat_core", true);
    s.set(p);

    // The following assertion will not be tracked.
    s.add(x > 0);

    // The following assertion will be tracked using Boolean variable p1.
    // The C++ wrapper will automatically create the Boolean variable.
    s.add(y > 0, "p1");

    // Asserting other tracked assertions.
    s.add(x < 10, "p2");
    s.add(y < 0,  "p3");

    std::cout << s.check() << "\n";
    std::cout << s.unsat_core() << "\n";
    */