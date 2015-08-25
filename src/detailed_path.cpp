#include "detailed_path.h"
#include "debug.h"

DetailedPath::FlowInfo::FlowInfo(kind_t kind, BasicBlock* bb) : _kind(kind), _identifier(bb) { assert(isBasicBlockKind(kind)); }
DetailedPath::FlowInfo::FlowInfo(kind_t kind, Edge* e) : _kind(kind), _identifier(e) { assert(isEdgeKind(kind)); }
DetailedPath::FlowInfo::FlowInfo(const FlowInfo& fi) : _kind(fi._kind), _identifier(fi._identifier) { }

DetailedPath::DetailedPath() { }
DetailedPath::DetailedPath(const SLList<Edge*>& edge_list)
{
	for(SLList<Edge*>::Iterator iter(edge_list); iter; iter++)
		this->addLast(*iter);
}
DetailedPath::DetailedPath(const DetailedPath& dp) : _path(dp._path) { }

/**
 * @fn inline void DetailedPath::clear(void)
 * @brief Reset the detailed path
 */

void DetailedPath::addLast(Edge* e)
{
	_path.addLast(FlowInfo(FlowInfo::KIND_EDGE, e));
}

/**
 * @fn void SLList::removeLast(void);
 * Remove the last item from the list.
 * @warning It is an error to call this method if the list is empty.
 */

/**
 * @fn void DetailedPath::optimize();
 * Remove redundant information such as loop entries followed by loop exits with nothing in between
 */
void DetailedPath::optimize()
{
	removeDuplicates();
	removeAntagonists();
}

void DetailedPath::removeDuplicates()
{
	Iterator iter(*this), prev(*this);
	if(!iter) // make sure the detailed path is not empty
		return;
	for(iter++; iter; prev++, iter++)
	{
		if(    ((iter->isLoopEntry() && prev->isLoopEntry()) || (iter->isLoopExit() && prev->isLoopExit()))
			&& (iter->getLoopHeader() == prev->getLoopHeader()))
		{
			remove(iter);
			return removeDuplicates(); // parse again
		}
	}
}

void DetailedPath::removeAntagonists()
{
	Iterator iter(*this), prev(*this);
	if(!iter) // make sure the detailed path is not empty
		return;
	for(iter++; iter; prev++, iter++)
	{
		if(iter->isLoopExit() && prev->isLoopEntry() && (iter->getLoopHeader() == prev->getLoopHeader()))
		{
			remove(prev);
			remove(iter);
			return removeAntagonists(); // parse again
		}
	}
}

void DetailedPath::addEnclosingLoop(BasicBlock* loop_header)
{
	_path.addFirst(FlowInfo(FlowInfo::KIND_LOOP_ENTRY, loop_header));
}

void DetailedPath::onLoopEntry(BasicBlock* loop_header)
{
	_path.addLast(FlowInfo(FlowInfo::KIND_LOOP_ENTRY, loop_header));
}

void DetailedPath::onLoopExit(Option<BasicBlock*> new_loop_header)
{
	if(new_loop_header)
		_path.addLast(FlowInfo(FlowInfo::KIND_LOOP_EXIT, new_loop_header));
	else _path.addLast(FlowInfo(FlowInfo::KIND_LOOP_EXIT, (BasicBlock*)NULL));
}

void DetailedPath::onCall(Edge* e)
{
	// TODO!
}

void DetailedPath::onReturn()
{
	// TODO!
}

bool DetailedPath::hasAnEdge() const
{
	return ! EdgeIterator(*this).ended();
}

Edge* DetailedPath::firstEdge() const
{
	return EdgeIterator(*this).item();
}

Edge* DetailedPath::lastEdge() const
{
	Edge* rtn = NULL;
	for(EdgeIterator iter(*this); iter; iter++)
		rtn = *iter;
	assert(rtn != NULL);
	return rtn;
}

int DetailedPath::countEdges() const
{
	int count = 0;
	for(SLList<FlowInfo>::Iterator iter(_path); iter; iter++)
		if((*iter).isEdge())
			count++;
	return count;
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
	for(Iterator iter(*this); iter; iter++)
	{
		if(first)
			first = false;
		else
			str = str.concat(_ << ", ");
		str = str.concat(iter->toString());
	}
	return str;
}

io::Output& DetailedPath::print(io::Output& out) const
{
	return(out << toString());
}

elm::String DetailedPath::FlowInfo::toString() const
{
	switch(_kind)
	{
		case KIND_EDGE:
			return _ << getEdge()->source()->number() << "->" << getEdge()->target()->number();
		case KIND_LOOP_ENTRY:
			return _ << "LEn#" << getLoopHeader()->number();
		case KIND_LOOP_EXIT:
		return _ << "LEx#" << getLoopHeader()->number();
		case KIND_CALL:
		default:
			return "";
	}
}

io::Output& DetailedPath::FlowInfo::print(io::Output& out) const
{
	return(out << toString());
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
