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

#include <elm/genstruct/SLList.h>
#include "analysis.h"

using namespace elm::genstruct;

//template<E>
class SMT
{
public:
	SMT();
	Option<Analysis::Path> seekInfeasiblePaths(const Analysis::State& s);
	
private:
	virtual bool checkPredSat(const SLList<LabelledPredicate>& labelled_preds) = 0;
	virtual bool retrieveUnsatCore(Analysis::Path& path, const SLList<LabelledPredicate>& labelled_preds, std::basic_string<char>& unsat_core_output) = 0;
	// Option<E> getExpr(const Predicate& p);
	// Option<E> getExpr(const Operand& o);

	/*	SLList<Analysis::Path> getAllInfeasiblePaths(const SLList<LabelledPredicate>& labelled_preds, int index = 0);
	void removeIncompletePredicates(SLList<LabelledPredicate>& labelled_preds);
	void genPathPointBitMaps(const SLList<Analysis::Path>& paths, AVLMap<const Edge*, unsigned int>& map_pathpoint_to_bit, Vector<const Edge*>& map_bit_to_pathpoint);
	SLList<BitVector> genBitCodes(const SLList<Analysis::Path>& paths, const AVLMap<const Edge*, unsigned int>& map_pathpoint_to_bit, unsigned int used_bits);
	BitVector getListOfPathsToKeep(const SLList<BitVector>& bitcode_list);
	SLList<Analysis::Path> filterPaths(const SLList<BitVector>& bitcode_list, const Vector<const Edge*>& map_bit_to_pathpoint, const BitVector& paths_to_keep, bool print_results = false);
	*/
};

#endif
