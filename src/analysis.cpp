#include "analysis.h"
#include <otawa/cfg/Edge.h>

void addIndents(io::Output& out, int n) { for(int i=0; i<n; i++) out << "\t"; }

Analysis::Analysis(CFG *cfg, const dfa::State *dfa_state, int sp, unsigned int max_tempvars, unsigned int max_registers, int state_size_limit, int flags)
	: dfa_state(dfa_state), sp(sp), max_tempvars(max_tempvars), max_registers(max_registers), state_size_limit(state_size_limit), flags(flags)
	, loop_header_count(0), bb_count(cfg->countBB()-1) // do not count ENTRY
	, ip_count(0), unminimized_ip_count(0)
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
	int lastid;
	for(OrderedPath::Iterator iter(path); iter; iter++)
	{
		if(!first && (*iter)->source()->number() != lastid)
		{
			DBG("str=" << str)
			DBG("lastid=" << lastid << ", (*iter)->source->number()=" << (*iter))
		}
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
	if(isValid())
		return (out << getPathString());
	else
#		ifndef NO_UTF8
			return (out << "⊥");
#		else
			return (out << "(bottom)");
#		endif
}

/**
 * @fn void Analysis::State::merge(const SLList<State>& sl, Edge* e);
 * Modifies the current state to be the result of the merge of a (SL)list of states 
 */
void Analysis::State::merge(const SLList<State>& sl)
{
	// resetting stuff
	generated_preds.clear();
	generated_preds_taken.clear();
	labelled_preds.clear();
	SLList<ConstantVariables> cvl;
	// intialize to first element
	ASSERTP(!sl.isEmpty(), "call to Analysis::State::merge with empty sl parameter"); // maybe just leave the state empty
	constants = sl.first().constants;
	// copy firstElement.labelled_preds into labelled_preds with empty labels
	for(SLList<LabelledPredicate>::Iterator iter(sl.first().labelled_preds); iter; iter++)
		labelled_preds += LabelledPredicate(iter->pred(), Path::null);
	bool first = true;
	for(SLList<State>::Iterator sl_iter(sl); sl_iter; sl_iter++)
	{
		if(first) // the first element is s itself, it's useless to merge s with s
		{
			first = false;
			continue;
		}
		// for each element of labelled_preds, we make sure it is in *sl_iter
		for(SLList<LabelledPredicate>::Iterator iter(labelled_preds); iter; )
		{
			// do not use 'if(s.labelled_preds.contains(*iter))' as we want to use Predicate::operator== and not LabelledPredicate::operator==
			bool contains = false;
			for(SLList<LabelledPredicate>::Iterator subiter((*sl_iter).labelled_preds); subiter; subiter++)
			{
				if((*subiter).pred() == iter->pred())
				{
					contains = true;
					break;
				}
			}
			if(contains)
				iter++;
			else
				labelled_preds.remove(iter);
		}
		cvl += (*sl_iter).constants;
	}
	constants.merge(cvl);
}

/**
 * @fn void Analysis::State::merge(const SLList<State>& sl, Edge* e);
 * Modifies the current state to be the result of the merge of a (SL)list of states 
 */
void Analysis::State::merge(const SLList<State>& sl, Edge* e)
{
	DBG(color::Whi() << "Merging " << sl.count() << " paths at BB " << e->source()->number())
	merge(sl);
	labelled_preds = labelPredicateList(labelled_preds, e);
	constants.label(e);
	path.clear();
	DBG("Merged predicates: " << generated_preds << ", " << constants)
}

elm::String Analysis::State::dumpEverything() const
{
	return _
		<< "--- DUMPING WHOLE STATE ---" << endl
		<< "  * OrderedPath path=" << getPathString() << endl
		<< "  * ConstantVariables constants=" << constants << endl
		<< "  * SLList<LabelledPredicate> labelled_preds=" << labelled_preds << endl
		// << "  * SLList<LabelledPredicate> generated_preds=" << generated_preds << endl
		// << "  * SLList<LabelledPredicate> generated_preds_taken=" << generated_preds_taken << endl
		<< "--- END OF DUMP ---" << endl;
}

// this is not Leibniz equality, but a test to check for a fixpoint!
// <!> this compares labelled_preds only <!>
bool Analysis::State::isFixPoint(const Analysis::State& s) const
{
	assert(this->sp == s.sp);
	// do not check the path or any of the edges!
	/*if(generated_preds != generated_preds)
		return false;
	if(generated_preds_taken != generated_preds_taken)
		return false;
	*/
	if(!this->constants.sameValuesAs(s.constants))
		return false;
	// checking for this->labelled_preds.sameValuesAs(s.labelled_preds)
	if(this->labelled_preds.count() != s.labelled_preds.count())
		return false;
	for(SLList<LabelledPredicate>::Iterator self_iter(this->labelled_preds); self_iter; self_iter++)
	{
		bool contains = false;
		for(SLList<LabelledPredicate>::Iterator s_iter(s.labelled_preds); s_iter; s_iter++)
		{
			if(self_iter->pred() == s_iter->pred())
			{
				contains = true;
				break;
			}
		}
		if(!contains)
			return false;
	}
	return true;
}

bool Analysis::listOfFixpoints(const SLList<Analysis::State>& sl)
{
	for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++)
	{
		if(!(sl_iter->fixpointState()))
			return false;
	}
	return true;
}