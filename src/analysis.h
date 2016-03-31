#ifndef _ANALYSIS_H
#define _ANALYSIS_H

#include <elm/avl/Set.h>
#include <elm/genstruct/SLList.h>
#include <elm/compare.h>
#include <otawa/cfg/Edge.h>
#include <otawa/cfg/features.h>
#include <otawa/dfa/State.h> // dfa::State: isInitialized(addr), get(addr, _)...
#include <otawa/prop/Identifier.h>
#include "constant_variables.h"
#include "labelled_predicate.h"
#include "detailed_path.h"
#include "debug.h"

using namespace otawa;
using namespace elm::genstruct;
using namespace elm::avl;

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

	Analysis(CFG *cfg, const dfa::State *dfa_state, int sp, unsigned int max_tempvars, unsigned int max_registers, int state_size_limit, int flags);
	inline Vector<DetailedPath> infeasiblePaths() const { return infeasible_paths; }
	// static bool listOfFixpoints(const SLList<Analysis::State>& sl);
	static elm::String pathToString(const Path& path);
	static elm::String orderedPathToString(const OrderedPath& path);

	class State {
	private:
		const dfa::State* dfa_state;
		const OperandVar sp; // the Stack Pointer register
		// OrderedPath path;
		DetailedPath path;
		ConstantVariables constants; // remember in an array the variables that have been identified to a constant (e.g. t2 = 4)
		SLList<LabelledPredicate> labelled_preds; // previously generated predicates
		SLList<LabelledPredicate> generated_preds; // predicates local to the current BB
		SLList<LabelledPredicate> generated_preds_taken; // if there is a conditional, the taken preds will be saved here and the not taken preds will stay in generated_preds
			// that have been updated and need to have their labels list updated (add the next edge to the LabelledPreds struct)
		bool fixpoint;
		class PredIterator;

	public:
		State(); // create an invalid (bottom) state
		State(Block* entryb, const dfa::State* state, const OperandVar& sp, unsigned int max_tempvars, unsigned int max_registers, bool init = true);
		State(Edge* entry_edge, const dfa::State* state, const OperandVar& sp, unsigned int max_tempvars, unsigned int max_registers, bool init = false);
		State(const State& s);
		// inline const OrderedPath& getPath() const { return path; }
		inline const DetailedPath& getDetailedPath() const { return path; }
		inline Edge* lastEdge() const { return path.lastEdge(); }
		inline const SLList<LabelledPredicate>& getLabelledPreds() const { return labelled_preds; }
		inline const ConstantVariables& getConstants() const { return constants; }
		inline elm::String getPathString() const { return path.toString(); /*orderedPathToString(path.toOrderedPath());*/ }
		inline void onLoopEntry(Block* loop_header) { path.onLoopEntry(loop_header); }
		inline void onLoopExit(Option<Block*> maybe_loop_header = elm::none) { path.onLoopExit(maybe_loop_header); }
		inline void onCall(Edge* e) { path.onCall(e); }
		inline void onReturn(Block* b) { path.onReturn(b); }
		inline bool isValid() const { return dfa_state != 0; }

		// inline bool inALoop() const { return ; }
		// inline bool fixpointState() const { return fixpoint; }
		// inline void setFixpointState(bool new_fixpoint) { fixpoint = new_fixpoint; }

		inline void dumpPredicates() const { for(PredIterator iter(*this); iter; iter++) DBG(*iter); }
		friend io::Output& operator<<(io::Output& out, const State& s) { return s.print(out); }

		// analysis.cpp
		Vector<DetailedPath> stateListToPathVector(const SLList<State>& sl) const;
		elm::String dumpEverything() const;
		void merge(const SLList<State>& sl);
		// void merge(const SLList<State>& sl, Edge* e);
		bool isFixPoint(const Analysis::State& s) const;

		// analysis_cfg.cpp
		void appendEdge(Edge* e, bool is_conditional);
		void printFixPointState() const;

		// analysis_bb.cpp
		void processBB(const BasicBlock *bb);
		void throwInfo();
		int invalidateStackBelow(const Constant& stack_limit);


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
		inline Set<Edge*> getLabels(const OperandVar& opdv) const { return constants.getLabels(opdv); }
		inline Set<Edge*> getLabels(const OperandVar& opdv1, const OperandVar& opdv2) const { return constants.getLabels(opdv1, opdv2); }

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

	// bool invalidate_constant_info 
	enum
	{
		KEEP_CONSTANT_INFO = false,
		INVALIDATE_CONSTANT_INFO = true,
	};

	// flags 
	enum
	{
		FOLLOW_CALLS		= 0b1 << 1,
		//SUPERSILENT		= 0b1 << 2,
		MERGE				= 0b1 << 3,
		UNMINIMIZED_PATHS	= 0b1 << 4,
		DRY_RUN				= 0b1 << 5,
	};
	
	// Fixpoint status of the loop header, for annotation
	typedef enum 
	{	// ENTER represented by no annotation, when we haven't entered the loop yet
		FIX,
		LEAVE,
	} loopheader_status_t;

private:
	// static Identifier<bool>					MOTHERLOOP_FIXPOINT_STATE;
	// static Identifier<bool>					FIXPOINT_REACHED;
	static Identifier<SLList<Analysis::State> >	EDGE_S; // Trace on an edge
	static Identifier<Analysis::State>			LH_S; // Trace on a loop header
	static Identifier<loopheader_status_t>		LH_STATUS; // Fixpt status of a loop header

	// TODO: try using a Set or something more appropriate (we check if(contains) everytime we add an element...)
	Vector<Block*> wl; // working list

	const dfa::State* dfa_state;
	const OperandVar sp; // Stack Pointer
	Vector<DetailedPath> infeasible_paths; // TODO: Set<Path, PathComparator<Path> > path; to make Set useful
	int max_tempvars, max_registers, state_size_limit, flags;
	int total_paths, loop_header_count, bb_count;
	int ip_count, unminimized_ip_count;

	// analysis.cpp
	void debugProgress(int block_id, bool enable_smt) const;
	inline Analysis::State topState(Block* entry) const { return Analysis::State(entry, dfa_state, sp, max_tempvars, max_registers); }
	inline static bool loopStatusIsEnter(Block* h) { ASSERT(LOOP_HEADER(h)); return LH_STATUS.get(h) == elm::none; }
	inline static bool loopStatusIsFix	(Block* h) { ASSERT(LOOP_HEADER(h)); return LH_STATUS.get(h) == elm::some(FIX); }
	inline static bool loopStatusIsLeave(Block* h) { ASSERT(LOOP_HEADER(h)); return LH_STATUS.get(h) == elm::some(LEAVE); }
	static Vector<Edge*> allIns    (Block* h);
	static Vector<Edge*> backIns   (Block* h);
	static Vector<Edge*> nonBackIns(Block* h);
	
	// analysis_cfg.cpp
	void processCFG(CFG *cfg);
	int processBlock(State& s, Block* b);
	int processBB(State& s, BasicBlock *bb);
	void processOutEdge(Edge* e, const SLList<Analysis::State>& sl, bool is_conditional, bool enable_smt);
	void processLoopHeader(Block* b, SLList<Analysis::State>& sl);
	void stateListToInfeasiblePathList(SLList<Option<Path> >& sl_paths, const SLList<Analysis::State>& sl, Edge* e, bool is_conditional);
	bool checkInfeasiblePathValidity(const SLList<Analysis::State>& sl, const SLList<Option<Path> >& sl_paths, const Edge* e, const Path& infeasible_path, elm::String& counterexample) const;
	void addDisorderedInfeasiblePath(const Path& infeasible_path, const DetailedPath& full_path, Edge* last_edge);
	void addDetailedInfeasiblePath(const DetailedPath& infeasible_path);
	void purgeStateList(SLList<Analysis::State>& sl) const;
	bool mergeOversizedStateList(SLList<Analysis::State>& sl) const;
	void placeboProcessCFG(/*CFG* cfg*/) const;
	void printResults(int exec_time_ms) const;
	void printCurrentlyProcessingBlock(Block* b, int progression_percentage, bool loop_header) const;
	void removeDuplicateInfeasiblePaths();
	void onPathEnd();
	void onAnyInfeasiblePath();
	// bool isAHandledEdgeKind(Edge::kind_t kind) const;
	Option<Constant> getCurrentStackPointer(const SLList<Analysis::State>& sl) const;
	bool isConditional(Block* b) const;
	// void cleanIncomingEdges(Block* b) const;
	// void cleanIncomingBackEdges(Block* b) const;
	// bool fixpointFoundOnAllMotherLoops(Block* b) const;
	// bool edgeIsExitingToLoopLevel0(const Edge* e) const;
	// bool shouldEnableSolver(const Edge* e);
	// bool allRequiredInEdgesAreProcessed(Block* block) const;
	// bool allIncomingNonBackEdgesAreAnnotated(Block* block, const Identifier<SLList<Analysis::State> >& annotation_identifier) const;
	// bool allIncomingEdgesAreAnnotated(Block* block, const Identifier<SLList<Analysis::State> >& annotation_identifier) const;
	bool isSubPath(const OrderedPath& included_path, const Edge* e, const Path& path_set) const;
	elm::String wlToString() const;

	bool anyEdgeHasTrace(const Vector<Edge*>& edges) const;
	bool anyEdgeHasTrace(Block::EdgeIter& biter) const;
	bool allEdgesHaveTrace(const Vector<Edge*>& edges) const;
	bool allEdgesHaveTrace(Block::EdgeIter& biter) const;
}; // Analysis class

template <class C> io::Output& printCollection(io::Output& out, const C& items)
{
	int count = items.count();
	if(count > 25)
	{
		out << "#" << count << "#";
		return out; // just exit if too many items
	}
	
	static int indent = 0;
	bool indented_output = (count > 5) && false; // TODO!
	
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
