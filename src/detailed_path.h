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
private:
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
		};
		FlowInfo(kind_t kind, BasicBlock* bb);
		FlowInfo(kind_t kind, Edge* e);
		inline bool isEdge() const { return _kind == KIND_EDGE; }
		inline Edge* getEdge() const { assert(isEdgeKind(_kind)); return (Edge*)_identifier; } // TODO! remove the assert

	private:
		inline bool isBasicBlockKind(kind_t kind) const { return (kind == KIND_LOOP_ENTRY) || (kind == KIND_LOOP_EXIT); }
		inline bool isEdgeKind(kind_t kind) const { return (kind == KIND_EDGE) || (kind == KIND_CALL); }
		kind_t _kind;
		void* _identifier; // BasicBlock*, Edge*
	}; // FlowInfo class

public:
	DetailedPath();
	DetailedPath(const SLList<Edge*>& edge_list);
	void clear();
	void addLast(Edge* e);
	void onLoopEntry(BasicBlock* loop_header);
	void onLoopExit(Option<BasicBlock*> new_loop_header);
	void onCall(Edge* e);
	void onReturn(); // TODO!
	Edge* lastEdge() const;
	SLList<Edge*> toOrderedPath() const;
	elm::String toString() const;

	// EdgeIterator class
	class EdgeIterator: public PreIterator<EdgeIterator, Edge*> {
	public:
		inline EdgeIterator(const DetailedPath& dpath) : dpath_iter(dpath.path), done(false) { goToNextEdge(); }
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
	};

private:
	SLList<FlowInfo> path;
}; // DetailedPath class

/*class InfeasiblePaths
{
public:
	InfeasiblePaths();
	void addOrderedPath(const SLList<Edge*>& infeasible_path);
	void addDisorderedPath(const Set<Edge*>& infeasible_path, const SLList<Edge*>& full_path, Edge* last_edge);

	// inline SLList<Edge*> getOrderedPaths() const { return paths; }

private:
	Vector<SLList<Edge*> > ips;
};*/

#endif