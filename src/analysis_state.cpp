/*
 * General analysis::state methods
 */

#include "analysis_state.h"
// #include "debug.h"

/**
 * @class Analysis::State
 * @brief Abstract state of a set of path of the program
 */
const Analysis::State bottom(true);

Analysis::State::State(bool bottom) : dfa_state(NULL), sp(0), dag(null<DAG>()), lvars(), mem(53), bottom(bottom)
#ifdef V1
	, constants()
#endif
	{ }

// Analysis::State::State(const context_t& context)
	// : dfa_state(context.dfa_state), sp(context.sp), bottom(true), constants(context.max_tempvars, context.max_registers) { }

/*Analysis::State::State(Block* entryb, const context_t& context, bool init)
	: dfa_state(context.dfa_state), sp(context.sp), dag(context.dag), lvars(*dag, context.max_tempvars, context.max_registers),
	mem(53), bottom(false)
#ifdef V1
	, constants(context.max_tempvars, context.max_registers)
#endif
{
	generated_preds.clear(); // generated_preds := [[]]
	labelled_preds.clear(); // labelled_preds := [[]]
	if(init)
	{
		// Block::EdgeIter outs(entryb->outs());
		// ASSERT(outs);
		path.addLast(theOnly(entryb->outs()));
#ifdef V1
		constants.set(sp, SP, Set<Edge*>::null, false); // set that ?13==SP (since SP is the value of ?13 at the beginning of the program)
#endif
		set(sp, dag->cst(SP));
	}
}*/

Analysis::State::State(Edge* entry_edge, const context_t& context, bool init)
	: dfa_state(context.dfa_state), sp(context.sp), dag(context.dag), lvars(*dag, context.max_tempvars, context.max_registers),
	mem(53), bottom(false)
#ifdef V1
	, constants(context.max_tempvars, context.max_registers)
#endif
{
	generated_preds.clear(); // generated_preds := [[]]
	labelled_preds.clear(); // labelled_preds := [[]]
	if(init)
	{
		path.addLast(entry_edge);
#ifdef V1
		constants.set(sp, SP, Set<Edge*>::null, false); // set that ?13==SP (since SP is the value of ?13 at the beginning of the program)
#endif
		set(sp, dag->cst(SP));
	}
}

Analysis::State::State(const State& s)
	: dfa_state(s.dfa_state), sp(s.sp), dag(s.dag), lvars(s.lvars), mem(s.mem), bottom(s.bottom), path(s.path),
#ifdef V1
	constants(s.constants),
#endif
	  labelled_preds(s.labelled_preds), generated_preds(s.generated_preds), generated_preds_taken(s.generated_preds_taken)//, fixpoint(s.fixpoint)
	{ }

void Analysis::State::appendEdge(Edge* e)
{
	// add edge to the end of the path
	this->path.addLast(e);
	// we now need to label the correct list of predicates
	SLList<LabelledPredicate> &relevant_preds = (isConditional(e->source()) && e->isTaken())
		? generated_preds_taken // conditional TAKEN
		: generated_preds; // non-conditional, NOT TAKEN
	labelled_preds += labelPredicateList(relevant_preds, e); // label our list of predicates with the current edge then append it
#ifdef V1
		constants.label(e); // label the constants as well
#endif
	relevant_preds.clear(); // TODO!! this is too strong in case of multiple not taken...
	// v2
	lvars.onEdge(e);
}


// PredIterator
void Analysis::State::setPredicate(PredIterator &iter, const LabelledPredicate &labelled_predicate)
{
	ASSERT(!iter.ended());
	if(iter.state == PredIterator::GENERATED_PREDS)
		generated_preds.set(iter.gp_iter, labelled_predicate);
	else if(iter.state == PredIterator::LABELLED_PREDS)
		labelled_preds.set(iter.lp_iter, labelled_predicate);
	else ASSERTP(false, "Analysis::setPredicate(): unhandled iter.state!")
}

/**
 * @fn void Analysis::State::movePredicateToGenerated(PredIterator &iter);
 * @brief Move predicates to the local list (so that they get updated with an edge)
 * @warning this may result in iter.ended()!
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
	else ASSERTP(false, "Analysis::movePredicateToGenerated(): unhandled iter.state!")
}

void Analysis::State::removePredicate(PredIterator &iter)
{
	ASSERT(!iter.ended());
	if(iter.state == PredIterator::GENERATED_PREDS)
		generated_preds.remove(iter.gp_iter);
	else if(iter.state == PredIterator::LABELLED_PREDS)
		labelled_preds.remove(iter.lp_iter);
	else ASSERTP(false, "Analysis::removePredicate(): unhandled iter.state!")
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
	if(!isBottom())
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
 * @brief      This function is *this -> s -> s o *this, state composition
 * @param[in]  s     state to apply
 */
/*void Analysis::State::apply(const State& s)
{

}
*/
/**
 * @fn void Analysis::State::accel();
 * @param *this State after one iteration (starting from T)
 * @param s0 Initial state (before the loop)
 */
/*void Analysis::State::accel(const State& s0)
{
	// deal with lvars
	for(LocalVariables::Iter iter(lvars); iter; iter++)
	{
		if(*iter) // iter was modified
		{
			const OperandVar x = iter->id();
			if(iter->isAffineIn(x))
			{

			}
			else
			{
				DBG(color::IRed() << x << " too complex to accel: " << *iter)
				lvars[x] = NULL;
			}
		}
	}

	// deal with memory
	for(mem_t::PairIterator iter(mem); iter; iter++)
	{
		const Constant addr = mem.fst;
	}
}*/

/**
 * @brief merge all states into one (a bit brutal). Does not take in account current state
 *
 * @param cl Collection of States to process (accepts SLList, Vector etc.)
 */
void Analysis::State::merge(const States& ss, Block* b)
{
	ASSERTP(!ss.isEmpty(), "call to Analysis::State::merge with empty ss parameter"); // maybe just leave the state empty
	DBGG("-\tmerging from " << ss.count() << " state(s).")
	// resetting stuff
	generated_preds.clear();
	generated_preds_taken.clear();
	labelled_preds.clear();
#ifdef V1
	SLList<ConstantVariables> cvl;
#endif
	lvars = ss.first().lvars;
	mem = ss.first().mem;
	// const mem_t* mtab[ss.count()];
	// int i = 0;
	// intialize to first element
#ifdef V1
	constants = ss.first().constants;
#endif
	// copy firstElement.labelled_preds into labelled_preds with empty labels
	for(SLList<LabelledPredicate>::Iterator iter(ss.first().labelled_preds); iter; iter++)
		labelled_preds += LabelledPredicate(iter->pred(), Path::null);
	bool first = true;
	for(States::Iterator siter(ss.states()); siter; siter++)
	{
		if(first) // the first element is s itself, it's useless to merge s with s
		{
			first = false;
			continue;
		}

		// lvars = lvars ∩ siters->lvars
		lvars.merge(siter->lvars);
		// mem = mem ∩ siters->mem
		const mem_t& smem = siter->mem;
		for(mem_t::PairIterator i(mem); i; i++)
		{
			if((*i).snd != smem.get((*i).fst, NULL)) // for each (k, v) in mem, if smem[k] != v, invalidate mem[k]
				mem[i] = Top;
		}
		for(mem_t::PairIterator i(smem); i; i++)
			if((*i).snd != mem.get((*i).fst, NULL)) // for each (k, v) in smem, if mem[k] != v, invalidate mem[k]
				mem[i] = Top;
		// lvtab[i] = &(siter->lvars);
		// mtab[i++] = &(siter->mem);
#ifdef V1
		cvl += (*siter).constants; // constants.merge(...) uses the info from "constants" so it's useless to add it at the first iteration
#endif
		// for each element of labelled_preds, we make sure it is in *siter
		for(SLList<LabelledPredicate>::Iterator iter(labelled_preds); iter; )
		{
			// do not use 'if(s.labelled_preds.contains(*iter))' as we want to use Predicate::operator== and not LabelledPredicate::operator==
			bool contains = false;
			for(SLList<LabelledPredicate>::Iterator subiter((*siter).labelled_preds); subiter; subiter++)
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
	}
#ifdef V1
	this->constants.merge(cvl);
#endif
	// this->path.merge(stateListToPathVector(sc)); // merge paths as well while keeping some flow info and shrink that in this->path
	// this-path = DetailedPath(sc.first().lastEdge()->target()->toBasic());
	this->path.clear();
	// this->path.fromContext(sc.first().lastEdge()->target()->toBasic());
	this->path.fromContext(b);
}
/*void Analysis::State::merge(const SLList<State>& sl)
{
	DBGG("-\tmerging " << sl)
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
	// cout << color::Pur() << "result of merge, this->path=" << this->path.toString() << color::Pur() << ", *this=" << *this << color::RCol() << io::endl;
#endif
}*/

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

/**
 * @fn void Analysis::State::throwInfo();
 * throw all gathered info on current state
 */
/*void Analysis::State::throwInfo()
{
	DBG(color::IRed() << "throwing info from path " << getPathString())
	path.clear();
	constants.clear();
	labelled_preds.clear();
	generated_preds.clear();
	generated_preds_taken.clear();
}*/

/**
 * @fn inline Analysis::State Analysis::topState(Block* entry) const;
 * @brief Returns a Top state
 */
Analysis::State Analysis::topState(Block* entry) const
{
	return Analysis::State(theOnly(entry->outs()), context, true);
}

elm::String Analysis::State::dumpEverything() const
{
	elm::String rtn = _
		<< "--- DUMPING WHOLE STATE ---" << endl
		<< "  * path=" << getPathString() << endl
#ifdef V1
		<< "  * constants=" << constants << endl
#endif
		<< "  * labelled_preds=" << labelled_preds << endl
		<< "  * generated_preds=" << generated_preds << endl
		<< "  * lvars= [" << endl << lvars << "]" << endl
		<< "  * mem= [" << endl;
	for(mem_t::PairIterator i(mem); i; i++)
		rtn = _ << rtn << "        [" << (*i).fst << "]   \t| " << *(*i).snd << endl;
	return _ << rtn << "]" << endl << "\t--- END OF DUMP ---";
}

// this is not Leibniz equality, but a test to check for a fixpoint!
// <!> this compares labelled_preds only <!>
bool Analysis::State::equiv(const Analysis::State& s) const
{
	if(s.isBottom())
		return this->isBottom();
	ASSERT(this->sp == s.sp);
	// do not check the path or any of the edges!
	/*if(generated_preds != generated_preds)
		return false;
	if(generated_preds_taken != generated_preds_taken)
		return false;
	*/
#ifdef V1
	if(! this->constants.sameValuesAs(s.constants))
		return false;
#endif
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
	DBGG("-	" << color::IGre() << "FIXPOINT!")
	DBG(s.dumpEverything())
	return true;
}

/**
 * @fn void Analysis::State::removeConstantPredicates();
 * @brief Removes constant predicates. Useful after a SMT call returning SAT, as the constant predicates of such states must be tautologies
 */
void Analysis::State::removeConstantPredicates()
{	
	for(PredIterator piter(*this); piter; )
	{
		if(piter.pred().isConstant())
		{
			DBG(color::IPur() << DBG_SEPARATOR << color::IYel() << " - " << *piter)
			removePredicate(piter);
		}
		else piter++;
	}
}
