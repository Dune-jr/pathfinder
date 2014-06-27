#include "analysis.h"
#include <otawa/cfg/Edge.h>

void addIndents(io::Output& out, int n) { for(int i=0; i<n; i++) out << "\t"; }

Analysis::Analysis(CFG *cfg, int sp_id) : sp(sp_id)
{
	initializeAnalysis();
	processCFG(cfg);
}

Analysis::LabelledPredicate::LabelledPredicate(const Predicate& pred, SLList<const Edge*> labels)
	: _pred(pred), _labels(labels) { }
	
io::Output& Analysis::LabelledPredicate::print(io::Output &out) const
{
	out << "(" << _pred << " | ";
	bool first_time = true;
	for(SLList<const Edge*>::Iterator iter(_labels); iter; iter++)
	{
		if(first_time)
			first_time = false;
		else
			out << ", ";
		out << (*iter)->source()->number() << "->" << (*iter)->target()->number();
	}
	return (out << ")");
}

void Analysis::addElemToTopList(const SLList<LabelledPredicate>& lps)
{	
	SLList<LabelledPredicate> topList = getTopList(); // get topList
	topList.addAll(lps);							  // append to topList
	setTopList(topList); 							  // set topList as new first item of attribute labelled_preds
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
