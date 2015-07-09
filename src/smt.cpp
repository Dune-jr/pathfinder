/*
 * Interfacing with the SMT solver
 */
#include <cvc4/expr/command.h> // getUnsatCoreCommand
#include <cvc4/util/unsat_core.h>
#include <elm/genstruct/SLList.h>
#include <elm/genstruct/Vector.h>
#include <elm/util/BitVector.h>
#include <otawa/cfg/Edge.h>
#include "smt.h"
#include "smt_variable_stack.h"
#include "operand.h"
#include "debug.h"

using namespace CVC4::kind;
using elm::BitVector;
using otawa::Edge;
using CVC4::Expr;

SMT::SMT(): smt(&em), variables(em), integer(em.integerType())
{
	smt.setLogic("QF_LIA"); // Quantifier-Free (no forall, exists...) Linear Integer Arithmetic
	smt.setOption("incremental", CVC4::SExpr("false")); // non incremental
	smt.setOption("produce-unsat-cores", CVC4::SExpr("true"));
	smt.setOption("rewrite-divk", CVC4::SExpr("true"));
	// smt.setOption("dump-unsat-cores", CVC4::SExpr("true"));
	// smt.setOption("produce-proofs", CVC4::SExpr("true"));
	// smt.setOption("dump", "assertions:pre-everything");
	// smt.setOption("dump-to", "dump.log"); // this is actually global to CVC4... meaning setting it once per pathfinder execution is enough
}

// gets a copy of the labelled_preds SLList
// actually always returns 1 infeasible path, but that may be changed in the future
// Option<Analysis::Path> SMT::seekInfeasiblePaths(SLList<LabelledPredicate> labelled_preds, const ConstantVariables& constants)
Option<Analysis::Path> SMT::seekInfeasiblePaths(const Analysis::State& s)
{
	// add the constant info to the the list of predicates
	SLList<LabelledPredicate> labelled_preds = s.getLabelledPreds();
	labelled_preds += s.getConstants().toPredicates();
	// get a SLList<Option<Expr> > out of a SLList<LabelledPredicate> in order to know which LP matches which expr
	for(SLList<LabelledPredicate>::Iterator iter(labelled_preds); iter; iter++)
		exprs.addLast(getExpr(iter->pred()));
	
	if(checkPredSat(exprs))
	{
		DBG("Checking path " << s.getPathString() << ": " << color::BGre() << "SAT")
		return elm::none;
	}
	DBG("Checking path " << s.getPathString() << ": " << color::BIRed() << "UNSAT")

	CVC4::UnsatCore unsat_core = smt.getUnsatCore(); // get an unsat subset of our assumptions
	Analysis::Path path; // build our path
	bool empty = true;
	std::basic_string<char> unsat_core_output = "[";
	for(CVC4::UnsatCore::const_iterator unsat_core_iter = unsat_core.begin(); unsat_core_iter != unsat_core.end(); unsat_core_iter++)
	{
		if(!empty)
			unsat_core_output += ", ";	
		unsat_core_output += (*unsat_core_iter).toString();
		empty = false;
		SLList<LabelledPredicate>::Iterator lp_iter(labelled_preds);
		SLList<Option<Expr> >::Iterator expr_iter(exprs);

		for(; lp_iter; lp_iter++, expr_iter++)
		{
			if(*expr_iter && **expr_iter == *unsat_core_iter)
			{
				path += (*lp_iter).labels();
				// Set<const Edge*>::Iterator label_iter((*lp_iter).labels());
				// DBG(" => fst label: " << (*label_iter)->source()->number() << "->" << (*label_iter)->target()->number())
			}
		}
	}
	if(empty)
	{
		DBG(color::BIWhi() << "A)" << color::RCol() << " Extracting UNSAT core... " << color::IRed() << "FAILED!");
		DBG("   Using original predicates:")
		for(SLList<LabelledPredicate>::Iterator parse_iter(labelled_preds); parse_iter; parse_iter++)
		{
			if((*parse_iter).pred().isComplete())
			{
				DBG("   * " << (*parse_iter))
				path.addAll((*parse_iter).labels());
			}
		}
	}
	else
	{
		DBG(color::BIWhi() << "A)" << color::RCol() << " Extracting UNSAT core... " << color::IGre() << "SUCCESS!");
		DBG_STD("   UNSAT core: " << unsat_core_output << "]")
#ifdef DBGG
		cout << "   Original predicates:" << endl;
		for(SLList<LabelledPredicate>::Iterator parse_iter(labelled_preds); parse_iter; parse_iter++)
		{
			if((*parse_iter).pred().isComplete())
				cout << "   * " << (*parse_iter) << endl;
		}
		std::cout << "   UNSAT core: " << unsat_core_output << "]" << endl;
#endif
	}

	// printInfeasiblePath(path);
	if(path == Analysis::Path::null)
		return elm::none;
	return elm::some(path);

/*
	removeIncompletePredicates(labelled_preds); // optimization: incomplete predicates are not sent to the SMT therefore they cannot play a role in the UNSAT
	SLList<Analysis::Path> paths = getAllInfeasiblePaths(labelled_preds); // exhaustive list of paths
		cout << labelled_preds.count() << " " << paths.count() << endl;
	AVLMap<const Edge*, unsigned int> map_pathpoint_to_bit;
	Vector<const Edge*> map_bit_to_pathpoint;
	genPathPointBitMaps(paths, map_pathpoint_to_bit, map_bit_to_pathpoint); // generate maps to convert Path <-> BitVector
	// const Vector<BitVector>& bitcode_vector = genBitCodes(paths, map_pathpoint_to_bit, map_bit_to_pathpoint.length()); // generate the list of BitVectors corresponding to the list of paths
	const SLList<BitVector>& bitcode_list = genBitCodes(paths, map_pathpoint_to_bit, map_bit_to_pathpoint.length()); // generate the list of BitVectors corresponding to the list of paths
	const BitVector& paths_to_keep = getListOfPathsToKeep(bitcode_list); // get the list of relevant paths (removes redundant bitcode such as 101 in {101, 100})
	
	return elm::some(filterPaths(bitcode_list, map_bit_to_pathpoint, paths_to_keep, true)); // generate the list of non-redundant paths and return it
//*/
}

// check predicates satisfiability
bool SMT::checkPredSat(const SLList<Option<Expr> >& exprs)
{
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
	/*catch(...)
	{
		cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" << endl;
		return true;	
	}*/
}

// check predicates satisfiability
bool SMT::checkPredSat(const SLList<LabelledPredicate>& labelled_preds)
{	
	try
	{
		for(SLList<LabelledPredicate>::Iterator iter(labelled_preds); iter; iter++)
			if(Option<Expr> expr = getExpr(iter->pred()))
				smt.assertFormula(*expr); // second parameter to true for unsat cores
	
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
}

Option<Expr> SMT::getExpr(const Predicate& p)
{
	if(!p.isComplete())
		return elm::none;
	return em.mkExpr(getKind(p), getExpr(p.leftOperand()), getExpr(p.rightOperand()));
}

Option<Expr> SMT::getExpr(const Operand& o)
{
	if(!o.isComplete())
		return elm::none;
		
	SMTOperandVisitor visitor(em, variables);
	if(!o.accept(visitor))
		return elm::none;
	return elm::some(visitor.result());
}

Kind_t SMT::getKind(condoperator_t opr) const
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

void SMT::printInfeasiblePath(const Analysis::Path& path) const
{
	elm::String str = "[";
	bool first = true;
	for(Analysis::Path::Iterator iter(path); iter; iter++)
	{
		if(first)
			first = false;
		else
			str = str.concat(_ << ", ");
		str = str.concat(_ << (*iter)->source()->number() << "->" << (*iter)->target()->number());
	}
	str = str.concat(_ << "]");
	DBG(color::On_IRed() << "Inf. path found: " << str)
}

/*
void SMT::removeIncompletePredicates(SLList<LabelledPredicate>& labelled_preds)
{
	SLList<LabelledPredicate>::Iterator iter(labelled_preds);
	while(iter)
	{
		if(!iter->pred().isComplete())
		{
			SLList<LabelledPredicate>::Iterator prev_iter(iter);
			iter++;
			labelled_preds.remove(prev_iter);
		}
		else
			iter++;
	}
}

SLList<Analysis::Path> SMT::getAllInfeasiblePaths(const SLList<LabelledPredicate>& labelled_preds, int index)
{	// Algorithm 3 (n~2^n)
	bool index_in_range = false;
	SLList<LabelledPredicate> labelled_preds_truncated = labelled_preds;
	SLList<LabelledPredicate>::Iterator iter(labelled_preds_truncated);
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
		Analysis::Path path; // typedef SLList<const Edge*> Path;
		SLList<Analysis::Path> path_list;
		for(SLList<LabelledPredicate>::Iterator parse_iter(labelled_preds); parse_iter; parse_iter++)
			path += (*parse_iter).labels();
		path_list += path;
		return path_list;
	}
	labelled_preds_truncated.remove(iter);
	// DBG(color::Whi() << "LPs: " << labelled_preds)
	// DBG(color::Whi() << "LPs_truncated: " << labelled_preds_truncated)

	if(checkPredSat(labelled_preds_truncated))
	{	// SAT: keep predicate in list
		return getAllInfeasiblePaths(labelled_preds, index+1);
	}
	else
	{	// UNSAT: fork
		SLList<Analysis::Path> paths = getAllInfeasiblePaths(labelled_preds_truncated, index); // without the unnecessary predicate
							   paths+= getAllInfeasiblePaths(labelled_preds,         index+1); // with the unnecessary predicate
		return paths;
	}
}

// generate maps
void SMT::genPathPointBitMaps(const SLList<Analysis::Path>& paths, AVLMap<const Edge*, unsigned int>& map_pathpoint_to_bit, Vector<const Edge*>& map_bit_to_pathpoint)
{
	unsigned int bit_count = 0;
	for(SLList<Analysis::Path>::Iterator iter(paths); iter; iter++)
	{
		Analysis::Path p = *iter;
		for(Analysis::Path::Iterator subiter(p); subiter; subiter++)
		{
			if(!map_pathpoint_to_bit.hasKey(*subiter))
			{
				// DBG(bit_count << " <=> " << *subiter)
				map_pathpoint_to_bit.put(*subiter, bit_count++);
				map_bit_to_pathpoint.push(*subiter);
			}
		}
	}
}

// map paths to bitcodes
SLList<BitVector> SMT::genBitCodes(const SLList<Analysis::Path>& paths, const AVLMap<const Edge*, unsigned int>& map_pathpoint_to_bit, unsigned int used_bits)
{
	BitVector bitcode(used_bits);
	SLList<BitVector> bitcode_list;
	for(SLList<Analysis::Path>::Iterator iter(paths); iter; iter++)
	{
		bitcode.clear();
		Analysis::Path p = *iter;
		for(Analysis::Path::Iterator subiter(p); subiter; subiter++)
			bitcode.set(*map_pathpoint_to_bit.get(*subiter));
			// bitcode |= 1u << *map_pathpoint_to_bit.get(*subiter);
		bitcode_list += bitcode;
	}
	return bitcode_list;
}

BitVector SMT::getListOfPathsToKeep(const SLList<BitVector>& bitcode_list)
{
	const int number_of_paths = bitcode_list.count();
	BitVector paths_to_keep(number_of_paths, true); // a priori, keep all paths
	int i = 0;
	for(SLList<BitVector>::Iterator iter(bitcode_list); iter; iter++, i++)
	{
		if(!paths_to_keep[i])
			continue;

		SLList<BitVector>::Iterator subiter(iter);
		subiter++;
		for(int j = i+1; subiter; subiter++, j++)
		{
			if(!paths_to_keep[j])
				continue;
			const BitVector &opd1 = *iter, &opd2 = *subiter;
			if(opd1 <= opd2)
			{	// opd2 is superfluous
				paths_to_keep.set(j, false);
				continue;
			}
			if(opd1 >= opd2)
			{	// opd1 is superfluous
				paths_to_keep.set(i, false);
				break;
			}
		}
	}
	return paths_to_keep;
}

SLList<Analysis::Path> SMT::filterPaths(const SLList<BitVector>& bitcode_list, const Vector<const Edge*>& map_bit_to_pathpoint, const BitVector& paths_to_keep, bool print_results)
{
	SLList<Analysis::Path> filtered_paths;
	int index = 0;
	for(SLList<BitVector>::Iterator iter(bitcode_list); iter; iter++, index++)
	{
		if(!paths_to_keep[index])
			continue;
		Analysis::Path path; // this is a SLList
		elm::String str = "[";
		bool first = true;
		for(BitVector::OneIterator oneiter(*iter); oneiter; oneiter++)
		{
			const Edge* e = map_bit_to_pathpoint[*oneiter];
			if(first)
				first = false;
			else
				str = str.concat(_ << ", ");
			str = str.concat(_ << e->source()->number() << "->" << e->target()->number());
			// path.addLast(e); // preserving order for aesthetic purposes
			path += e;
		}
		str = str.concat(_ << "]");
		if(print_results)
			DBG(color::On_IRed() << "Inf. path found: " << str << " (bitcode=" << *iter << ")")
		filtered_paths += path;
	}
	return filtered_paths;
}
*/
