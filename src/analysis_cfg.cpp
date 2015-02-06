/*
 * Skeleton of the analysis algorithm, defines the way we parse the CFG 
 */

#include <ctime>
#include <elm/io/Output.h>
#include <elm/genstruct/SLList.h>
#include <otawa/cfg/Edge.h>
#include <otawa/cfg/BasicBlock.h>
#include <otawa/cfg/features.h>

#include "analysis.h"
#include "smt.h"
#include "debug.h"

using namespace elm::genstruct;
using namespace elm::io;

/*
void Analysis::initializeAnalysis()
{
	wl.add(Analysis::State(cfg->entry(), sp)); // wl <- {ɛ}
}
*/

Analysis::State::State(BasicBlock* entrybb, const OperandVar& sp, unsigned int max_tempvars, unsigned int max_registers)
	: sp(sp), constants(max_tempvars, max_registers)
{
	BasicBlock::OutIterator outs(entrybb);
	ASSERT(outs);
	path += *outs;
	generated_preds.clear(); // generated_preds := [[]]
	labelled_preds.clear(); // labelled_preds := [[]]
	labelled_preds += SLList<LabelledPredicate>::null; // add an empty list as first element
	constants.set(sp, SP, false); // set that ?13==SP (since SP is the value of ?13 at the beginning of the program)
}

void Analysis::State::appendEdge(Edge* e)
{
	// add edge to the end of the path
	this->path.addLast(e);
	// we now need to label the correct list of predicates
	const SLList<LabelledPredicate> &relevant_preds = (e->kind() == Edge::TAKEN) ?
		generated_preds_taken : // TAKEN
		generated_preds;  // NOT TAKEN, VIRTUAL, VIRTUAL RETURN

	// label our list of predicates with the current edge then append it
	SLList<LabelledPredicate> labelled_analysis_result = labelPredicateList(relevant_preds, e);
	labelled_preds += labelled_analysis_result;
	// label the constants as well
	constants.label(e);
}

// WARNING: atm, this function assumes we have NO LOOPS!
void Analysis::processCFG(CFG* cfg)
{
	DBG(color::Whi() << "Processing CFG " << cfg)
	paths_count = infeasible_paths_count = total_paths = 0;
	placeboProcessCFG(cfg);
	std::time_t timestamp = clock(); // Timestamp before analysis
	
	/* *Algorithm*
	begin
		wl <- {ɛ}; // init to root
		lock[] <- {{}}; // not exactly an array struct

		While wl != ∅
			sl::wl <- wl; // pop
			bb <- sl[0].last.target(); // whichever element of il will do

			If bb.ins.count() > 1 // if we have to join several paths
				lock[bb] += sl; // adds new element sl

				If lock[bb].count() < bb.ins.count // we haven't explored all the incoming paths
		 			continue;
				End If
				sl <- mergeIntoOneList(lock[bb]); // for instance {{1,2},{3},{4}} becomes {1,2,3,4}
			End If
			// à ce stade, toutes les entrées ont été traitées

			For s in sl
				processBB(s, bb); // modification des prédicats dans s
			End For

			For e in bb.outs
				wl <- sl ⊙ e; // ⊙ = une sorte de concaténation de langages
			End For
		End While
	end
	*/

	/* begin */
	/* wl <- {ɛ} */
	wl.push(cfg->firstBB());
	/* lock[] <- {{}}; */
	Identifier<SLList<Analysis::State> > PROCESSED_EDGES("Analyis::IP analysis processed incoming edges"); //, SLList<State>::null);
	BasicBlock::OutIterator entry_outs(cfg->entry());
	SLList<State> entry_annotation;
	entry_annotation += Analysis::State(cfg->entry(), sp, max_tempvars, max_registers);
	PROCESSED_EDGES(*entry_outs) = entry_annotation;

	/* While wl != ∅ */
	while(!wl.isEmpty())
	{
		DBG(color::IRed() << "wl=" << wlToString())
		/* bb::wl <- wl; */
		BasicBlock *bb = wl.pop();
		SLList<Analysis::State> sl;

		int bb_ins_count = 0;
		for(BasicBlock::InIterator bb_ins(bb); bb_ins; bb_ins++) { bb_ins_count++; }
		/*  If lock[bb].count() < bb.ins.count
				continue;
			End If
		*/
		if(countAnnotations(bb, PROCESSED_EDGES) < bb_ins_count)
			continue;
		/* sl <- mergeIntoOneList(lock[bb]); */
		for(BasicBlock::InIterator bb_ins(bb); bb_ins; bb_ins++)
			sl.addAll(*PROCESSED_EDGES(*bb_ins));
		// at this point of the algorithm, all incoming edges of bb have been processed

		/* For s in sl */
		bool sl_is_empty = true;
		for(SLList<Analysis::State>::MutableIterator sl_iter(sl); sl_iter; )
		{
			DBG(color::Whi() << "Processing path " << sl_iter.item().getPathString())
			/* processBB(s, bb); */
			if(processBB(sl_iter.item(), bb) > 0)
				sl.remove(sl_iter); // path ended
			else
			{
				sl_is_empty = false;
				sl_iter++;
			}
		}
		/* End For */

		if(sl_is_empty) // all paths have been terminated
			continue;

		/*	For e in bb.outs */
		for(BasicBlock::OutIterator bb_outs(bb); bb_outs; bb_outs++)
		{
			if(isAHandledEdgeKind(bb_outs->kind())) // filter out calls etc
			{
				Edge* e = *bb_outs;
				/* wl <- sl ⊙ e; */
				SLList<Analysis::State> new_sl;
				Vector<Option<Path> > sl_paths;
				for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++)
				{
					State s = sl_iter.item();
					s.appendEdge(e);
					// SMT call(s)
					SMT smt;
					Option<Path> infeasible_path = smt.seekInfeasiblePaths(s);
					sl_paths.push(infeasible_path); // TODO find more appropriate struct
					if(infeasible_path)
					{	// TODO: why should i have to put brackets
						ASSERT((*infeasible_path).contains(s.lastEdge())); // make sure the last edge was relevant in this path
					}
					else
						new_sl += s;
				}
				Vector<Option<Path> >::Iterator sl_paths_iter(sl_paths);
				for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++, sl_paths_iter++)
				{
					const State& s = sl_iter.item();
					// s.appendEdge(e);
					if(*sl_paths_iter) // infeasible?
					{
						const Path& infeasible_path = **sl_paths_iter;
						DBG("Path " << s.getPathString() << "->" << e->target()->number() << " minimized to " << pathToString(infeasible_path))
						bool valid = true;
						Vector<Option<Path> >::Iterator sl_paths_subiter(sl_paths);
						for(SLList<Analysis::State>::Iterator sl_subiter(sl); sl_subiter; sl_subiter++, sl_paths_subiter++)
						{
							if(!*sl_paths_iter && isSubPath((*sl_subiter).getPath(), e, infeasible_path)) // feasible path && contained in the minimized inf. path
							{
								valid = false;
								// DBG("isSubPath(" << (*sl_subiter).getPath() << ", " << e->source()->number()<<">"<<e->target()->number() << ", " 
								// 	<< pathToString(infeasible_path) << ") = " << isSubPath((*sl_subiter).getPath(), e, infeasible_path))
								break;
							}
						}
						DBG(color::BIWhi() << "B)" << color::RCol() << " Verifying minimized path validity... " << (valid?color::IGre():color::IRed()) << (valid?"SUCCESS!":"FAILED!"))
						// TODO!!!
						infeasible_paths += infeasible_path;
						DBG(color::On_IRed() << "Inf. path found: " << pathToString(infeasible_path))
						onAnyInfeasiblePath();
					}
				}
				PROCESSED_EDGES(*bb_outs) = new_sl; // annotate regardless of new_sl being empty or not
				if(!wl.contains(bb_outs->target()))
					wl.push(bb_outs->target());
			}
		}
		/* End For */
	}
	/* End While */
	/* end */

	// analysis complete, print infeasible paths
	printResults((clock()-timestamp)*1000/CLOCKS_PER_SEC);
}

/*
	0: continue
	1: stop analysis for this path
*/
int Analysis::processBB(State& s, BasicBlock* bb)
{	//  TODO! reorganize to put more stuff into subfunctions
	if(bb->isExit())
	{
		onPathEnd();
		return 1;
	}
	
	DBG(color::Whi() << "Processing " << bb)
	s.processBB(bb); // generates lists of predicates in generated_preds and generated_preds_taken	
	return 0;
}

void Analysis::placeboProcessCFG(CFG* cfg)
{
	if(dbg_flags&DBG_NO_DEBUG)
		cout << "Running pre-analysis... ";
	else
		DBG(color::Whi() << "Running pre-analysis... ")

	placeboProcessBB(cfg->firstBB());

	if(dbg_flags&DBG_NO_DEBUG)
		cout << total_paths << " paths found." << endl;
	else
		DBG(color::Whi() << total_paths << " paths found.")
}

void Analysis::placeboProcessBB(BasicBlock* bb)
{
	if(bb->isExit())
	{
		total_paths++;
		return;
	}

	for(BasicBlock::OutIterator outs(bb); outs; outs++)
	{
		if(outs->kind() == Edge::TAKEN
		|| outs->kind() == Edge::NOT_TAKEN
		|| outs->kind() == Edge::VIRTUAL
		|| outs->kind() == Edge::VIRTUAL_RETURN) // Filter out irrelevant edges (calls...)
		{	
			placeboProcessBB((*outs)->target());
		}
	}
}

void Analysis::printResults(int exec_time_ms)
{
	int infeasible_paths_count = infeasible_paths.count();
	if(dbg_flags&DBG_NO_TIME)
		DBG(color::BIGre() << infeasible_paths_count << " infeasible path" << (infeasible_paths_count == 1 ? "" : "s") << " found: ")
	else
		DBG(color::BIGre() << infeasible_paths_count << " infeasible path" << (infeasible_paths_count == 1 ? "" : "s") << " found: "
			<< "(" << (exec_time_ms>=1000 ? ((float)exec_time_ms)/(float(100)) : exec_time_ms) << (exec_time_ms>=1000 ? "s" : "ms") << ")")
	for(Set<Path>::Iterator iter(infeasible_paths); iter; iter++)
	{
		Path l = *iter;
		bool first = true;
		elm::String str = "    * [";
		for(Path::Iterator subiter(l); subiter; subiter++)
		{
			if(first) first = false; else // do nothing at first iteration
				str = _ << str << ", ";
			str = str.concat(_ << (*subiter)->source()->number() << "->" << (*subiter)->target()->number());
		}
		str = _ << str << "]";
		DBG(color::IGre() << str)
		if(dbg_flags&DBG_NO_DEBUG)
			cout << str << endl;
	}
	if(dbg_flags&DBG_NO_DEBUG)
	{
		cout << infeasible_paths_count << " infeasible path(s) found.";
		if(!(dbg_flags&DBG_NO_TIME))
			cout << " (" << exec_time_ms/1000 << "." << exec_time_ms%1000 << "s)\n";
	}
}

void Analysis::onPathEnd()
{
	if(dbg_flags&DBG_NO_DEBUG)
	{		
		if((++paths_count % 100) == 0 || total_paths <= 1000)
		{
			cout << "(" << paths_count << "/" << total_paths << ")";
			if(infeasible_paths_count)
				cout << " !*" << infeasible_paths_count;
			cout << endl;
			infeasible_paths_count = 0;
		}
	}
	else DBG(color::BBla() << color::On_Yel() << "EXIT block reached")
}

void Analysis::onAnyInfeasiblePath()
{
	if(dbg_flags&DBG_NO_DEBUG)
	{
		infeasible_paths_count++;
		if((++paths_count % 100) == 0 || total_paths <= 1000)
		{
			cout << "(" << paths_count << "/" << total_paths << ") !*" << infeasible_paths_count << endl;
			infeasible_paths_count = 0;
		}
	}
	else DBG(color::BIYel() << "Stopping current path analysis")
}

bool Analysis::isAHandledEdgeKind(Edge::kind_t kind)
{
	return (kind == Edge::TAKEN) || (kind == Edge::NOT_TAKEN) || (kind == Edge::VIRTUAL) || (kind == Edge::VIRTUAL_RETURN);
}

int Analysis::countAnnotations(BasicBlock* bb, const Identifier<SLList<Analysis::State> >& annotation_identifier) const
{
	int processed_edges_count = 0;
	for(BasicBlock::InIterator bb_ins(bb); bb_ins; bb_ins++)
		if(annotation_identifier.get(*bb_ins))
			processed_edges_count++;
	return processed_edges_count;
}

bool Analysis::isSubPath(const OrderedPath& included_path, const Edge* e, const Path& path_set) const
{
	for(Path::Iterator iter(path_set); iter; iter++)
		if(*iter != e && !included_path.contains(*iter))
			return false;
	return true;
}

elm::String Analysis::wlToString() const
{
	elm::String rtn = "[";
	bool first = true;
	for(Vector<BasicBlock*>::Iterator iter(wl); iter; iter++)
	{
		if(first) first = false; else
			rtn = rtn.concat((CString)", ");
		rtn = _ << rtn << (*iter)->number();
	}
	rtn = rtn.concat((CString)"]");
	return rtn;
}

elm::String Analysis::pathToString(const Path& path) const
{
	elm::String str = "[";
	bool first = true;
	for(Analysis::Path::Iterator iter(path); iter; iter++)
	{
		if(first)
			first = false;
		else
			str = str.concat(_ << ", ");
		str = str.concat(_ << (*iter)->source()->number() << "->" << (*iter)->target()->number());
	}
	str = str.concat(_ << "]");
	return str;
}