#include "analysis.h"
#include <otawa/cfg/Edge.h>

void addIndents(io::Output& out, int n) { for(int i=0; i<n; i++) out << "\t"; }

Analysis::Analysis(CFG *cfg, int sp_id, unsigned int max_tempvars, unsigned int max_registers)
	: sp(sp_id), constants(max_tempvars, max_registers)
{
	DBG("Stack pointer identified to " << sp)	
	initializeAnalysis();
	processCFG(cfg);
}

// PredIterator
void Analysis::setPredicate(PredIterator &iter, const LabelledPredicate &labelled_predicate)
{
	ASSERT(!iter.ended());
	if(iter.state == PredIterator::GENERATED_PREDS)
		generated_preds.set(iter.gp_iter, labelled_predicate.pred());
	else if(iter.state == PredIterator::LABELLED_PREDS)
		labelled_preds.first().set(iter.lp_iter, labelled_predicate);
	else DBG(color::BIRed() << "Analysis::removePredicate(): unhandled iter.state!")
}

void Analysis::removePredicate(PredIterator &iter)
{
	ASSERT(!iter.ended());
	if(iter.state == PredIterator::GENERATED_PREDS)
		generated_preds.remove(iter.gp_iter);
	else if(iter.state == PredIterator::LABELLED_PREDS)
		labelled_preds.first().remove(iter.lp_iter);
	else DBG(color::BIRed() << "Analysis::removePredicate(): unhandled iter.state!")
	iter.updateState();
}

/* 
	inline SLList<Path> infeasiblePaths(void)
*/