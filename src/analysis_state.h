#ifndef _ANALYSIS_STATE_H
#define _ANALYSIS_STATE_H

#include <elm/avl/Set.h>
#include <elm/genstruct/HashTable.h>
#include <elm/genstruct/SLList.h>
#include <otawa/cfg/Edge.h>
#include <otawa/cfg/features.h>
#include <otawa/dfa/State.h> // dfa::State: isInitialized(addr), get(addr, _)...
#include <otawa/sem/PathIter.h>
#include "analysis.h"
#include "constant_variables.h"
#include "detailed_path.h"
// #include "halfpredicate.h"
#include "labelled_predicate.h"
#include "local_variables.h"

using namespace otawa;
using elm::genstruct::SLList;
using elm::genstruct::Vector;
using elm::genstruct::HashTable;

class Analysis::State {
private:
	typedef HashTable<Constant, const Operand*, ConstantHash> mem_t;
	// HashTable<t::int32, const Operand*> lvars;

	const dfa::State* dfa_state;
	OperandVar sp; // the Stack Pointer register
	DAG* dag;
#ifdef EXP
	LocalVariables lvars;
	// LocalVariables lvars_taken;
	mem_t mem;
#endif
	bool bottom; // true=is Bottom, false= is Top
	DetailedPath path;
	ConstantVariables constants; // remember in an array the variables that have been identified to a constant (e.g. t2 = 4)
	SLList<LabelledPredicate> labelled_preds; // previously generated predicates
	SLList<LabelledPredicate> generated_preds; // predicates local to the current BB
	SLList<LabelledPredicate> generated_preds_taken; // if there is a conditional, the taken preds will be saved here and the not taken preds will stay in generated_preds
		// that have been updated and need to have their labels list updated (add the next edge to the LabelledPreds struct)
	class PredIterator;

public:
	explicit State(bool bottom = false); // false: create an invalid state, true: create a bottom state
	State(Block* entryb, const context_t& context, bool init = true);
	State(Edge* entry_edge, const context_t& context, bool init = false);
	State(const State& s);
	inline const DetailedPath& getDetailedPath() const { return path; }
	inline Edge* lastEdge() const { return path.lastEdge(); }
	inline const SLList<LabelledPredicate>& getLabelledPreds() const { return labelled_preds; }
	inline const ConstantVariables& getConstants() const { return constants; }
#ifdef EXP
	inline const LocalVariables& getLocalVariables() const { return lvars; }
	inline const mem_t& getMemoryTable() const { return mem; }
#endif
	inline elm::String getPathString() const { return path.toString(); /*orderedPathToString(path.toOrderedPath());*/ }
	inline void onLoopEntry(Block* loop_header) { path.onLoopEntry(loop_header); }
	inline void onLoopExit(Option<Block*> maybe_loop_header = elm::none) { path.onLoopExit(maybe_loop_header); }
	inline void onCall(SynthBlock* sb)   { path.onCall(sb); }
	inline void onReturn(SynthBlock* sb) { path.onReturn(sb); }
	inline bool isBottom() const { return bottom; }
	inline bool isValid() const { return dfa_state != NULL && constants.isValid(); } // this is so that we can have empty states that do not use too much memory
	inline DAG& getDag() const { return *dag; }

	// analysis_state.cpp
	template <class C> Vector<DetailedPath> stateListToPathVector(const C& sl) const;
	elm::String dumpEverything() const;
	template<template< class _ > class C> void merge(const C<Analysis::State>& cl, Block* b);
	bool equiv(const State& s) const;
	void appendEdge(Edge* e);
	void removeConstantPredicates();

	// analysis_bb.cpp
	void processBB(const BasicBlock *bb, int version_flags = Analysis::WITH_V1 | Analysis::WITH_V2);
	void processSemInst1(const otawa::sem::PathIter& seminsts, sem::inst& last_condition);
	void processSemInst2(const otawa::sem::PathIter& seminsts, sem::inst& last_condition);
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

	// analysis_bb.cpp (v2)
	void set(const OperandVar& var, const Operand* expr, bool set_updated = true);
	void setMem(Constant addr, const Operand* expr);
	inline const Operand* getPtr(t::int32 var_id) const;
	inline void scratch(const OperandVar& var) { set(var, dag->new_top(), false); lvars.clearLabels(var); }
	void scratchAllMemory();
	// smart functions
	const Operand* smart_add(const Operand* a, const Operand* b);
	const Operand* smart_add(Constant x, const Operand* y);
	const Operand* smart_sub(const Operand* a, const Operand* b);
	const Operand* smart_sub(Constant x, const Operand* y);
	const Operand* smart_mul(const Operand* a, const Operand* b);
	const Operand* smart_mul(const Operand* a, Constant c);
	const Operand* smart_div(const Operand* a, const Operand* b);
	const Operand* smart_div(const Operand* a, Constant c);
	const Operand* smart_divmul(const Operand* x, Constant k, Constant c);
	const Operand* smart_muldiv(const Operand* x, Constant k, Constant c);

	LabelledPredicate makeLabelledPredicate(condoperator_t opr, const Operand* opd1, const Operand* opd2, Path& labels) const;
	bool tryToKeepVar(const OperandVar& var);//, const Predicate*& removed_predicate);
	bool invalidateVar(const OperandVar& var, bool invalidate_constant_info = true);
	// bool invalidate(const Operand& var, bool invalidate_constant_info = true);
	bool invalidateMem(const OperandMem& addr);
	bool invalidateMem(const OperandVar& var);
	bool invalidateTempVars();
	// bool replaceOperand(const Operand& opd, const Operand& expr);
	bool replaceVar(const OperandVar& var, const Operand* expr);
	bool replaceMem(const OperandMem& opd, const Operand* expr, const Path& labels);
	bool replaceTempVar(const OperandVar& temp_var, const Operand* expr);
	bool replaceTempVar(const OperandMem& mem, const Operand* expr);
	bool update(const OperandVar& opd_to_update, const Operand* opd_modifier, Path& labels);
	Option<OperandConst> findConstantValueOfVar(const OperandVar& var); // changed to a simple lookup to "constants"
	Option<Constant> findConstantValueOfMemCell(const OperandMem& mem, Path &labels);
	Option<t::int32> findStackRelativeValueOfVar(const OperandVar& var, Path& labels);
	bool findValueOfCompVar(const OperandVar& var, Operand const*& opd_left, Operand const*& opd_right, Path& labels);
	Option<OperandMem> getOperandMem(const OperandVar& var, Path& labels);
	bool invalidateAllMemory();
	void updateLabelsWithReplacedConstantsInfo(Path& labels, const Vector<OperandVar>& replaced_vars) const;
	Option<Predicate> getPredicateGeneratedByCondition(sem::inst condition, bool taken, Path& labels);
	Option<Constant> getConstantValueOfReadOnlyMemCell(const OperandMem& addr_mem, otawa::sem::type_t type);
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
		inline const LabelledPredicate& item(void) const {
			return (state == GENERATED_PREDS) ? gp_iter.item() : lp_iter.item();
			// switch(state) {
			// 	case GENERATED_PREDS: return gp_iter.item();
			// 	case LABELLED_PREDS: return lp_iter.item();
			// 	default: ASSERT(false);
			// }
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

extern const Analysis::State bottom;

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
template<template< class _ > class C> void Analysis::State::merge(const C<Analysis::State>& sc, Block* b)
{
	ASSERTP(!sc.isEmpty(), "call to Analysis::State::merge with empty sc parameter"); // maybe just leave the state empty
	DBGG("-\tmerging from " << sc.count() << " state(s).")
	// resetting stuff
	generated_preds.clear();
	generated_preds_taken.clear();
	labelled_preds.clear();
	SLList<ConstantVariables> cvl;
#ifdef EXP0
	// const LocalVariables* lvtab[sc.count()];
	LocalVariables lvars1(sc.first().lvars);
	mem_t mem1(sc.first().mem);
	// const mem_t* mtab[sc.count()];
	// int i = 0;
#endif
	// intialize to first element
	constants = sc.first().constants;
	// copy firstElement.labelled_preds into labelled_preds with empty labels
	for(SLList<LabelledPredicate>::Iterator iter(sc.first().labelled_preds); iter; iter++)
		labelled_preds += LabelledPredicate(iter->pred(), Path::null);
	bool first = true;
	for(typename C<State>::Iterator sc_iter(sc); sc_iter; sc_iter++)
	{
		if(first) // the first element is s itself, it's useless to merge s with s
		{
			first = false;
			continue;
		}
		// else //TODOv2
			cvl += (*sc_iter).constants; // constants.merge(...) uses the info from "constants" so it's useless to add it at the first iteration
#ifdef EXP0
		lvars.merge(sc_iter->lvars);
		for(mem_t::PairIterator iter(mem1); iter; iter++)		
			if((*iter).snd != sc_iter->mem[(*iter).fst])
				mem1[(*iter).fst] = Top;
		for(mem_t::PairIterator iter(sc_iter->mem); iter; iter++)		
			if((*iter).snd != mem1[(*iter).fst])
				mem1[(*iter).fst] = Top;
		// lvtab[i] = &(sc_iter->lvars);
		// mtab[i++] = &(sc_iter->mem);

#endif
		// for each element of labelled_preds, we make sure it is in *sc_iter
		for(SLList<LabelledPredicate>::Iterator iter(labelled_preds); iter; )
		{
			// do not use 'if(s.labelled_preds.contains(*iter))' as we want to use Predicate::operator== and not LabelledPredicate::operator==
			bool contains = false;
			for(SLList<LabelledPredicate>::Iterator subiter((*sc_iter).labelled_preds); subiter; subiter++)
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
	// this->path.merge(stateListToPathVector(sc)); // merge paths as well while keeping some flow info and shrink that in this->path
	// this-path = DetailedPath(sc.first().lastEdge()->target()->toBasic());
	this->path.clear();
	// this->path.fromContext(sc.first().lastEdge()->target()->toBasic());
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
