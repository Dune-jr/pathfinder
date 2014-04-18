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
{	// labelled_preds := [[]]
	labelled_preds += null<LabelledPredicate>(); // add an empty list as first element
}

// WARNING: atm, this function assumes we have NO LOOPS!
void Analysis::processCFG(CFG* cfg)
{
	DBG(COLOR_Whi << "Processing CFG " << cfg)
	processBB(cfg->firstBB());
	// DBG("\e[4mResult of the analysis: " << COLOR_RCol << labelled_preds)
}

void Analysis::processBB(BasicBlock* bb)
{
	if(bb->isExit())
	{
		DBG(COLOR_BIBlu << "EXIT block reached" << COLOR_RCol)
		return;
	}
		
	DBG(COLOR_Whi << "Processing " << bb)
	analyzeBB(bb); // generates lists of predicates in generated_preds and generated_preds_taken	
	assert(labelled_preds); // Assert that the list of lists we are modifying is not empty
	
	// these will be overwritten by further analysis, so back them up
	SLList<Predicate> generated_preds_backup	   = generated_preds;
	SLList<Predicate> generated_preds_taken_backup = generated_preds_taken;
	SLList<LabelledPredicate> top_list_backup	   = getTopList();
	
	int edgeId = 0;
	for(BasicBlock::OutIterator outs(bb); outs; outs++)
	{
		if(outs->kind() == Edge::TAKEN
		|| outs->kind() == Edge::NOT_TAKEN
		|| outs->kind() == Edge::VIRTUAL
		|| outs->kind() == Edge::VIRTUAL_RETURN) // Filter out irrelevant edges (calls...)
		{	
			// TODO Hugues: is there a way without using the ternary operator?
			SLList<Predicate>& relevant_preds = (outs->kind() == Edge::TAKEN) ?
				generated_preds_taken_backup :
				generated_preds_backup;
			
			// label our list of predicates with the current edge then append it
			SLList<LabelledPredicate> labelled_analysis_result = labelPredicateList(relevant_preds, *outs);
			
			if(edgeId++) // if this is not the first valid edge
				labelled_preds += top_list_backup; // copy the predicates we have generated until this node into a new list
			
			addElemToTopList(labelled_analysis_result);
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
