/*
 * Skeleton of the analysis algorithm, defines the way we parse the CFG 
 */

#include <elm/io.h>
#include <elm/io/Output.h>
#include <elm/genstruct/SLList.h>
#include <otawa/cfg/CFG.h>
#include <otawa/cfg/Edge.h>
#include <otawa/cfg/BasicBlock.h>

#include "analysis.h"
#include "debug.h"

using namespace elm::genstruct;
using namespace elm::io;

void Analysis::initializeAnalysis()
{
	// labelled_preds := [[]]
	labelled_preds += null<LabelledPredicate>(); // Add an empty list as first element
}

// WARNING: atm, this function assumes we have NO LOOPS!
void Analysis::processCFG(CFG *cfg)
{
	DBG("Processing CFG " << cfg)
	processBB(cfg->firstBB());
	DBG("\e[4mResult of the analysis\e[0m: " << labelled_preds)
}

template <class T, class E> template <template<class _> class C> inline void SLList<T, E>::addAll(const C<T> &items);
void Analysis::processBB(BasicBlock *bb)
{
	if(bb->isExit())
	{
		DBG(COLOR_ICya << "EXIT block reached" << COLOR_RCol)
		return;
	}
		
	DBG("Processing " << bb)

	// may remove some of the labeled predicates
	// returns a list of generated predicates
	SLList<Predicate> analysis_result = analyzeBB(bb);
	
	DBG(COLOR_Whi << "State of the analysis: " << labelled_preds << COLOR_RCol)
	
	assert(labelled_preds);
	int edgeId = 0;
	for(BasicBlock::OutIterator outs(bb); outs; outs++)
	{		
		if(outs->kind() == Edge::TAKEN
		|| outs->kind() == Edge::NOT_TAKEN
		|| outs->kind() == Edge::VIRTUAL
		|| outs->kind() == Edge::VIRTUAL_RETURN) // Filter out irrelevant edges (calls...)
		{	
			if(edgeId++) // If this is not the first valid edge
			{
				labelled_preds += labelled_preds.first(); // then duplicate the list we are working on
			}
			// Label our list of predicates with the current edge then append it
			SLList<LabelledPredicate> labelled_analysis_result = labelPredicateList(analysis_result, *outs);
			SLList<LabelledPredicate> oldlist = labelled_preds.first();
			
			
			
			//template <class T, class E> template <template<class _> class C>
				//inline void SLList<T, E>::addAll(const C<T> &items);
			// oldlist.addAll<SLList<LabelledPredicate> >(labelled_analysis_result); // TODO!!!
			
			//for(SLList<LabelledPredicate>::Iterator iter(labelled_analysis_result); iter; iter++)
			//	oldlist.add(iter);
			
			assert(analysis_result);
			Predicate p = analysis_result.first();
			DBG("p.opd1 = (" << p.opd1 << ")")
			DBG("*(p.opd1) = " << *(p.opd1))
			DBG("analysis_result.first() = " << analysis_result.first());
			DBG(COLOR_IRed << "labelled_analysis_result = " << labelled_analysis_result << ", oldlist = " << oldlist << COLOR_RCol);
			
			
			
			processEdge(*outs);
		}
	}
}

void Analysis::processEdge(Edge *edge)
{
	BasicBlock* target = edge->target();
	DBG("Processing Edge: " << edge)
	
	// TODO: we very well may have something to process here,
	//       in case it's a conditional branch (c or neg(c))
	
	DBG(COLOR_Whi << "State of the analysis: " << labelled_preds << COLOR_RCol)
	
	processBB(target);
}

SLList<Analysis::LabelledPredicate> Analysis::labelPredicateList(SLList<Predicate> pred_list, Edge* label)
{
	SLList<LabelledPredicate> LP_list;
	while(pred_list)
	{
		LabelledPredicate lp(pred_list.first(), label);
		LP_list.addFirst(lp);
		pred_list.removeFirst(); // This warrants that the loop will end
	}
	return LP_list;
	// TODO!
}
