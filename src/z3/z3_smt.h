/*
 * Implementing z3 as the SMT solver
 */
#ifndef _Z3_Z3_SMT_H
#define _Z3_Z3_SMT_H

#include <z3++.h>
#include "../analysis.h" // Analyis::Path
#include "../smt.h" // abstract SMT class inheritance

class Z3SMT : public SMT
{
public:
    Z3SMT(int flags);
    static inline elm::String name() { return "z3"; }
    
private:
    z3::context c;
    z3::solver s;
    z3::params p;
    z3::expr sp;
    // Z3VariableStack variables; // no need of this with z3
    Vector<unsigned int> labels_hash;

    // SMT virtual pure methods
    void initialize(const SLList<LabelledPredicate>& labelled_preds);
    void initialize(const LocalVariables& lv, const HashTable<Constant, const Operand*, ConstantHash>& mem, DAG& dag) { }
    bool checkPredSat();
    bool retrieveUnsatCore(Analysis::Path& path, const SLList<LabelledPredicate>& labelled_preds, std::basic_string<char>& unsat_core_output);
    z3::expr getExpr(const Predicate& p);
    z3::expr getExpr(const Operand& o);
};

#endif