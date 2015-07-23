/*
 * Implementing z3 as the SMT solver
 */
#ifndef _Z3_Z3_SMT_H
#define _Z3_Z3_SMT_H

#include <z3++.h> // TODO! do not include everything
#include "../analysis.h" // Analyis::Path
#include "../smt.h" // abstract SMT class inheritance
// #include "z3_operand_visitor.h"
// #include "z3_variable_stack.h"

using namespace elm::genstruct;

class Z3SMT : public SMT
{
public:
    Z3SMT();
    
private:
    z3::context c;
    z3::solver s;
    z3::params p;
    z3::expr sp;
    // Z3VariableStack variables; // no need of this with z3!
    Vector<unsigned int> labels_hash;

    // SMT virtual pure methods
    bool checkPredSat(const SLList<LabelledPredicate>& labelled_preds);
    bool retrieveUnsatCore(Analysis::Path& path, const SLList<LabelledPredicate>& labelled_preds, std::basic_string<char>& unsat_core_output);
    z3::expr getExpr(const Predicate& p);
    z3::expr getExpr(const Operand& o);
    // int charCoupleToInt(const char* label) const;
    // void intToCharCouple(int x, char label[4]) const;
};

#endif