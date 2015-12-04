/*
 * General analysis methods
 */

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

io::Output& Analysis::State::print(io::Output& out) const
{
	// out << ":" << labelled_preds << "/" << constants;
	if(isValid())
		// return (out << getPathString()); // TODO!
		return (out << path);
	else
#		ifndef NO_UTF8
			return (out << "⊥");
#		else
			return (out << "(bottom)");
#		endif
}

/**
 * @fn void Analysis::State::merge(const SLList<State>& sl);
 * Modifies the current state to be the result of the merge of an (SL)list of states 
 */
void Analysis::State::merge(const SLList<State>& sl)
{
	DBG("merging " << sl)
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
		cvl += (*sl_iter).constants; // constants.merge(...) uses the info from "constants" so it's useless to add it at the first iteration
	}
	this->constants.merge(cvl);
	this->path.merge(stateListToPathVector(sl)); // merge paths as well while keeping some flow info and shrink that in this->path
#ifdef DBGG
	cout << color::Pur() << "result of merge, this->path=" << this->path.toString() << color::Pur() << ", *this=" << *this << color::RCol() << io::endl;
#endif
}

/*
 * @fn void Analysis::State::merge(const SLList<State>& sl, Edge* e);
 * Modifies the current state to be the result of the merge of an (SL)list of states 
 */
/*void Analysis::State::merge(const SLList<State>& sl, Edge* e)
{
	DBG(color::Whi() << "Merging " << sl.count() << " paths at BB " << e->source()->number())
	merge(sl);
	labelled_preds = labelPredicateList(labelled_preds, e);
	constants.label(e);
	path.merge(stateListToPathVector(sl)); // clears up path and merge all of sl into 1
	DBG("Merged predicates: " << generated_preds << ", " << constants)
}*/

Vector<DetailedPath> Analysis::State::stateListToPathVector(const SLList<State>& sl) const
{
	Vector<DetailedPath> rtn;
	for(SLList<State>::Iterator iter(sl); iter; iter++)
		rtn.add(iter->getDetailedPath());
	return rtn;
}

void Analysis::debugProgress(int bb_number, bool enable_smt) const
{
	if(dbg_verbose >= DBG_VERBOSE_RESULTS_ONLY && (dbg_flags&DBG_PROGRESS))
	{
		static int processed_bbs = 0;
		if(enable_smt)
			++processed_bbs; // only increase processed_bbs when we are in a state where we are no longer looking for a fixpoint
		cout << "[" << processed_bbs*100/bb_count << "%] Processed BB #" << bb_number << " of " << bb_count << "        " << endl << "\e[1A";
	}
}

elm::String Analysis::State::dumpEverything() const
{
	return _
		<< "--- DUMPING WHOLE STATE ---" << endl
		<< "  * OrderedPath path=" << getPathString() << endl
		<< "  * ConstantVariables constants=" << constants << endl
		<< "  * SLList<LabelledPredicate> labelled_preds=" << labelled_preds << endl
		<< "  * SLList<LabelledPredicate> generated_preds=" << generated_preds << endl
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

/**
 * @brief Computes a conjunction over the fixpoint booleans of all the states
 */
bool Analysis::listOfFixpoints(const SLList<Analysis::State>& sl)
{
	for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++)
	{
		if(!(sl_iter->fixpointState()))
			return false;
	}
	return true;
}