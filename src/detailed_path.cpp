#include "detailed_path.h"
#include "debug.h"
#include "otawa/cfg/features.h"

/**
 * @class DetailedPath
 * @brief A path enriched with flowfact information such as loops, calls, etc.
 */
/**
 * @class DetailedPath::Iterator
 * @brief Iterator on all the compounds of the DetailedPath
 */
/**
 * @class DetailedPath::FlowInfo
 * @brief Flowfact information around the path, such as loops, calls, etc.
 */
/**
 * @class DetailedPath::EdgeIterator
 * @brief Iterator only on the actual edges of the DetailedPath
 */
DetailedPath::FlowInfo::FlowInfo(kind_t kind, BasicBlock* bb) : _kind(kind), _identifier(bb) { assert(isBasicBlockKind(kind)); }
DetailedPath::FlowInfo::FlowInfo(kind_t kind, Edge* e) : _kind(kind), _identifier(e) { assert(isEdgeKind(kind)); }
// DetailedPath::FlowInfo::FlowInfo(const FlowInfo& fi) : _kind(fi._kind), _identifier(fi._identifier) { }
DetailedPath::FlowInfo::FlowInfo(const FlowInfo& fi) { _kind = fi._kind; _identifier = fi._identifier; }

DetailedPath::DetailedPath() { }
DetailedPath::DetailedPath(const SLList<Edge*>& edge_list)
{
	for(SLList<Edge*>::Iterator iter(edge_list); iter; iter++)
		this->addLast(*iter);
}
DetailedPath::DetailedPath(const DetailedPath& dp) : _path(dp._path) { }

/**
 * @fn inline void DetailedPath::clear(void);
 * @brief Reset the detailed path
 */

/**
 * @fn void DetailedPath::addLast(Edge* e);
 * @brief add Edge at the end of the DetailedPath
 * @param e Edge to add
 */
void DetailedPath::addLast(Edge* e)
	{ _path.addLast(FlowInfo(FlowInfo::KIND_EDGE, e)); }

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
	removeCallsAtEndOfPath();
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
			// Be careful, do not call prev before iter, because remove(prev) does NOT update info in iter.
			remove(iter);
			remove(prev);
			return removeAntagonists(); // parse again
		}
	}
}

void DetailedPath::removeCallsAtEndOfPath()
{
	Iterator last_edge_iter(*this);
	for(Iterator iter(*this); iter; iter++)
		if(iter->isEdge())
			last_edge_iter = iter;
	// last_edge_iter is now at the position of the last edge in the path
	if(!last_edge_iter)
		return; // nothing remains, exit
	for(last_edge_iter++; last_edge_iter; )
		if(last_edge_iter->isCall())
			remove(last_edge_iter);
		else last_edge_iter++;
}

/**
  * Add missing enclosing loop headers at the beginning of the path
  */
void DetailedPath::addEnclosingLoop(Block* loop_header)
{
	ASSERT(loop_header->isBasic())	
	if(!contains(FlowInfo(FlowInfo::KIND_LOOP_ENTRY, loop_header->toBasic())))
		_path.addFirst(FlowInfo(FlowInfo::KIND_LOOP_ENTRY, loop_header->toBasic()));
}

void DetailedPath::onLoopEntry(Block* loop_header)
{
	ASSERT(loop_header->isBasic())
	_path.addLast(FlowInfo(FlowInfo::KIND_LOOP_ENTRY, loop_header->toBasic())); // TODO! change this
}

void DetailedPath::onLoopExit(Option<Block*> new_loop_header) // TODO: shouldn't this be old_loop_header?
{
	ASSERT(!new_loop_header || (*new_loop_header)->isBasic())
	if(new_loop_header)
		_path.addLast(FlowInfo(FlowInfo::KIND_LOOP_EXIT, (*new_loop_header)->toBasic())); // TODO! change this
	else _path.addLast(FlowInfo(FlowInfo::KIND_LOOP_EXIT, (BasicBlock*)NULL));
}

void DetailedPath::onCall(Edge* e)
{
	return; // TODO!!
	/*
	_path.addLast(FlowInfo(FlowInfo::KIND_CALL, e));
	*/
}

void DetailedPath::onReturn(Block* b)
{
	// TODO!!
	/*
	BasicBlock::InIterator ins(bb);
	while(otawa::RECURSIVE_LOOP(ins))
	{
		ins++;
		ASSERTP(ins, "Only recursive loop calls have been found pointing to BasicBlock bb.")  // useless check in case of a well-formed CFG
	}
	_path.addLast(FlowInfo(FlowInfo::KIND_RETURN, ins->source()));
	while(ins++)
		ASSERTP(otawa::RECURSIVE_LOOP(ins), "First BB of function has several incoming non-recursive edges (e.g. it is called several times)");
	*/
}

/**
 * @fn bool DetailedPath::weakEqualsTo(const DetailedPath& dp) const;
 * @brief weak equality: test if the edge paths are the same - do not consider other flow info
 * 
 * @param dp DetailedPath to compare to
 */
bool DetailedPath::weakEqualsTo(const DetailedPath& dp) const
{
	EdgeIterator dp_iter(dp), this_iter(*this);
	while(dp_iter && this_iter)
	{
		if(*this_iter != *dp_iter) // compare edge*
			return false; // not equal
		dp_iter++, this_iter++;
	}
	if(dp_iter || this_iter) // if we are not done parsing one of the two paths
		return false; // then not equal
	return true; // all checks passed, equal
}

#define TODO
/**
 * @fn void DetailedPath::merge(const Vector<DetailedPath>& paths);
 * Modifies the current path to be the result of the merge of a Vector of path (basically we're going to preserve info such as loop and call info)
 * Previous info in current DetailedPath is ignored
 */
void DetailedPath::merge(const Vector<DetailedPath>& paths)
{
	ASSERTP(paths, "merge called with empty paths vector")
	_path.clear(); // do not take in account current path
// #ifdef DBGG
#if 0
	int merge_count = 0;
	for(Vector<DetailedPath>::Iterator iter(paths); iter; iter++)
		cout << color::IRed() << "\tmerge[" << merge_count++ << "]=" << *iter << io::endl;
	cout << color::RCol();
	Edge* first_call = NULL;
	for(Iterator this_iter(paths.first()); this_iter; this_iter++)
	{
		if(this_iter->isCall())
		{
			first_call = this_iter->getEdge();
			break;
		}
	}
	if(first_call == NULL)
		return; // nothing to do
	else cout << color::ICya() << "is call:" << first_call << color::RCol() << io::endl;
	int count = 0;
	for(Vector<DetailedPath>::Iterator paths_iter(paths); paths_iter; paths_iter++)
	{
		cout << "DP#" << count++ << ": ";
		for(DetailedPath::Iterator flowinfo_iter(*paths_iter); flowinfo_iter; flowinfo_iter++)
		{
			if(flowinfo_iter->isCall())
				cout << flowinfo_iter->toString() << ", ";
		}
		cout << io::endl;
	}
#endif
	/* explanation of the algorithm:
		Path#0: C#8, C#24, 
		Path#1: C#8, C#24, 
		Path#2: C#8
		Path#4: C#8, C#24, C#25,

		First, take this->_path := Path#0 (C#8, C#24)
		Then for each Path#1-4 "p", check that p contains this->_path.
		For example, when p=Path#2, p (C#8) does not contain this->_path (C#8, C#24) does not contain p, so we remove C#24 from this->_path
	*/ 
	bool first = true;
	for(Vector<DetailedPath>::Iterator paths_iter(paths); paths_iter; paths_iter++)
	{
		const DetailedPath &p = *paths_iter;
		if(first) // Path#0: initialize this->_path
		{
			first = false;
			// this->_path initialized with all calls contained in paths[0]
			for(DetailedPath::Iterator p_flowinfo_iter(p); p_flowinfo_iter; p_flowinfo_iter++)
			{
				if(p_flowinfo_iter->isCall())
					this->_path.addLast(*p_flowinfo_iter); // use addLast instead of addFirst to preserve order of calls
			}
		}
		else
		{
			for(DetailedPath::Iterator this_iter(*this); this_iter; ) // for each element in this->_path
			{
				if(!p.contains(this_iter)) // if it is not in p
				{
					#ifdef DBGG
						cout << color::ICya() << "!p.contains(this_iter);" << color::RCol() << io::endl;
						cout << "\t* p=" << p << io::endl;
						cout << "\t* this=" << *this << io::endl;
						cout << "\t* this_iter=" << *this_iter << io::endl;
						cout << "this->remove(*this_iter);" << io::endl;
					#endif
					this->_path.remove(this_iter); // remove it from this->_path
				}
				else this_iter++;
			}
		}
	}
#	ifdef DBGG
		// cout << color::ICya() << "result: " << toString() << color::RCol() << io::endl;
#	endif
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
	ASSERTP(rtn != NULL, "lastEdge() called on empty DetailedPath");
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

elm::String DetailedPath::toString(bool colored) const
{
	elm::String str;
	bool first = true;
	if(dbg_flags&DBG_PRINT_FLOWINFO)
	{
		// iterate on everything
		for(Iterator iter(*this); iter; iter++)
		{
			if(first)
				first = false;
			else
				str = str.concat(_ << ", ");
			str = str.concat(iter->toString(colored));
		}
	}
	else
	{
		// print limited info, iterate only on edges
		for(EdgeIterator iter(*this); iter; iter++)
		{
			if(first)
				first = false;
			else
				str = str.concat(_ << ", ");
			str = str.concat(_ << iter->source()->index() << "->" << iter->target()->index());
		}
	}
	return str;
}

io::Output& DetailedPath::print(io::Output& out) const
{
	return out << toString();
}

elm::String DetailedPath::FlowInfo::toString(bool colored) const
{
	switch(_kind)
	{
		case KIND_EDGE:
			return _ << getEdge()->source()->index() << "->" << getEdge()->target()->index();
		case KIND_LOOP_ENTRY:
			return _ << color::Dim() << "LEn#" << getLoopHeader()->index() << color::NoDim();
		case KIND_LOOP_EXIT:
			return _ << color::Dim() << "LEx#" << getLoopHeader()->index() << color::NoDim();
		case KIND_CALL:
			return _ << color::Dim() << "C#" << getEdge()->source()->index() << color::NoDim();
			// return _ << color::Dim() << "C#" << getEdge()->source()->index() << "-" << getEdge()->target()->index() << color::NoDim();
		case KIND_RETURN:
			return _ << color::Dim() << "R#" << getBasicBlock()->index() << color::NoDim();
		default:
			return _ << "[UKNOWN KIND " << (int)_kind << "]";
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
