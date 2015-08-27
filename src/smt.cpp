/*
 * Interfacing with the SMT solver
 */
// #include <iostream>
#include <elm/genstruct/SLList.h>
#include "smt.h"
#include "debug.h"

using namespace elm::genstruct;

SMT::SMT() { }

Option<Analysis::Path> SMT::seekInfeasiblePaths(const Analysis::State& s)
{
	// add the constant info to the the list of predicates
	SLList<LabelledPredicate> labelled_preds = s.getLabelledPreds(); // implicit copy
	labelled_preds += s.getConstants().toPredicates();
	
	if(checkPredSat(labelled_preds))
	{
		DBG("Checking path " << s.getPathString() << ": " << color::BGre() << "SAT")
		return elm::none;
	}
	DBG("Checking path " << s.getPathString() << ": " << color::BIRed() << "UNSAT")

	Analysis::Path path;
	std::basic_string<char> unsat_core_output;
	bool unsat_core_success = retrieveUnsatCore(path, labelled_preds, unsat_core_output);

	if(!unsat_core_success)
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
		DBG("   UNSAT core: " << unsat_core_output.c_str())
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

const elm::String SMT::printChosenSolverInfo()
{
#ifdef SMT_SOLVER_Z3
	return "z3";
#endif
#ifdef SMT_SOLVER_CVC4
	return "cvc4";
#endif
}

/*
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
