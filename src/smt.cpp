// Interfacing with the SMT solver

#include <elm/genstruct/SLList.h>
#include <otawa/cfg/Edge.h>
#include "smt.h"
#include "smt_variable_stack.h"
#include "analysis.h"
#include "debug.h"

// TODO: TEST Expr simplify(const Expr& e) 

using namespace CVC4::kind;
using CVC4::Expr;
using otawa::Edge;

SMT::SMT(): smt(&em), integer(em.integerType())
{
	smt.setLogic("QF_LIRA"); // Linear Incremental Arith Logic?
}

Option<SLList<Analysis::Path> > SMT::seekInfeasiblePaths(const SLList<Analysis::LabelledPredicate>& labelled_preds)
{
	if(checkPredSat(labelled_preds))
		return elm::none; // no inconsistency found
		
	// TODO optimization: remove incomplete predicates!

	SLList<Analysis::LabelledPredicate>::Iterator iter(labelled_preds);
	SLList<Analysis::Path> paths = seekInfeasiblePaths_interm(labelled_preds);
	
	// TODO: some filtering work on this path list
	// use an AVLmap to store the id <-> path matches
	// example 0x4000 is 0, 0x4004 is 1, 0x4008 is 2
	// then we have 0b101 <-> {0x4000, 0x4008}
	// 				0b001 <-> {0x4000}
	// 		do OR!
	// 				0b101
	// this is equal to the first operand, so 0b101 CONTAINS 0b001
	// therefore we can remove 0b101
	unsigned int count = 0;
	AVLMap<const Edge*, unsigned int> map_pathpoint_to_bit;
	for(SLList<Analysis::Path>::Iterator iter(paths); iter; iter++)
	{
		DBG(COLOR_BIRed << "sllist '*iter', (size=" << (*iter).count() << "): " << *iter)
		for(SLList<const Edge*>::Iterator subiter(*iter); subiter; subiter++)
		{
			assert(subiter);
			subiter.item();
			//((SLList<const Edge*>::Iterator)subiter)->item();
			*subiter; // segfault
			//DBG("\tsubiter = " << subiter.item())
			
			if(!map_pathpoint_to_bit.hasKey(subiter.item()))
			{
				DBG(COLOR_BIGre << "YES!")
				map_pathpoint_to_bit.put(subiter.item(), count++);
			}
			else
			{
				DBG(COLOR_BIRed << "NO!")
			}
	}
		
	DBG(COLOR_BIRed << "AVLMAP now looks like: " << map_pathpoint_to_bit)
		
	
	DBG(COLOR_IGre << "Result: " << paths)
	return elm::some(paths);
}

SLList<Analysis::Path> SMT::seekInfeasiblePaths_interm(const SLList<Analysis::LabelledPredicate>& labelled_preds, int index)
{	// Algorithm 3 (n~2^n)
	DBG(COLOR_IYel << "Call : index = " << index)
	bool index_in_range = false;
	SLList<Analysis::LabelledPredicate> labelled_preds_truncated = labelled_preds;
	SLList<Analysis::LabelledPredicate>::Iterator iter(labelled_preds_truncated);
	for(int i = 0; iter; iter++)
	{
		if(i++ == index)
		{
			index_in_range = true;
			break;
		}
	}
	if(!index_in_range) // we're done parsing predicates
	{
		// typedef SLList<const Edge*> Path;
		SLList<const Edge*> path;
		SLList<Analysis::Path> path_list;
		for(SLList<Analysis::LabelledPredicate>::Iterator parse_iter(labelled_preds); parse_iter; parse_iter++)
			path += (*parse_iter).label();
		path_list += path;
		return path_list;
	}

	Analysis::LabelledPredicate lp = *iter;
	labelled_preds_truncated.remove(iter);
	DBG(COLOR_Whi << "LPs: " << labelled_preds)
	DBG(COLOR_Whi << "LPs_truncated: " << labelled_preds_truncated)

	if(checkPredSat(labelled_preds_truncated))
	{	// SAT: keep predicate in list
		return seekInfeasiblePaths_interm(labelled_preds, index+1);
	}
	else
	{	// UNSAT: fork
		SLList<Analysis::Path> paths = seekInfeasiblePaths_interm(labelled_preds_truncated, index); // without the unnecessary predicate
							   paths+= seekInfeasiblePaths_interm(labelled_preds,         index+1); // with the unnecessary predicate
		return paths;
	}
}

// check predicates satisfiability
bool SMT::checkPredSat(const SLList<Analysis::LabelledPredicate>& labelled_preds)
{
	smt.push();
	for(SLList<Analysis::LabelledPredicate>::Iterator iter(labelled_preds); iter; iter++)
	{
		Predicate pred = (*iter).pred();
		if(Option<Expr> expr = getExpr(pred))
		{
			DBG_STD(COLOR_IRed << "Assumption: " << *expr)
			smt.assertFormula(*expr);
		}
	}
		
	CVC4::Result result = smt.checkSat(em.mkConst(true)); // check satisfability
	DBG_STD(COLOR_IRed << "CheckSat: " << result)
	smt.pop();
	return result.isSat();
}

Option<Expr> SMT::getExpr(const Predicate& p)
{
	const Operand &opd1 = p.leftOperand(), &opd2 = p.rightOperand();
	if(!(opd1.isComplete() && opd2.isComplete()))
		return elm::none;

	return em.mkExpr(getKind(p), getExpr(opd1), getExpr(opd2));
}

Option<Expr> SMT::getExpr(const Operand& o)
{
	if(!o.isComplete())
		return elm::none;
		
	SMTOperandVisitor visitor(em, variables);
	o.accept(visitor);
	return elm::some(visitor.result());
}

Kind_t SMT::getKind(condoperator_t opr)
{
	switch(opr)
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
			assert(false);
	}
}
