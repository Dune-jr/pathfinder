#ifndef _INFEASIBLE_PATH_H
#define _INFEASIBLE_PATH_H

// #include <elm/genstruct/SLList.h> 
#include <elm/genstruct/Vector.h>
#include <elm/string/String.h>
#include <otawa/cfg/CFG.h>
#include <otawa/cfg/features.h>
#include <otawa/cfg/Edge.h>
#include <elm/util/Option.h>
#include "cfg_features.h"
#include "debug.h"

using elm::genstruct::SLList;
// using elm::genstruct::Vector;
using otawa::Edge;
using otawa::Block;
using otawa::BasicBlock;
using otawa::SynthBlock;

class DetailedPath
{
public:
	class FlowInfo;
	class Iterator;

	DetailedPath(CFG* f = NULL);
	DetailedPath(BasicBlock* bb); // initializes with LEn and CALL from context of b
	// DetailedPath(const SLList<Edge*>& edge_list);
	DetailedPath(const DetailedPath& dp);
	
	// SLList methods
	inline void clear() { _path.clear(); }
	void addBefore (const Iterator &pos, const FlowInfo &value);
	void addLast(Edge* e);
	inline void addLast(const FlowInfo& fi) { _path.addLast(fi); }
	inline bool contains(const FlowInfo &fi) const { return _path.contains(fi); }
	inline SLList<FlowInfo>::Iterator find(const FlowInfo &fi) const { return _path.find(fi); }
	inline void remove(Edge* e) { _path.remove(FlowInfo(e)); }
	inline void remove(Iterator &iter) { _path.remove(iter); }
	// inline void remove(Iterator &iter) { _path.remove(iter.getFlowInfoIter()); }
	inline void removeLast() { _path.removeLast(); }
	
	// events
	// void onLoopEntry(Block* loop_header);
	void onLoopExit(Option<Block*> new_loop_header);
	void onCall(otawa::SynthBlock* sb);
	void onReturn(otawa::SynthBlock* sb);

	// utility
	bool weakEqualsTo(const DetailedPath& dp) const;
	void fromContext(Block* b);
	// void addEnclosingLoop(Block* loop_header);
	void merge(const Vector<DetailedPath>& detailed_paths);
	void apply(const DetailedPath& path);
	void optimize();
	bool hasAnEdge() const;
	Edge* firstEdge() const;
	Edge* lastEdge() const;
	Option<Block*> lastBlock() const;
	int countEdges() const;
	SLList<Edge*> toOrderedPath() const;
	elm::String toString(bool colored = true) const;
	inline const SLList<FlowInfo>& path() const { return _path; }
	inline CFG* function() const { return fun; }
	inline bool operator==(const DetailedPath& dp) const { return _path == dp._path && fun == dp.fun; }
	inline const DetailedPath* operator->(void) const { return this; }
	friend io::Output& operator<<(io::Output& out, const DetailedPath& dp) { ASSERT(dp.fun); return dp.print(out); }

	// FlowInfo class
	class FlowInfo
	{
	public:
		enum kind_t
		{
			KIND_EDGE, // Edge
			KIND_LOOP_ENTRY, // BasicBlock
			KIND_LOOP_EXIT, // BasicBlock
			KIND_CALL, // SynthBlock
			KIND_RETURN, // SynthBlock
		};
		FlowInfo() : _kind(kind_t(-1)), _identifier(NULL) { }
		FlowInfo(kind_t kind, BasicBlock* bb) : _kind(kind), _identifier(bb) { ASSERT(isBasicBlockKind()); }
		FlowInfo(kind_t kind, SynthBlock* sb) : _kind(kind), _identifier(sb) { ASSERT(isSynthBlockKind()); }
		FlowInfo(kind_t kind, Edge* e) : _kind(kind), _identifier(e) { ASSERT(isEdgeKind()); }
		FlowInfo(Edge* e) : _kind(KIND_EDGE), _identifier(e) { }
		FlowInfo(const FlowInfo& fi) { _kind = fi._kind; _identifier = fi._identifier; }
		inline kind_t kind() const { return _kind; }
		inline bool isEdge() const { return _kind == KIND_EDGE; }
		inline bool isLoopEntry() const { return _kind == KIND_LOOP_ENTRY; }
		inline bool isLoopExit() const { return _kind == KIND_LOOP_EXIT; }
		inline bool isCall() const { return _kind == KIND_CALL; }
		inline bool isReturn() const { return _kind == KIND_RETURN; }
		inline Edge* getEdge() const { ASSERT(isEdgeKind()); return (Edge*)_identifier; }
		inline BasicBlock* getBasicBlock() const { ASSERT(isBasicBlockKind()); return (BasicBlock*)_identifier; }
		inline SynthBlock* getSynthBlock() const { ASSERT(isSynthBlockKind()); return (SynthBlock*)_identifier; }
		inline BasicBlock* getLoopHeader() const { return getBasicBlock(); }
		inline SynthBlock* getCaller() const { return getSynthBlock(); }
		elm::String toString(bool colored = true) const;
		inline bool operator==(const FlowInfo& fi) const { return (_kind == fi._kind) && (_identifier == fi._identifier); }
		inline bool operator!=(const FlowInfo& fi) const { return !this->operator==(fi); }
		inline FlowInfo& operator=(const FlowInfo& fi) { _kind = fi._kind; _identifier = fi._identifier; return *this; }
		inline const FlowInfo* operator->(void) const { return this; }
		friend io::Output& operator<<(io::Output& out, const FlowInfo& fi) { return fi.print(out); }

		inline bool isBasicBlockKind(void) const
			{ return (_kind == KIND_LOOP_ENTRY) || (_kind == KIND_LOOP_EXIT); }
		inline bool isSynthBlockKind(void) const
			{ return (_kind == KIND_CALL) || (_kind == KIND_RETURN); }
		inline bool isEdgeKind(void) const
			{ return (_kind == KIND_EDGE); }
	private:
		io::Output& print(io::Output& out) const;

		kind_t _kind;
		void* _identifier; // BasicBlock*, Edge*
	}; // FlowInfo class


	// EdgeIterator class
	class EdgeIterator: public PreIterator<EdgeIterator, Edge*> {
	public:
		inline EdgeIterator(const DetailedPath& dpath) : dpath_iter(dpath._path), done(false) { goToNextEdge(); }
		inline EdgeIterator(const EdgeIterator& source): dpath_iter(source.dpath_iter), done(false) { goToNextEdge(); }
		inline EdgeIterator& operator=(const EdgeIterator& source) { dpath_iter = source.dpath_iter; return *this; }

		inline bool ended(void) const { return done == true; }
		inline Edge* item(void) const { return dpath_iter.item().getEdge(); }
		inline void next(void) { dpath_iter++; goToNextEdge(); }

	private:
		// this is the only method of this class that doesn't assume we are positionned on a "valid state" (that is, done || dpath_iter->isEdge())
		void goToNextEdge() {
			while(!dpath_iter.ended())
			{
				if((*dpath_iter).isEdge())
					return; // we still have an edge to read
				dpath_iter++;
			}
			done = true; // only non-edges
		}

		SLList<FlowInfo>::Iterator dpath_iter;
		bool done;
	}; // EdgeIterator class

	// Iterator class
	class Iterator: public SLList<FlowInfo>::Iterator {
	public:
		inline Iterator(const DetailedPath& dpath) : SLList<FlowInfo>::Iterator(dpath._path) { }
	};

	void removeCallsAtEndOfPath();
private:
	void removeDuplicates();
	void removeAntagonists();
	io::Output& print(io::Output& out) const;
	
	SLList<FlowInfo> _path;
	CFG* fun;
}; // DetailedPath class

#endif