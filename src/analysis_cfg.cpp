/*
 * Skeleton of the analysis algorithm, defines the way we parse the CFG 
 */

#include <elm/io/Output.h>
#include <elm/genstruct/SLList.h>
#include <otawa/cfg/Edge.h>
#include <otawa/cfg/BasicBlock.h>
#include <otawa/cfg/features.h>

#include "analysis.h"
#include "smt.h"
#include "debug.h"

using namespace elm::genstruct;
using namespace elm::io;

void Analysis::initializeAnalysis()
{	// labelled_preds := [[]]
	generated_preds.clear();
	labelled_preds.clear();
	labelled_preds += SLList<LabelledPredicate>::null; // add an empty list as first element
}

// WARNING: atm, this function assumes we have NO LOOPS!
void Analysis::processCFG(CFG* cfg)
{
	DBG(COLOR_Whi << "Processing CFG " << cfg)
	total_paths = 0;
	placeboProcessBB(cfg->firstBB());
#	ifndef DBG_NO_DEBUG
		DBG(COLOR_Whi << "Running pre-analysis... ")
		DBG(COLOR_Whi << total_paths << " paths found.")
#	else
		cout << "Running pre-analysis... ";
		cout << total_paths << " paths found." << endl;
#	endif
	processBB(cfg->firstBB());
	// DBG("\e[4mResult of the analysis: " << COLOR_RCol << labelled_preds)
	int infeasible_paths_count = infeasible_paths.count();
	DBG(COLOR_BIGre << infeasible_paths_count << " infeasible path" << (infeasible_paths_count == 1 ? "" : "s") << " found:")
	for(SLList<Path>::Iterator iter(infeasible_paths); iter; iter++)
	{
		SLList<const Edge*> l = *iter; // Path is SLList<const Edge*>
		bool first = true;
		elm::String str = "    - [";
		for(SLList<const Edge*>::Iterator subiter(l); subiter; subiter++)
		{
			if(first)
				first = false;
			else
				str = _ << str << ", ";
			str = str.concat(_ << (*subiter)->source()->number() << "->" << (*subiter)->target()->number());
		}
		str = _ << str << "]";
		DBG(COLOR_IGre << str)
	}
	cout << infeasible_paths_count << " infeasible path(s) found.\r\n";
}

void Analysis::processBB(BasicBlock* bb)
{
	static int paths_count = 0;
	if(bb->isExit())
	{
		DBG(COLOR_BIYel << "EXIT block reached")
		if((++paths_count % 100) == 0 || total_paths <= 1000)
			cout << "(" << paths_count << "/" << total_paths << ")\r\n";
		return;
	}
		
	// SMT call
	// TODO: this should be put at a smarter place
	SMT smt;
	if(Option<SLList<Analysis::Path> > maybe_infeasible_paths = smt.seekInfeasiblePaths(labelled_preds.first(), constants))
	{
		infeasible_paths += *maybe_infeasible_paths;
		DBG(COLOR_BIYel "Current path identified as infeasible, stopping analysis")
		cout << "(" << ++paths_count << "/" << total_paths << ") !\r\n";
		return; // No point to continue an infeasible path
	}
	// else DBG(COLOR_Cya "labelled_preds= " << labelled_preds) // TODO REMOVE
	
	DBG(COLOR_Whi << "Processing " << bb)
	analyzeBB(bb); // generates lists of predicates in generated_preds and generated_preds_taken	
	assert(labelled_preds);
	
	// these will be overwritten by further analysis, so back them up
	SLList<Predicate> generated_preds_backup	   = generated_preds;
	SLList<Predicate> generated_preds_taken_backup = generated_preds_taken;
	SLList<LabelledPredicate> top_list_backup	   = labelled_preds.first();
	ConstantVariables constants_backup(constants);
	
	int edgeId = 0;
	for(BasicBlock::OutIterator outs(bb); outs; outs++)
	{
		if(outs->kind() == Edge::TAKEN
		|| outs->kind() == Edge::NOT_TAKEN
		|| outs->kind() == Edge::VIRTUAL
		|| outs->kind() == Edge::VIRTUAL_RETURN) // Filter out irrelevant edges (calls...)
		{	
			SLList<Predicate> &relevant_preds = (outs->kind() == Edge::TAKEN) ?
				generated_preds_taken_backup :
				generated_preds_backup;
			
			if(edgeId++) // if this is not the first valid edge
			{
				labelled_preds += top_list_backup; // copy the predicates we have generated until this node into a new list
				constants = constants_backup; // also reset the constants
			}			

			// label our list of predicates with the current edge then append it
			SLList<LabelledPredicate> labelled_analysis_result = labelPredicateList(relevant_preds, *outs);
			// label the constants as well
			constants.label(*outs);
			
			// Add result to topList
			SLList<LabelledPredicate> topList = labelled_preds.first();
			topList += labelled_analysis_result;
			labelled_preds.removeFirst();
			labelled_preds.addFirst(topList);

			processEdge(*outs);
		}
	}
}

void Analysis::placeboProcessBB(BasicBlock* bb)
{
	if(bb->isExit())
	{
		total_paths++;
		return;
	}

	for(BasicBlock::OutIterator outs(bb); outs; outs++)
	{
		if(outs->kind() == Edge::TAKEN
		|| outs->kind() == Edge::NOT_TAKEN
		|| outs->kind() == Edge::VIRTUAL
		|| outs->kind() == Edge::VIRTUAL_RETURN) // Filter out irrelevant edges (calls...)
		{	
			placeboProcessBB((*outs)->target());
		}
	}
}

void Analysis::processEdge(const Edge* edge)
{
	BasicBlock* target = edge->target();
	// DBG(COLOR_Whi << "Processing Edge: " << edge->source()->number() << "->" << (*subiter)->target()->number());)	
	DBG(COLOR_Whi << "Processing Edge: " << edge)	
	// DBG("State of the analysis: " << labelled_preds)	
	processBB(target);
}

SLList<LabelledPredicate> Analysis::labelPredicateList(const SLList<Predicate>& pred_list, const Edge* label)
{
	SLList<const Edge*> sll_containing_only_label;
	sll_containing_only_label += label;

	SLList<LabelledPredicate> LP_list;
	for(SLList<Predicate>::Iterator preds(pred_list); preds; preds++)
	{
		LabelledPredicate lp(*preds, sll_containing_only_label);
		LP_list.addFirst(lp);
	}
	return LP_list;
}
