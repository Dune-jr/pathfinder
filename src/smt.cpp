// TODO bitvector

// Interfacing with the SMT solver

#include <elm/genstruct/SLList.h>
#include <elm/genstruct/Vector.h>
#include <elm/util/BitVector.h>
#include <otawa/cfg/Edge.h>
#include "smt.h"
#include "smt_variable_stack.h"
#include "analysis.h"
#include "debug.h"

// TODO: TEST Expr simplify(const Expr& e) 

using namespace CVC4::kind;
using elm::BitVector;
using otawa::Edge;
using CVC4::Expr;

SMT::SMT(): smt(&em), integer(em.integerType())
{
	smt.setLogic("QF_LIRA"); // Linear Incremental Arith Logic?
}

Option<SLList<Analysis::Path> > SMT::seekInfeasiblePaths(const SLList<Analysis::LabelledPredicate>& labelled_preds)
{
	const unsigned int MAX_BITVECTOR_SIZE = 64; // TODO: get rid of this
	if(checkPredSat(labelled_preds, true))
		return elm::none; // no inconsistency found
		
	// TODO optimization: remove incomplete predicates!

	SLList<Analysis::LabelledPredicate>::Iterator iter(labelled_preds);
	SLList<Analysis::Path> paths = seekInfeasiblePaths_interm(labelled_preds);
	
	// now some filtering work on this path list
	AVLMap<const Edge*, unsigned int> map_pathpoint_to_bit;
	const Edge* map_bit_to_pathpoint[MAX_BITVECTOR_SIZE] = {NULL};
	const unsigned int used_bits = genPathPointBitMaps(paths, map_pathpoint_to_bit, map_bit_to_pathpoint);
	assert(used_bits < MAX_BITVECTOR_SIZE);
	Vector<BitVector> bitcode_vector = genBitCodeList(paths, map_pathpoint_to_bit, used_bits);

	const int number_of_paths = bitcode_vector.length();
	BitVector paths_to_keep(number_of_paths, true); // a priori, keep all paths
	for(int i = 0; i < number_of_paths; i++)
	{
		if(!paths_to_keep[i])
			continue;
		for(int j = i+1; j < number_of_paths; j++)
		{
			if(!paths_to_keep[j])
				continue;
			const BitVector& opd1 = bitcode_vector[i];
			const BitVector& opd2 = bitcode_vector[j];
			// DBG("\t#" << i << "(" << opd1 << ") <= #" << j << "(" << opd2 << ") : " << DBG_TEST(opd1 <= opd2, false))
			if(opd1 <= opd2)
			{	// opd2 is superfluous
				paths_to_keep.set(j, false);
				continue;
			}
			// DBG("\t#" << i << "(" << opd1 << ") >= #" << j << "(" << opd2 << ") : " << DBG_TEST(opd1 >= opd2, false))
			if(opd1 >= opd2)
			{	// opd1 is superfluous
				paths_to_keep.set(i, false);
				break;
			}
		}
	}
	
	SLList<Analysis::Path> filtered_paths;
	int index = -1;
	for(Vector<BitVector>::Iterator iter(bitcode_vector); iter; iter++)
	{
		index++;
		if(!paths_to_keep[index])
			continue;
		Analysis::Path path; // this is a SLList
		elm::String str = "[";
		bool first = true;
		for(BitVector::OneIterator oneiter(bitcode_vector[index]); oneiter; oneiter++)
		{
			const Edge* e = map_bit_to_pathpoint[*oneiter];
			if(first)
				first = false;
			else
				str = str.concat(_ << ", ");
			str = str.concat(_ << e->source()->number() << "->" << e->target()->number());
			path += e;
		}
		str = str.concat(_ << "]");
		DBG("Inf. path identified: " << str << " (bitcode=" << bitcode_vector[index] << ")")
		filtered_paths += path;
	}
	return elm::some(filtered_paths);
}

// generate maps
unsigned int SMT::genPathPointBitMaps(const SLList<Analysis::Path>& paths, AVLMap<const Edge*, unsigned int>& map_pathpoint_to_bit, const Edge* map_bit_to_pathpoint[])
{
	unsigned int bit_count = 0;
	for(SLList<Analysis::Path>::Iterator iter(paths); iter; iter++)
	{
		SLList<const Edge*> p = *iter;
		for(SLList<const Edge*>::Iterator subiter(p); subiter; subiter++)
		{
			if(!map_pathpoint_to_bit.hasKey(*subiter))
			{
				// DBG(bit_count << " <=> " << *subiter)
				map_pathpoint_to_bit.put(*subiter, bit_count);
				map_bit_to_pathpoint[bit_count++] = *subiter;
			}
		}
	}
	return bit_count;
}

// map paths to bitcodes
Vector<BitVector> SMT::genBitCodeList(SLList<Analysis::Path> paths, const AVLMap<const Edge*, unsigned int>& map_pathpoint_to_bit, unsigned int used_bits)
{
	BitVector bitcode(used_bits);
	Vector<BitVector> bitcode_vector;
	int index = 0;
	for(SLList<Analysis::Path>::Iterator iter(paths); iter; iter++)
	{
		bitcode.clear();
		SLList<const Edge*> p = *iter;
		for(SLList<const Edge*>::Iterator subiter(p); subiter; subiter++)
			bitcode.set(*map_pathpoint_to_bit.get(*subiter));
			// bitcode |= 1u << *map_pathpoint_to_bit.get(*subiter);
		bitcode_vector.push(bitcode);
	}
	return bitcode_vector;
}

SLList<Analysis::Path> SMT::seekInfeasiblePaths_interm(const SLList<Analysis::LabelledPredicate>& labelled_preds, int index)
{	// Algorithm 3 (n~2^n)
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
	labelled_preds_truncated.remove(iter);
	// DBG(COLOR_Whi << "LPs: " << labelled_preds)
	// DBG(COLOR_Whi << "LPs_truncated: " << labelled_preds_truncated)

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
bool SMT::checkPredSat(const SLList<Analysis::LabelledPredicate>& labelled_preds, bool print_result)
{
	smt.push();
	for(SLList<Analysis::LabelledPredicate>::Iterator iter(labelled_preds); iter; iter++)
	{
		Predicate pred = (*iter).pred();
		if(Option<Expr> expr = getExpr(pred))
		{
			// DBG_STD(COLOR_Red << "Assumption: " << *expr)
			smt.assertFormula(*expr);
		}
	}
		
	CVC4::Result result = smt.checkSat(em.mkConst(true)); // check satisfability
	smt.pop();
	if(print_result)
		DBG(COLOR_BIWhi << "SMT call:" << (result.isSat() ? COLOR_BGre : COLOR_BIRed) << (result.isSat() ? " SAT" : " UNSAT"))
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
 