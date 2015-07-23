/*
 * Implementing z3 as the SMT solver
 */
#include <elm/genstruct/SLList.h>
// #include <elm/util/BitVector.h>
#include "z3_operand_visitor.h"
#include "../debug.h"
#include "z3_smt.h"

Z3SMT::Z3SMT(): s(c), p(c), sp(c.int_const("SP"))
{
	p.set("unsat_core", true);
	s.set(p);
}

// check predicates satisfiability
bool Z3SMT::checkPredSat(const SLList<LabelledPredicate>& labelled_preds)
{
	for(SLList<LabelledPredicate>::Iterator iter(labelled_preds); iter; iter++)
	{
		const Predicate& p = iter->pred();
		if(p.isComplete())
		{
			const z3::expr& e = getExpr(p);
			z3::expr label = c.bool_const(Analysis::pathToString(iter->labels()).chars());
			labels_hash.push(label.hash());
			s.add(e, label);
		}
	}
	return s.check();
}

// get unsat core and build a shortened path accordingly
// rtn false if failure, true otherwise
bool Z3SMT::retrieveUnsatCore(Analysis::Path& path, const SLList<LabelledPredicate>& labelled_preds, std::basic_string<char>& unsat_core_output)
{
	z3::expr_vector core = s.unsat_core();
	bool empty = true;
	unsat_core_output = "[";
	for(unsigned int i = 0; i < core.size(); i++)
	{
		if(!empty)
			unsat_core_output += ", ";
		// unsat_core_output += core[i];
		unsat_core_output += Z3_ast_to_string(c, core[i]);
		empty = false;

		Vector<unsigned int>::Iterator labels_hash_iter(labels_hash);
		for(SLList<LabelledPredicate>::Iterator lp_iter(labelled_preds); lp_iter; lp_iter++)
		{	
			if(!lp_iter->pred().isComplete())
				continue;
			assert(labels_hash_iter);
			if(*labels_hash_iter == core[i].hash())
				path += lp_iter->labels(); //DBG(color::IGre() << "yeeees (" << Analysis::pathToString(lp_iter->labels()) << ")")}
			labels_hash_iter++;
		}
	}
	unsat_core_output += "]";
	return !empty;
}

z3::expr Z3SMT::getExpr(const Predicate& p)
{
	switch(p.opr())
	{
		case CONDOPR_LT:
			return getExpr(p.leftOperand()) < getExpr(p.rightOperand());
		case CONDOPR_LE:
			return getExpr(p.leftOperand()) <= getExpr(p.rightOperand());
		case CONDOPR_EQ:
			return getExpr(p.leftOperand()) == getExpr(p.rightOperand());
		case CONDOPR_NE:
			return getExpr(p.leftOperand()) != getExpr(p.rightOperand());
		default:
			assert(false);
	}
}

z3::expr Z3SMT::getExpr(const Operand& o)
{
	assert(o.isComplete()); // should have been checked for earlier
	Z3OperandVisitor visitor(c, sp);
	assert(o.accept(visitor)); // invalid operands are not tolerated at this point
	return visitor.result();
}

// char ll[4];
// for(count = 0; count <= 256*256; count++)
// {
// 	intToCharCouple(count, ll);
// 	assert(count == charCoupleToInt(ll));
// }
/*
int Z3SMT::charCoupleToInt(const char* label) const
{
	return ((unsigned char)label[1]-1)*255 + ((unsigned char)label[2]-1);
}

void Z3SMT::intToCharCouple(int x, char label[4]) const
{
	assert(x < 255*255);
	label[0] = 'p'; // to prevent overlapping with variables such as SP, [..], t1, r4...
	label[1] = (x / 255)+1;
	label[2] = (x % 255)+1;
	label[3] = 0;
}*/

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