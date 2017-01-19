/*
 * Interfacing with the SMT solver
 */
#ifndef _SMT_H
#define _SMT_H

// check for consistent SMT options
#ifdef SMT_SOLVER_CVC4
	#ifdef SMT_SOLVER_Z3
		#error "Conflict: several SMT solvers specified!"
	#endif
#else
	#ifndef SMT_SOLVER_Z3
		#error "No SMT solver specified!"
	#endif
#endif

#include "analysis_state.h"
#include "DAG.h"

class SMT
{
public:
	SMT(int flags);
	Option<Analysis::Path*> seekInfeasiblePaths(const Analysis::State& s);
	Option<Analysis::Path*> seekInfeasiblePathsv2(const Analysis::State& s);
	static const elm::String printChosenSolverInfo();
	
private:
	virtual void initialize(const SLList<LabelledPredicate>& labelled_preds) = 0;
	virtual void initialize(const LocalVariables& lv, const HashTable<Constant, const Operand*, ConstantHash>& mem, DAG& dag) = 0;
	virtual bool checkPredSat() = 0;
	virtual bool retrieveUnsatCore(Analysis::Path& path, const SLList<LabelledPredicate>& labelled_preds, std::basic_string<char>& unsat_core_output) = 0;

protected:
	int flags;
};

#endif
