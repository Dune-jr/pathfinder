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
	labelled_preds += null<LabelledPredicate>(); // Adds an empty list as first element
}

// WARNING: atm, this function assumes we have NO LOOPS!
void Analysis::processCFG(CFG* cfg)
{
	DBG(COLOR_Whi << "Processing CFG " << cfg)
	processBB(cfg->firstBB());
	DBG("\e[4mResult of the analysis: " << COLOR_RCol << labelled_preds)
}

void Analysis::processBB(BasicBlock* bb)
{
	if(bb->isExit())
	{
		DBG(COLOR_BIBlu << "EXIT block reached" << COLOR_RCol)
		return;
	}
		
	DBG(COLOR_Whi << "Processing " << bb)

	// may remove some of the labelled predicates
	// generates a list of predicates in generated_preds
	analyzeBB(bb);
	
	assert(labelled_preds); // Assert that the list of lists we are modifying is not empty
	int edgeId = 0;
	for(BasicBlock::OutIterator outs(bb); outs; outs++)
	{
		if(outs->kind() == Edge::TAKEN
		|| outs->kind() == Edge::NOT_TAKEN
		|| outs->kind() == Edge::VIRTUAL
		|| outs->kind() == Edge::VIRTUAL_RETURN) // Filter out irrelevant edges (calls...)
		{	
			if(edgeId++) // If this is not the first valid edge
				// labelled_preds += labelled_preds.first(); // then duplicate the list we are working on
				labelled_preds += null<LabelledPredicate>(); // TODO: is this the right move?
			
			// Label our list of predicates with the current edge then append it
			SLList<LabelledPredicate> labelled_analysis_result = labelPredicateList(generated_preds, *outs);
			SLList<LabelledPredicate> topList = getTopList();
			topList.addAll(labelled_analysis_result); // append to topList
			setTopList(topList); // set topList as new first item of attribute labelled_preds
			
			processEdge(*outs);
		}
	}
}

void Analysis::processEdge(Edge* edge)
{
	BasicBlock* target = edge->target();
	DBG(COLOR_Whi << "Processing Edge: " << edge)
	
	// TODO: we very well may have something to process here,
	//       in case it's a conditional branch (c or neg(c))
	
	DBG("State of the analysis: " << labelled_preds << COLOR_RCol)
	
	processBB(target);
}

SLList<Analysis::LabelledPredicate> Analysis::labelPredicateList(const SLList<Predicate>& pred_list, Edge* label)
{
	SLList<LabelledPredicate> LP_list;
	for(SLList<Predicate>::Iterator preds(pred_list); preds; preds++)
	{
		LabelledPredicate lp(*preds, label);
		LP_list.addFirst(lp);
	}
	return LP_list;
}
