#ifndef _ANALYSIS_H
#define _ANALYSIS_H

#include <otawa/cfg/Edge.h>
#include <otawa/dfa/State.h> // dfa::State: isInitialized(addr), get(addr, _)...
#include <otawa/prop/Identifier.h>
#include <otawa/sem/inst.h>
#include <elm/avl/Set.h>
#include <elm/imm/list.h>
#include <elm/genstruct/SLList.h>
#include <elm/util/Comparator.h>
#include "constant_variables.h"
#include "labelled_predicate.h"
#include "debug.h"

using namespace otawa;
using namespace elm::genstruct;
using namespace elm::avl;
using namespace debug;

// pretty printing
void addIndents(io::Output& out, int n);
template <class C> io::Output& printCollection(io::Output& out, const C& items);
template <class T> inline io::Output& operator<<(io::Output& out, const SLList<T>& l) { return printCollection(out, l); }
template <class T> inline io::Output& operator<<(io::Output& out, const Vector<T>& v) { return printCollection(out, v); }

/*
class PathComparator: public elm::Comparator<SLList<const Edge*> > {
	static inline int compare(const SLList<const Edge*>& v1, const SLList<const Edge*>& v2)
		{ return 0; }
};
*/

class Analysis {
public:
	typedef SLList<Edge*> OrderedPath;
	typedef Set<Edge*> Path;
	class State;

	Analysis(CFG *cfg, const dfa::State *dfa_state, int sp, unsigned int max_tempvars, unsigned int max_registers, int flags);
	inline Vector<OrderedPath> infeasiblePaths() const { return infeasible_paths; }
	static bool listOfFixpoints(const SLList<Analysis::State>& sl);
	
	class State {
	private:
		const dfa::State* dfa_state;
		const OperandVar& sp; // the Stack Pointer register
		OrderedPath path;
		ConstantVariables constants; // remember in an array the variables that have been identified to a constant (e.g. t2 = 4)
		SLList<LabelledPredicate> labelled_preds; // previously generated predicates
		SLList<LabelledPredicate> generated_preds; // predicates local to the current BB
		SLList<LabelledPredicate> generated_preds_taken; // if there is a conditional, the taken preds will be saved here and the not taken preds will stay in generated_preds
			// that have been updated and need to have their labels list updated (add the next edge to the LabelledPreds struct)
		bool fixpoint;
		class PredIterator;

	public:
		State(); // create an invalid (bottom) state
		State(BasicBlock* entrybb, const dfa::State* state, const OperandVar& sp, unsigned int max_tempvars, unsigned int max_registers, bool init = true);
		State(Edge* entry_edge, const dfa::State* state, const OperandVar& sp, unsigned int max_tempvars, unsigned int max_registers, bool init = false);
		State(const State& s);
		inline const OrderedPath& getPath() const { return path; }
		inline Edge* lastEdge() const { return path.last(); }
		inline const SLList<LabelledPredicate>& getLabelledPreds() const { return labelled_preds; }
		inline const ConstantVariables& getConstants() const { return constants; }
		inline bool isValid() const { return dfa_state != 0; }
		inline bool fixpointState() const { return fixpoint; }
		inline void setFixpointState(bool new_fixpoint) { fixpoint = new_fixpoint; }
		inline void dumpPredicates() const { for(PredIterator iter(*this); iter; iter++) DBG(*iter); }
		friend io::Output& operator<<(io::Output& out, const State& s) { return s.print(out); }

		// analysis.cpp
		elm::String getPathString() const;
		elm::String dumpEverything() const;
		void merge(const SLList<State>& sl);
		void merge(const SLList<State>& sl, Edge* e);
		bool isFixPoint(const Analysis::State& s) const;

		// analysis_cfg.cpp
		void appendEdge(Edge* e, bool is_conditional);

		// analysis_bb.cpp
		void processBB(const BasicBlock *bb);
		void throwInfo();

		inline const State* operator->(void) const { return this; }

	private:
		// Private methods
		// analysis.cpp
		void setPredicate(PredIterator &iter, const LabelledPredicate &labelled_predicate);
		void movePredicateToGenerated(PredIterator &iter);
		void removePredicate(PredIterator &iter);
		SLList<LabelledPredicate> labelPredicateList (const SLList<LabelledPredicate>& pred_list, Edge* label);
		io::Output& print(io::Output& out) const;

		// analysis_bb.cpp
		bool invalidateVar(const OperandVar& var, bool invalidate_constant_info = true);
		bool invalidateMem(const OperandMem& addr);
		bool invalidateMem(const OperandVar& var);
		bool invalidateTempVars();
		bool replaceVar(const OperandVar& var, const Operand& expr);
		bool replaceMem(const OperandMem& opd, const Operand& expr, const Path& labels);
		bool replaceTempVar(const OperandVar& temp_var, const Operand& expr);
		bool replaceTempVar(const OperandMem& mem, const Operand& expr);
		bool update(const OperandVar& opd_to_update, const Operand& opd_modifier);
		Option<Constant> findConstantValueOfVar(const OperandVar& var); // changed to a simple lookup to "constants"
		Option<Constant> findConstantValueOfMemCell(const OperandMem& mem, Path &labels);
		Option<t::int32> findStackRelativeValueOfVar(const OperandVar& var, Path& labels);
		bool findValueOfCompVar(const OperandVar& var, Operand*& opd_left, Operand*& opd_right);
		Option<OperandMem> getOperandMem(const OperandVar& var, Path& labels);
		bool invalidateAllMemory();
		Predicate* getPredicateGeneratedByCondition(sem::inst condition, bool taken);
		Option<OperandConst> getConstantDataValue(const OperandMem& addr_mem, otawa::sem::type_t type);
		inline bool isConstant(const OperandVar& var) const { return constants.isConstant(var); }

		// PredIterator class
		class PredIterator: public PreIterator<PredIterator, LabelledPredicate> {
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
			// this behaves fine when called while state == DONE. We use this in the code as of nov.14 (for movePredicateToGenerated)
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

	// bool invalidate_constant_info 
	enum
	{
		KEEP_CONSTANT_INFO = false,
		INVALIDATE_CONSTANT_INFO = true,
	};

	// flags 
	enum
	{
		FOLLOW_CALLS	= 0b1 << 1,
		// SUPERSILENT		= 0b1 << 2,
		MERGE			= 0b1 << 3,
	};
	
private:
	// TODO: try using a Set or something more appropriate (we check if(contains) everytime we add an element...)
	Vector<BasicBlock*> wl; // working list

	const dfa::State* dfa_state;
	const OperandVar sp; // Stack Pointer
	Vector<OrderedPath> infeasible_paths; // TODO: Set<Path, PathComparator<Path> > path; to make Set useful
	int max_tempvars, max_registers, flags;
	int total_paths, loop_header_count, bb_count;
	int ip_count, unminimized_ip_count;
	
	// analysis_cfg.cpp
	void processCFG(CFG *cfg);
	int processBB(State& s, BasicBlock *bb);
	void processOutEdge(Edge* e, const Identifier<SLList<Analysis::State> >& processed_edges_id, const SLList<Analysis::State>& sl, bool is_conditional, bool enable_smt);
	void processLoopHeader(BasicBlock* bb, SLList<Analysis::State>& sl, const Identifier<Analysis::State*>& processed_loopheader_bb_id, const Identifier<bool>& motherloop_fixpoint_state_id, const Identifier<bool>& fixpoint_reached_id);
	bool checkInfeasiblePathValidity(const SLList<Analysis::State>& sl, const SLList<Option<Path> >& sl_paths, const Edge* e, const Path& infeasible_path, elm::String& counterexample) const;
	void addDisorderedPath(const Path& infeasible_path, const OrderedPath& full_path, Edge* last_edge);
	void purgeStateList(SLList<Analysis::State>& sl) const;
	void mergeOversizedStateList(SLList<Analysis::State>& sl, int thresold) const;
	void placeboProcessCFG(CFG* cfg);
	void placeboProcessBB(BasicBlock *bb);
	void printResults(int exec_time_ms) const;
	void removeDuplicateInfeasiblePaths();
	void onPathEnd();
	void onAnyInfeasiblePath();
	bool isAHandledEdgeKind(Edge::kind_t kind) const;
	bool isConditional(BasicBlock* bb) const;
	void cleanIncomingEdges(BasicBlock* bb, const Identifier<SLList<Analysis::State> >& processed_edges_id) const;
	void cleanIncomingBackEdges(BasicBlock* bb, const Identifier<SLList<Analysis::State> >& processed_edges_id) const;
	bool fixpointFoundOnAllMotherLoops(BasicBlock *bb, const Identifier<bool>& fixpoint_found_id) const;
	bool allIncomingNonBackEdgesAreAnnotated(BasicBlock* bb, const Identifier<SLList<Analysis::State> >& annotation_identifier) const;
	bool allIncomingEdgesAreAnnotated(BasicBlock* bb, const Identifier<SLList<Analysis::State> >& annotation_identifier) const;
	bool isSubPath(const OrderedPath& included_path, const Edge* e, const Path& path_set) const;
	elm::String wlToString() const;
	elm::String pathToString(const Path& path);
}; // Analysis class

template <class C> io::Output& printCollection(io::Output& out, const C& items)
{
	int count = items.count();
	if(count > 10)
	{
		out << "#" << count << "#";
		return out; // just exit if too many items
	}
	
	static int indent = 0;
	bool indented_output = (count > 5);
	
	if(indented_output)
		addIndents(out, indent++);
	out << color::Bold() << "[" << color::NoBold();
	if(indented_output)
		out << io::endl;
	bool first = true;
	for(typename C::Iterator elems(items); elems; elems++)
	{
		if(first)
		{
			if(indented_output)
				addIndents(out, indent);
			out << *elems;
			first = false;
		}
		else
		{
			out << ", ";
			if(indented_output)
			{
				out << io::endl;
				addIndents(out, indent);
			}
			out << *elems;
		}
	}
	if(indented_output)
	{
		out << io::endl;
		addIndents(out, --indent);
	}
	
	return (out << color::Bold() << "]" << color::NoBold());
}

#endif
