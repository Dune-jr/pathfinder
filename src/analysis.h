#ifndef _ANALYSIS_H
#define _ANALYSIS_H

#include <elm/genstruct/SLList.h>
#include <otawa/cfg/Edge.h>
#include <otawa/cfg/features.h>
#include <otawa/dfa/State.h>
#include <otawa/prop/Identifier.h>
#include "detailed_path.h"
#include "operand.h"
#include "pretty_printing.h"

using namespace otawa;
using elm::genstruct::SLList;

class Analysis {
public:
	typedef SLList<Edge*> OrderedPath;
	typedef elm::avl::Set<Edge*> Path;
	class State;

	typedef struct
	{
		const dfa::State* dfa_state;
		OperandVar sp; // Stack Pointer
		unsigned int max_tempvars;
		unsigned int max_registers;
	} context_t;
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
	{	
		ENTER, // not used: represented by no annotation, when we haven't entered the loop yet
		FIX,
		LEAVE,
	} loopheader_status_t;

	Analysis(const context_t& context, int state_size_limit, int flags);
	const Vector<DetailedPath>& run(CFG *cfg);
	inline const Vector<DetailedPath>& infeasiblePaths() const { return infeasible_paths; }
	// static bool listOfFixpoints(const SLList<Analysis::State>& sl);
	static elm::String pathToString(const Path& path);
	static elm::String orderedPathToString(const OrderedPath& path);

protected:
	context_t context;
	int state_size_limit, flags;

	inline static loopheader_status_t loopStatus(Block* h) { ASSERT(LOOP_HEADER(h)); return LH_STATUS.get(h,ENTER); }
	static Block* insAlias		   (Block* b);
	static Vector<Edge*> ins 	   (Block* b);
	static Vector<Edge*> allIns    (Block* h);
	static Vector<Edge*> backIns   (Block* h);
	static Vector<Edge*> nonBackIns(Block* h);
	static Vector<Edge*> outsWithoutUnallowedExits(Block* b);
	static bool isAllowedExit(Edge* exit_edge);
	static Option<Block*> getCaller(CFG* cfg);
	static Block* getCaller(Block* exit);
	static elm::String printFixPointStatus(Block* b);

	static Identifier<Vector<Analysis::State> >	EDGE_S; // Trace on an edge
private:
	static Identifier<Analysis::State>			LH_S; // Trace on a loop header
	static Identifier<loopheader_status_t>		LH_STATUS; // Fixpt status of a loop header
	// static Identifier<bool>					MOTHERLOOP_FIXPOINT_STATE;
	// static Identifier<bool>					FIXPOINT_REACHED;

	Vector<Block*> wl; // working list
	Vector<DetailedPath> infeasible_paths; // TODO: Set<Path, PathComparator<Path> > path; to make Set useful
	int total_paths, loop_header_count, bb_count;
	int ip_count, unminimized_ip_count;

	// virtual pure functions to implement
	virtual Vector<State> narrowing(const Vector<Edge*>& edges) const = 0;
	virtual bool inD_ip(const otawa::Edge* e) const = 0;
	virtual void ipcheck(const elm::genstruct::Vector<Analysis::State>& s, elm::genstruct::Vector<DetailedPath>& infeasible_paths) const = 0;

	// analysis.cpp
	void debugProgress(int block_id, bool enable_smt) const;
	Analysis::State topState(Block* entry) const;
	void wl_push(Block* b);
	
	// analysis_cfg.cpp
	void processCFG(CFG *cfg);
	Vector<State>& I(Block* b, Vector<State>& s);
	Vector<State>& I(Edge* e, Vector<State>& s);
	// int processBB(BasicBlock *bb, State& s);
	// void processOutEdge(Edge* e, const SLList<Analysis::State>& sl, bool is_conditional, bool enable_smt);
	// void processLoopHeader(Block* b, SLList<Analysis::State>& sl);
	// void stateListToInfeasiblePathList(SLList<Option<Path> >& sl_paths, const SLList<Analysis::State>& sl, Edge* e, bool is_conditional);
	bool checkInfeasiblePathValidity(const SLList<Analysis::State>& sl, const SLList<Option<Path> >& sl_paths, const Edge* e, const Path& infeasible_path, elm::String& counterexample) const;
	void addDisorderedInfeasiblePath(const Path& infeasible_path, const DetailedPath& full_path, Edge* last_edge);
	void addDetailedInfeasiblePath(const DetailedPath& infeasible_path);
	void purgeStateList(SLList<Analysis::State>& sl) const;
	// bool mergeOversizedStateList(SLList<Analysis::State>& sl) const;
	// void placeboProcessCFG(/*CFG* cfg*/) const;
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
	bool anyEdgeHasTrace(const Block::EdgeIter& biter) const;
	bool allEdgesHaveTrace(const Vector<Edge*>& edges) const;
	bool allEdgesHaveTrace(const Block::EdgeIter& biter) const;
}; // Analysis class

#endif
