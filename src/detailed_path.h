#ifndef _INFEASIBLE_PATH_H
#define _INFEASIBLE_PATH_H

#include <elm/genstruct/SLList.h> 
#include <elm/genstruct/Vector.h>
#include <elm/string/String.h>
#include <otawa/cfg/Edge.h>
#include <elm/util/Option.h>

using namespace elm;
using namespace elm::genstruct;
using otawa::Edge;
using otawa::BasicBlock;

class DetailedPath
{
public:
	class FlowInfo;
	class Iterator;

	DetailedPath();
	DetailedPath(const SLList<Edge*>& edge_list);
	DetailedPath(const DetailedPath& dp);
	
	// SLList methods
	inline void clear() { _path.clear(); }
	void addLast(Edge* e);
	inline void addLast(const FlowInfo& fi) { _path.addLast(fi); }
	inline void remove(Iterator &iter) { _path.remove(iter); }
	inline void removeLast() { _path.removeLast(); }
	
	// events
	void onLoopEntry(BasicBlock* loop_header);
	void onLoopExit(Option<BasicBlock*> new_loop_header);
	void onCall(Edge* e);
	void onReturn(); // TODO!

	// utility
	bool weakEqualsTo(const DetailedPath& dp) const;
	void addEnclosingLoop(BasicBlock* loop_header);
	void merge(const Vector<DetailedPath>& detailed_paths);
	void optimize();
	bool hasAnEdge() const;
	Edge* firstEdge() const;
	Edge* lastEdge() const;
	int countEdges() const;
	SLList<Edge*> toOrderedPath() const;
	elm::String toString(bool colored = true) const;
	inline const SLList<FlowInfo>& path() const { return _path; }
	inline const DetailedPath* operator->(void) const { return this; }
	friend io::Output& operator<<(io::Output& out, const DetailedPath& dp) { return dp.print(out); }

	// FlowInfo class
	class FlowInfo
	{
	public:
		enum kind_t
		{
			KIND_EDGE, // Edge
			KIND_LOOP_ENTRY, // BasicBlock
			KIND_LOOP_EXIT, // BasicBlock
			KIND_CALL, // Edge
			KIND_RETURN, // Edge
		};
		FlowInfo(kind_t kind, BasicBlock* bb);
		FlowInfo(kind_t kind, Edge* e);
		FlowInfo(const FlowInfo& fi);
		inline bool isEdge() const { return _kind == KIND_EDGE; }
		inline bool isLoopEntry() const { return _kind == KIND_LOOP_ENTRY; }
		inline bool isLoopExit() const { return _kind == KIND_LOOP_EXIT; }
		inline bool isCall() const { return _kind == KIND_CALL; }
		inline Edge* getEdge() const { assert(isEdgeKind(_kind)); return (Edge*)_identifier; } // TODO! remove the assert
		inline BasicBlock* getLoopHeader() const { assert(isBasicBlockKind(_kind)); return (BasicBlock*)_identifier; } // TODO! remove the assert
		elm::String toString(bool colored = true) const;
		inline bool operator==(const FlowInfo& fi) const { return (_kind == fi._kind) && (_identifier == fi._identifier); }
		inline FlowInfo& operator=(const FlowInfo& fi) { _kind = fi._kind; _identifier = fi._identifier; return *this; }
		inline const FlowInfo* operator->(void) const { return this; }
		friend io::Output& operator<<(io::Output& out, const FlowInfo& fi) { return fi.print(out); }

	private:
		kind_t _kind;
		void* _identifier; // BasicBlock*, Edge*
		inline bool isBasicBlockKind(kind_t kind) const { return (kind == KIND_LOOP_ENTRY) || (kind == KIND_LOOP_EXIT); }
		inline bool isEdgeKind(kind_t kind) const { return (kind == KIND_EDGE) || (kind == KIND_CALL); }
		io::Output& print(io::Output& out) const;
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
	class Iterator: public PreIterator<Iterator, FlowInfo> {
	public:
		inline Iterator() { }
		inline Iterator(const DetailedPath& dpath) : _iter(dpath._path) { }
		inline Iterator(const Iterator& iter) : _iter(iter._iter) { }

		inline bool ended(void) const { return _iter.ended(); }
		inline FlowInfo item(void) const { return _iter.item(); }
		inline void next(void) { _iter.next(); }

	private:
		SLList<FlowInfo>::Iterator _iter;
	}; // Iterator class

private:
	SLList<FlowInfo> _path;
	
	void removeDuplicates();
	void removeAntagonists();
	io::Output& print(io::Output& out) const;
}; // DetailedPath class

#endif