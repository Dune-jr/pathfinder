#include "analysis.h"
#include <otawa/cfg/Edge.h>

void addIndents(io::Output& out, int n) { for(int i=0; i<n; i++) out << "\t"; }

Analysis::Analysis(CFG *cfg, int sp_id, unsigned int given_max_tempvars, unsigned int given_max_registers)
	: sp(sp_id), max_tempvars(given_max_tempvars), max_registers(given_max_registers)
{
	DBG("Stack pointer identified to " << sp)
	processCFG(cfg);
}

// PredIterator
void Analysis::State::setPredicate(PredIterator &iter, const LabelledPredicate &labelled_predicate)
{
	ASSERT(!iter.ended());
	if(iter.state == PredIterator::GENERATED_PREDS)
		generated_preds.set(iter.gp_iter, labelled_predicate);
	else if(iter.state == PredIterator::LABELLED_PREDS)
		labelled_preds.first().set(iter.lp_iter, labelled_predicate);
	else DBG(color::BIRed() << "Analysis::setPredicate(): unhandled iter.state!")
}

/*
 * move predicates to the local list (so that they get updated with an edge)
 * WARNING: this may result in iter.ended()!
**/
void Analysis::State::movePredicateToGenerated(PredIterator &iter)
{
	if(iter.state == PredIterator::GENERATED_PREDS)
		return; // do not do anything
	else if(iter.state == PredIterator::LABELLED_PREDS)
	{
		generated_preds += *iter;
		labelled_preds.first().remove(iter.lp_iter);
		iter.updateState();
	}
	else DBG(color::BIRed() << "Analysis::movePredicateToGenerated(): unhandled iter.state!")
}

void Analysis::State::removePredicate(PredIterator &iter)
{
	ASSERT(!iter.ended());
	if(iter.state == PredIterator::GENERATED_PREDS)
		generated_preds.remove(iter.gp_iter);
	else if(iter.state == PredIterator::LABELLED_PREDS)
		labelled_preds.first().remove(iter.lp_iter);
	else DBG(color::BIRed() << "Analysis::removePredicate(): unhandled iter.state!")
	iter.updateState();
}

SLList<LabelledPredicate> Analysis::State::labelPredicateList(const SLList<LabelledPredicate>& pred_list, Edge* label)
{
	SLList<LabelledPredicate> rtn;
	for(SLList<LabelledPredicate>::Iterator iter(pred_list); iter; iter++)
	{
		LabelledPredicate lp(*iter);
		lp.addLabel(label);
		rtn += lp;
	}
	return rtn;
}

elm::String Analysis::State::pathToString() const
{
	elm::String str;
	bool first = true;
	int lastid = 0; // all paths must start with 0 (ENTRY)
	for(OrderedPath::Iterator iter(path); iter; iter++)
	{
		ASSERT((*iter)->source()->number() == lastid); // when path is x->y and y'->z, there must be y=y'
		if(first)
		{
#			ifndef NO_UTF8
				str = _ << "ε";
#			else
				str = _ << "0";
#			endif
			first = false;
		}
		str = _ << str << "->" << (*iter)->target()->number();
		lastid = (*iter)->target()->number();
	}
	return str;
}

io::Output& Analysis::State::print(io::Output& out) const
{
	// out << ":" << labelled_preds << "/" << constants;
	return (out << pathToString());
}

/* 
	inline SLList<Path> infeasiblePaths(void)
*/