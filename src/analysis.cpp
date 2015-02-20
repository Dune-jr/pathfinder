#include "analysis.h"
#include <otawa/cfg/Edge.h>

void addIndents(io::Output& out, int n) { for(int i=0; i<n; i++) out << "\t"; }

Analysis::Analysis(CFG *cfg, const dfa::State *state, int sp_id, unsigned int given_max_tempvars, unsigned int given_max_registers)
	: dfa_state(state), sp(sp_id), max_tempvars(given_max_tempvars), max_registers(given_max_registers)
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
		labelled_preds.set(iter.lp_iter, labelled_predicate);
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
		labelled_preds.remove(iter.lp_iter);
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
		labelled_preds.remove(iter.lp_iter);
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

elm::String Analysis::State::getPathString() const
{
	elm::String str;
	bool first = true;
	// int lastid = 0; // all paths must start with 0 (ENTRY) // no longer
	int lastid;
	for(OrderedPath::Iterator iter(path); iter; iter++)
	{
		ASSERTP(first || (*iter)->source()->number() == lastid, "OrderedPath previous target and current source do not match! ex: 1->2, 2->4, 3->5"); // when path is x->y and y'->z, there must be y=y'
		if(first)
		{
#			ifndef NO_UTF8
				if((*iter)->source()->number() == 0)
					str = _ << "ε";
				else
#			endif
				str = _ << (*iter)->source()->number();
			first = false;
		}
		str = _ << str << "->" << (*iter)->target()->number();
		lastid = (*iter)->target()->number();
	}
	if(str.isEmpty())
		return "(empty)";
	return str;
}

io::Output& Analysis::State::print(io::Output& out) const
{
	// out << ":" << labelled_preds << "/" << constants;
	return (out << getPathString());
}

// TODO! make it so that all predicates or constants take as only edge the current BB -> ...
void Analysis::State::merge(const SLList<State>& sl)
{
	path.clear();
	generated_preds_taken.clear(); // TODO: is that good?
	SLList<ConstantVariables> cvl;
	for(SLList<State>::Iterator sl_iter(sl); sl_iter; sl_iter++)
	{
		const State& s = *sl_iter;
		// build intersection of SLLists // TODO: isn't a better way to do this? implement a SLList method or something?
		for(SLList<LabelledPredicate>::Iterator iter(generated_preds); iter; )
		{
			if(s.generated_preds.contains(*iter))
				iter++;
			else generated_preds.remove(iter);
		}
		for(SLList<LabelledPredicate>::Iterator iter(labelled_preds); iter; )
		{
			if(s.labelled_preds.contains(*iter))
				iter++;
			else {
				// DBG(s.labelled_preds << ".notContains(" << *iter << ")")
				labelled_preds.remove(iter);
			}
		}
		cvl += s.constants;
	}
	constants.merge(cvl);
}

void Analysis::State::dumpEverything() const
{
	DBG("--- DUMPING WHOLE STATE---")
	DBG("OrderedPath path=" << getPathString())
	DBG("ConstantVariables constants=" << constants)
	DBG("SLList<LabelledPredicate> labelled_preds=" << labelled_preds)
	DBG("SLList<LabelledPredicate> generated_preds=" << generated_preds)
	DBG("SLList<LabelledPredicate> generated_preds_taken=" << generated_preds_taken)
	DBG("--- END OF DUMP OF WHOLE STATE---")
}