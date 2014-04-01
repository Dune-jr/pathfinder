#include "analysis.h"

#include <otawa/cfg/Edge.h>

void addIndents(io::Output& out, int n) { for(int i=0; i<n; i++) out << "\t"; }

Analysis::Analysis(CFG *cfg)
{
	initializeAnalysis();
	processCFG(cfg);
}

Analysis::LabelledPredicate::LabelledPredicate(const Predicate& pred, const Edge* label)
	: _pred(pred), _label(label) { }
	
io::Output& Analysis::LabelledPredicate::print(io::Output &out) const
{
	out << "(" << _pred << " | "  << _label->source()->number() << "->" << _label->target()->number() << ")";
	return out;
}

SLList<Analysis::LabelledPredicate> Analysis::getTopList()
{
	return labelled_preds.first();
}

void Analysis::setTopList(const SLList<LabelledPredicate>& lps)
{
	labelled_preds.removeFirst();
	labelled_preds.addFirst(lps);
}
