// Interfacing with the SMT solver

#ifndef _SMT_H
#define _SMT_H

#include <cvc4/expr/expr_manager.h>
#include <cvc4/smt/smt_engine.h>
#include <elm/util/BitVector.h>
#include "analysis.h"
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
	Option<SLList<Analysis::Path> > seekInfeasiblePaths(const SLList<Analysis::LabelledPredicate>& labelled_preds);
	bool checkPredSat(const SLList<Analysis::LabelledPredicate>& labelled_preds, bool print_result = false);
	
private:
	CVC4::ExprManager em;
	CVC4::SmtEngine smt;
	VariableStack variables;

	SLList<Analysis::Path> seekInfeasiblePaths_interm(const SLList<Analysis::LabelledPredicate>& labelled_preds, int index = 0);
	unsigned int genPathPointBitMaps(const SLList<Analysis::Path>& paths, AVLMap<const Edge*, unsigned int>& map_pathpoint_to_bit, const Edge* map_bit_to_pathpoint[]);
	Vector<BitVector> genBitCodeList(SLList<Analysis::Path> paths, const AVLMap<const Edge*, unsigned int>& map_pathpoint_to_bit, unsigned int used_bits);

	Option<Expr> getExpr(const Predicate& p);
	Option<Expr> getExpr(const Operand& o);
	
	       Kind_t getKind(condoperator_t opr);
	inline Kind_t getKind(const Predicate& p) { return getKind(p.opr()); }
	
	const CVC4::Type integer; // Z
};

#endif
