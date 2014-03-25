/*
 * Skeleton of the analysis algorithm, defines the way we parse the CFG 
 */

#include <elm/io.h>
#include <elm/io/Output.h>
#include <elm/imm/list.h>
#include <otawa/cfg/CFG.h>
#include <otawa/cfg/Edge.h>
#include <otawa/cfg/BasicBlock.h>

#include "analysis.h"
#include "debug.h"

using namespace elm::imm;
using namespace elm::io;

void Analysis::initializeAnalysis()
{
	// labeled_preds := [[]]
	labeled_preds += list<LabelledPredicate>::null; // Add an empty list as first element
}

// WARNING: atm, this function assumes we have NO LOOPS!
void Analysis::processCFG(CFG *cfg)
{
	DBG("Processing CFG " << cfg)
	processBB(cfg->firstBB());
	DBG("\e[4mResult of the analysis\e[0m: " << labeled_preds)
}

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
	list<Predicate> pred_list = analyzeBB(bb);
	
	DBG(COLOR_Whi << "State of the analysis: " << labeled_preds << COLOR_RCol)
	
	int edgeId = 0;
	for(BasicBlock::OutIterator outs(bb); outs; outs++)
	{		
		if(outs->kind() == Edge::TAKEN
		|| outs->kind() == Edge::NOT_TAKEN
		|| outs->kind() == Edge::VIRTUAL
		|| outs->kind() == Edge::VIRTUAL_RETURN) // Filter out irrelevant edges (calls...)
		{			
			if(edgeId++) // If this is not the first valid edge
				labeled_preds += labeled_preds.hd(); // then duplicate the list we are working on
			// labeled_preds.hd() += labelPredicateList(pred_list, *outs); // Label our list of predicates with the edge and append it
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
	
	DBG(COLOR_Whi << "State of the analysis: " << labeled_preds << COLOR_RCol)
	
	processBB(target);
}

list<Analysis::LabelledPredicate> Analysis::labelPredicateList(list<Predicate> pred_list, Edge* label)
{
	list<LabelledPredicate> LP_list;
	while(!pred_list.isEmpty())
	{
		LabelledPredicate lp(pred_list.hd(), label);
		LP_list = cons(lp, LP_list);
		pred_list = pred_list.tl(); // This warrants that the loop will end
	}
	// TODO!
}
