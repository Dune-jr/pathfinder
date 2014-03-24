#include <elm/io.h>
#include <elm/io/Output.h>
#include <otawa/cfg/CFG.h>
#include <otawa/cfg/Edge.h>
#include <otawa/cfg/BasicBlock.h>

#include "cfganalysis.h"
#include "debug.h"
// #include <stdio.h> // while (getchar() != '\n') ;

CFGAnalysis::LabeledPredicate::LabeledPredicate(Predicate pred, Edge* label)
	: _pred(pred), _label(label) { }

CFGAnalysis::CFGAnalysis(CFG *cfg)
{
	processCFG(cfg);
}

// WARNING: atm, this function assumes we have NO LOOPS!
void CFGAnalysis::processCFG(CFG *cfg)
{
	DBG("Processing CFG " << cfg)
	processBB(cfg->firstBB());
}

void CFGAnalysis::processBB(BasicBlock *bb)
{
	if(bb->isExit())
	{
		DBG(COLOR_ICya << "EXIT block reached" << COLOR_RCol)
		return;
	}
		
	DBG("Processing " << bb)
	
	// Read instructions and update the predicates
	
	for(BasicBlock::OutIterator outs(bb); outs; outs++)
	{
		if(outs->kind() == Edge::TAKEN
		|| outs->kind() == Edge::NOT_TAKEN
		|| outs->kind() == Edge::VIRTUAL
		|| outs->kind() == Edge::VIRTUAL_RETURN)
			processEdge(*outs);
	}
}

void CFGAnalysis::processEdge(Edge *edge)
{
	BasicBlock* target = edge->target();
	DBG("Processing Edge: " << edge)
	
	processBB(target);
}
