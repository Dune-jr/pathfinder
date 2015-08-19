#include "detailed_path.h"

DetailedPath::FlowInfo::FlowInfo(kind_t kind, BasicBlock* bb) : _kind(kind), _identifier(bb) { assert(isBasicBlockKind(kind)); }
DetailedPath::FlowInfo::FlowInfo(kind_t kind, Edge* e) : _kind(kind), _identifier(e) { assert(isEdgeKind(kind)); }

DetailedPath::DetailedPath() { }
DetailedPath::DetailedPath(const SLList<Edge*>& edge_list)
{
	for(SLList<Edge*>::Iterator iter(edge_list); iter; iter++)
		this->addLast(*iter);
}

void DetailedPath::clear()
{
	path.clear();
}

void DetailedPath::addLast(Edge* e)
{
	path.addLast(FlowInfo(FlowInfo::KIND_EDGE, e));
}

void DetailedPath::onLoopEntry(BasicBlock* loop_header)
{
	path.addLast(FlowInfo(FlowInfo::KIND_LOOP_ENTRY, loop_header));
}

void DetailedPath::onLoopExit(Option<BasicBlock*> new_loop_header)
{
	if(new_loop_header)
		path.addLast(FlowInfo(FlowInfo::KIND_LOOP_EXIT, new_loop_header));
	path.addLast(FlowInfo(FlowInfo::KIND_LOOP_EXIT, (BasicBlock*)NULL));
}

void DetailedPath::onCall(Edge* e)
{
	// TODO!
}

void DetailedPath::onReturn()
{
	// TODO!
}

Edge* DetailedPath::lastEdge() const
{
	Edge* rtn = NULL;
	for(EdgeIterator iter(*this); iter; iter++)
		rtn = *iter;
	assert(rtn != NULL);
	return rtn;
}

SLList<Edge*> DetailedPath::toOrderedPath() const
{
	SLList<Edge*> rtn;
	for(EdgeIterator iter(*this); iter; iter++)
		rtn.addLast(*iter);
	return rtn;
}

elm::String DetailedPath::toString() const
{
	elm::String str;
	bool first = true;
	int lastid = 0; // -Wmaybe-uninitialized
	for(EdgeIterator iter(*this); iter; iter++)
	{
		// if(!first && (*iter)->source()->number() != lastid)
		// {
		// 	DBG("str=" << str)
		// 	DBG("lastid=" << lastid << ", (*iter)->source->number()=" << (*iter))
		// }
		// when path is x->y and y'->z, there must be y=y'
		ASSERTP(first || (*iter)->source()->number() == lastid, "OrderedPath previous target and current source do not match! ex: 1->2, 2->4, 3->5");
		if(first)
		{
#			ifndef NO_UTF8
				if((*iter)->source()->number() == 0)
					str = _ << "ε";
				else
#			endif
				str = _ << (*iter)->source()->number();
			first = false;
		}
		str = _ << str << "->" << (*iter)->target()->number();
		lastid = (*iter)->target()->number();
	}
	if(str.isEmpty())
		return "(empty)";
	return str;
}



/*void InfeasiblePaths::addDisorderedPath(const Set<Edge*>& infeasible_path, const SLList<Edge*>& full_path, Edge* last_edge)
{
	SLList<Edge*> ordered_ip;
	for(SLList<Edge*>::Iterator full_path_iter(full_path); full_path_iter; full_path_iter++)
	{
		if(infeasible_path.contains(*full_path_iter))
			ordered_ip.addLast(*full_path_iter);
	}
	if(infeasible_path.contains(last_edge))
		ordered_ip.addLast(last_edge);
	ips.add(ordered_ip);
}*/
