#ifndef _ANALYSIS_STATE_H
#define _ANALYSIS_STATE_H

#include <elm/avl/Set.h>
#include <elm/genstruct/SLList.h>
#include <otawa/cfg/Edge.h>
#include <otawa/cfg/features.h>
#include <otawa/dfa/State.h> // dfa::State: isInitialized(addr), get(addr, _)...
#include "analysis.h"
#include "constant_variables.h"
#include "detailed_path.h"
#include "labelled_predicate.h"

using namespace otawa;
using elm::genstruct::SLList;
using elm::genstruct::Vector;

class Analysis::State {
private:
	const dfa::State* dfa_state;
	OperandVar sp; // the Stack Pointer register
	// OrderedPath path;
	bool bottom;
	DetailedPath path;
	ConstantVariables constants; // remember in an array the variables that have been identified to a constant (e.g. t2 = 4)
	SLList<LabelledPredicate> labelled_preds; // previously generated predicates
	SLList<LabelledPredicate> generated_preds; // predicates local to the current BB
	SLList<LabelledPredicate> generated_preds_taken; // if there is a conditional, the taken preds will be saved here and the not taken preds will stay in generated_preds
		// that have been updated and need to have their labels list updated (add the next edge to the LabelledPreds struct)
	// bool fixpoint;
	class PredIterator;

public:
	State(bool bottom = false); // false: create an invalid state, true: create a bottom state
	State(Block* entryb, const context_t& context, bool init = true);
	State(Edge* entry_edge, const context_t& context, bool init = false);
	State(const State& s);
	inline const DetailedPath& getDetailedPath() const { return path; }
	inline Edge* lastEdge() const { return path.lastEdge(); }
	inline const SLList<LabelledPredicate>& getLabelledPreds() const { return labelled_preds; }
	inline const ConstantVariables& getConstants() const { return constants; }
	inline elm::String getPathString() const { return path.toString(); /*orderedPathToString(path.toOrderedPath());*/ }
	inline void onLoopEntry(Block* loop_header) { path.onLoopEntry(loop_header); }
	inline void onLoopExit(Option<Block*> maybe_loop_header = elm::none) { path.onLoopExit(maybe_loop_header); }
	inline void onCall(SynthBlock* sb)   { path.onCall(sb); }
	inline void onReturn(SynthBlock* sb) { path.onReturn(sb); }
	inline bool isBottom() const { return bottom; }
	inline bool isValid() const { return dfa_state != NULL && constants.isValid(); } // this is so that we can have empty states that do not use too much memory


	// analysis_state.cpp
	template <class C> Vector<DetailedPath> stateListToPathVector(const C& sl) const;
	elm::String dumpEverything() const;
	template<template< class _ > class C> void merge(const C<Analysis::State>& cl, Block* b);
	bool equiv(const State& s) const;
	void appendEdge(Edge* e, bool is_conditional);

	// analysis_bb.cpp
	void processBB(const BasicBlock *bb);
	// void throwInfo();
	int invalidateStackBelow(const Constant& stack_limit);

	inline void dumpPredicates() const { for(PredIterator iter(*this); iter; iter++) DBG(*iter); }
	inline const State* operator->(void) const { return this; }
	friend io::Output& operator<<(io::Output& out, const State& s) { return s.print(out); }

private:
	// Private methods
	// analysis.cpp
	void setPredicate(PredIterator &iter, const LabelledPredicate &labelled_predicate);
	void movePredicateToGenerated(PredIterator &iter);
	void removePredicate(PredIterator &iter);
	SLList<LabelledPredicate> labelPredicateList (const SLList<LabelledPredicate>& pred_list, Edge* label);
	io::Output& print(io::Output& out) const;

	// analysis_bb.cpp
	LabelledPredicate makeLabelledPredicate(condoperator_t opr, Operand* opd1, Operand* opd2, Path& labels) const;
	bool tryToKeepVar(const OperandVar& var);//, const Predicate*& removed_predicate);
	bool invalidateVar(const OperandVar& var, bool invalidate_constant_info = true);
	bool invalidate(const Operand& var, bool invalidate_constant_info = true);
	bool invalidateMem(const OperandMem& addr);
	bool invalidateMem(const OperandVar& var);
	bool invalidateTempVars();
	bool replaceOperand(const Operand& opd, const Operand& expr);
	bool replaceVar(const OperandVar& var, const Operand& expr);
	bool replaceMem(const OperandMem& opd, const Operand& expr, const Path& labels);
	bool replaceTempVar(const OperandVar& temp_var, const Operand& expr);
	bool replaceTempVar(const OperandMem& mem, const Operand& expr);
	bool update(const OperandVar& opd_to_update, const Operand& opd_modifier, Path& labels);
	Option<OperandConst> findConstantValueOfVar(const OperandVar& var); // changed to a simple lookup to "constants"
	Option<OperandConst> findConstantValueOfMemCell(const OperandMem& mem, Path &labels);
	Option<t::int32> findStackRelativeValueOfVar(const OperandVar& var, Path& labels);
	bool findValueOfCompVar(const OperandVar& var, Operand*& opd_left, Operand*& opd_right, Path& labels);
	Option<OperandMem> getOperandMem(const OperandVar& var, Path& labels);
	bool invalidateAllMemory();
	void updateLabelsWithReplacedConstantsInfo(Path& labels, const Vector<OperandVar>& replaced_vars) const;
	Predicate* getPredicateGeneratedByCondition(sem::inst condition, bool taken, Path& labels);
	Option<OperandConst> getConstantValueOfReadOnlyMemCell(const OperandMem& addr_mem, otawa::sem::type_t type);
	inline bool isConstant(const OperandVar& var) const { return constants.isConstant(var); }
	inline elm::avl::Set<Edge*> getLabels(const OperandVar& opdv) const { return constants.getLabels(opdv); }
	inline elm::avl::Set<Edge*> getLabels(const OperandVar& opdv1, const OperandVar& opdv2) const { return constants.getLabels(opdv1, opdv2); }

	// PredIterator class
	class PredIterator: public PreIterator<PredIterator, const LabelledPredicate&> {
		enum pred_iterator_state_t
		{
			GENERATED_PREDS, // must have !gp_iter.ended()
			LABELLED_PREDS, // must have gp_iter.ended() && !lp_iter.ended()
			DONE, // must have gp_iter.ended() && lp_iter.ended()
		};

	public:
		inline PredIterator(void) { }
		inline PredIterator(const Analysis::State& analysis_state)
			: state(GENERATED_PREDS), gp_iter(analysis_state.generated_preds), lp_iter(analysis_state.labelled_preds) { updateState(); }
		inline PredIterator(const PredIterator& source): gp_iter(source.gp_iter), lp_iter(source.lp_iter) { }
		inline PredIterator& operator=(const PredIterator& i)
			{ state = i.state; gp_iter = i.gp_iter; lp_iter = i.lp_iter; return *this; }
		
		inline bool ended(void) const { return (state == DONE); }
		const LabelledPredicate& item(void) const {
			switch(state) {
				case GENERATED_PREDS: return gp_iter.item();
				case LABELLED_PREDS: return lp_iter.item();
				default: assert(false);
			}
		}
		// this behaves fine when called while state == DONE. We use this in the code as of 2014-11-14 (for movePredicateToGenerated)
		void next(void) {
			if(state == GENERATED_PREDS) gp_iter++;
			if(state == LABELLED_PREDS) lp_iter++;
			updateState();
		}

		inline const Predicate& pred(void) const { return item().pred(); }
		inline const Path& labels(void) const { return item().labels(); }

		void updateState() {
			if(state == GENERATED_PREDS && !gp_iter) { nextState(); updateState(); }
			if(state == LABELLED_PREDS && !lp_iter) { nextState(); updateState(); }
		}

	private:
		void nextState() { if(state == GENERATED_PREDS) state = LABELLED_PREDS; else if (state == LABELLED_PREDS) state = DONE; }

		friend class Analysis::State;
		pred_iterator_state_t state;
		SLList<LabelledPredicate>::Iterator gp_iter; // Generated predicates (local) iterator 
		SLList<LabelledPredicate>::Iterator lp_iter; // Labelled predicates (previous BBs) iterator
	}; // PredIterator class
}; // State class

const static Analysis::State bottom(true);

class Analysis::States : public elm::Lock {
public:
	inline States() { }
	inline States(int cap) : s(cap) { }
	inline States(const Vector<Analysis::State>& state_vector) : s(state_vector) { }
	inline States(const States& ss) : s(ss.s) { }
	// return the unique state, or bottom if none. it is an error to call this when s.count() > 1
	inline State one() const { ASSERTP(s.count() <= 1, "multiple states available"); return s ? s.first() : bottom; }
	inline bool isEmpty() const { return s.isEmpty(); }
	inline int count() const { return s.count(); }
	inline const Vector<State>& states() const { return s; }
	inline Vector<State>& states() { return s; }
	inline void push(const Analysis::State& state) { return s.push(state); }

	inline void onCall(SynthBlock* sb)   { for(MutableIterator iter(this->s); iter; iter++) iter.item().onCall(sb); }
	inline void onReturn(SynthBlock* sb) { for(MutableIterator iter(this->s); iter; iter++) iter.item().onReturn(sb); }
	inline void onLoopEntry(Block* b)    { for(MutableIterator iter(this->s); iter; iter++) iter.item().onLoopEntry(b); }
	inline void onLoopExit (Block* b)    { for(MutableIterator iter(this->s); iter; iter++) iter.item().onLoopExit(b); }
	inline void onLoopExit (Edge* e)
		{ Block* h = LOOP_EXIT_EDGE(e); for(LoopHeaderIter i(e->source()); i.item() != h; i++) onLoopExit(*i); onLoopExit(h); }

	inline operator Vector<State>() { return s; }
	inline States& operator=(const Vector<State>& sv) { s = sv; return *this; }
	inline States& operator=(const States& ss) { s = ss.s; return *this; }
	inline friend io::Output& operator<<(io::Output& out, const States& ss) { return out << ss.s; }

	typedef Vector<State>::Iterator Iterator;
	typedef Vector<State>::MutableIterator MutableIterator;
private:
	Vector<State> s;
};

/**
 * @brief merge all states into one (a bit brutal)
 * 
 * @param cl Collection of States to process (accepts SLList, Vector etc.)
 */
template<template< class _ > class C> void Analysis::State::merge(const C<Analysis::State>& cl, Block* b)
{
	ASSERTP(!cl.isEmpty(), "call to Analysis::State::merge with empty cl parameter"); // maybe just leave the state empty
	DBGG("-\tmerging from " << cl.count() << " state(s).")
	// resetting stuff
	generated_preds.clear();
	generated_preds_taken.clear();
	labelled_preds.clear();
	SLList<ConstantVariables> cvl;
	// intialize to first element
	constants = cl.first().constants;
	// copy firstElement.labelled_preds into labelled_preds with empty labels
	for(SLList<LabelledPredicate>::Iterator iter(cl.first().labelled_preds); iter; iter++)
		labelled_preds += LabelledPredicate(iter->pred(), Path::null);
	bool first = true;
	for(typename C<State>::Iterator sl_iter(cl); sl_iter; sl_iter++)
	{
		if(first) // the first element is s itself, it's useless to merge s with s
		{
			first = false;
			continue;
		}
		// else //TODOv2
			cvl += (*sl_iter).constants; // constants.merge(...) uses the info from "constants" so it's useless to add it at the first iteration
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
	}
	this->constants.merge(cvl);
	// this->path.merge(stateListToPathVector(cl)); // merge paths as well while keeping some flow info and shrink that in this->path
	// this-path = DetailedPath(cl.first().lastEdge()->target()->toBasic());
	this->path.clear();
	// this->path.fromContext(cl.first().lastEdge()->target()->toBasic());
	this->path.fromContext(b);
}

template <class C> Vector<DetailedPath> Analysis::State::stateListToPathVector(const C& cl) const
{
	Vector<DetailedPath> rtn;
	for(typename C::Iterator iter(cl); iter; iter++)
		rtn.add(iter->getDetailedPath());
	return rtn;
}

/**
 * @brief Remove all bottom states from a Collection of States
 */
template<template< class _ > class C>
void Analysis::purgeBottomStates(C<Analysis::State>& sc) const
{
	for(typename C<Analysis::State>::MutableIterator i(sc); i; )
	{
		if(i.item().isBottom())
			sc.remove(i);
		else i++;
	}
}

#endif
