// Interfacing with the SMT solver

#ifndef _SMT_H
#define _SMT_H

#include <cvc4/expr/expr_manager.h>
#include <cvc4/smt/smt_engine.h>
#include <elm/util/BitVector.h>
#include "analysis.h"
#include "constant_variables.h"
#include "smt_operand_visitor.h"
#include "smt_variable_stack.h"

using namespace elm::genstruct;
using namespace CVC4::kind;
using elm::BitVector;
using CVC4::Expr;

class SMT
{
public:
	SMT();
	~SMT();
	Option<Analysis::Path> seekInfeasiblePaths(const Analysis::State& s);
	// Option<Analysis::Path> seekInfeasiblePaths(SLList<LabelledPredicate> labelled_preds, const ConstantVariables& constants);
	bool checkPredSat(const SLList<Option<Expr> >& exprs);
	bool checkPredSat(const SLList<LabelledPredicate>& labelled_preds);
	
private:
	CVC4::ExprManager em;
	CVC4::SmtEngine smt;
	VariableStack variables;
	SLList<Option<Expr> > exprs;
	/*
	SLList<Analysis::Path> getAllInfeasiblePaths(const SLList<LabelledPredicate>& labelled_preds, int index = 0);
	void removeIncompletePredicates(SLList<LabelledPredicate>& labelled_preds);
	void genPathPointBitMaps(const SLList<Analysis::Path>& paths, AVLMap<const Edge*, unsigned int>& map_pathpoint_to_bit, Vector<const Edge*>& map_bit_to_pathpoint);
	SLList<BitVector> genBitCodes(const SLList<Analysis::Path>& paths, const AVLMap<const Edge*, unsigned int>& map_pathpoint_to_bit, unsigned int used_bits);
	*/
	BitVector getListOfPathsToKeep(const SLList<BitVector>& bitcode_list);
	SLList<Analysis::Path> filterPaths(const SLList<BitVector>& bitcode_list, const Vector<const Edge*>& map_bit_to_pathpoint, const BitVector& paths_to_keep, bool print_results = false);

	Option<Expr> getExpr(const Predicate& p);
	Option<Expr> getExpr(const Operand& o);
	
	       Kind_t getKind(condoperator_t opr) const;
	inline Kind_t getKind(const Predicate& p) const { return getKind(p.opr()); }

	void printInfeasiblePath(const Analysis::Path& path) const;
	
	const CVC4::Type integer; // Z
};

#endif
