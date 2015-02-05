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
	total_paths = 0;
	placeboProcessBB(cfg->firstBB());
#	ifndef DBG_NO_DEBUG
		DBG(color::Whi() << "Running pre-analysis... ")
		DBG(color::Whi() << total_paths << " paths found.")
#	else
		cout << "Running pre-analysis... ";
		cout << total_paths << " paths found." << endl;
#	endif
	std::time_t timestamp = clock(); // Timestamp before analysis
	int paths_count = 0, infeasible_paths_count = 0;
	
	/*	
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
	SLList<State> entry_singleton;
	entry_singleton += Analysis::State(cfg->entry(), sp, max_tempvars, max_registers);
	wl.push(entry_singleton);
	/* lock[] <- {{}}; */
	Identifier<SLList<Analysis::State> > PROCESSED_EDGES("Analyis::IP analysis processed incoming edges"); //, SLList<State>::null);

	/* While wl != ∅ */
	while(!wl.isEmpty())
	{
		DBG(color::IRed() << "wl=" << wl)
		/* sl::wl <- wl; */
		SLList<Analysis::State> sl(wl.pop());
		ASSERT(sl);
		/* bb <- sl[0].last.target(); */
		BasicBlock *bb = sl.first().lastEdge()->target();

		/* If bb.ins.count() > 1 */
		int bb_ins_count = 0;
		for(BasicBlock::InIterator bb_ins(bb); bb_ins; bb_ins++) { bb_ins_count++; }
		if(bb_ins_count > 1)
		{
			/* lock[bb] += sl; */
			PROCESSED_EDGES(sl.first().lastEdge()) = sl;
			/*  If lock[bb].count() < bb.ins.count
					continue;
				End If
			*/
			int processed_edges_count = 0;
			for(BasicBlock::InIterator bb_ins(bb); bb_ins; bb_ins++)
			{
				if(PROCESSED_EDGES.get(*bb_ins))
					processed_edges_count++;
			}
			if(processed_edges_count < bb_ins_count)
				continue;
			/* sl <- mergeIntoOneList(lock[bb]); */
			sl.clear();
			for(BasicBlock::InIterator bb_ins(bb); bb_ins; bb_ins++)
				sl.addAll(*PROCESSED_EDGES(*bb_ins));
			DBG(color::IRed() << "extracted sl from annotation, got sl=" << sl)
		}
		/* EndIf */
		// at this point of the algorithm, all incoming edges of bb have been processed

		/* For s in sl */
		bool sl_is_empty = true;
		for(SLList<Analysis::State>::MutableIterator sl_iter(sl); sl_iter; )
		{	
			/* processBB(s, bb); */
			DBG(color::Whi() << "Processing path " << sl_iter.item().pathToString())
			if(processBB(sl_iter.item(), bb) > 0)
			{
				if((++paths_count % 100) == 0 || total_paths <= 1000)
				{
					cout << "(" << paths_count << "/" << total_paths << ")";
					if(infeasible_paths_count)
						cout << " !*" << infeasible_paths_count;
					cout << endl;
					infeasible_paths_count = 0;
				}
				sl.remove(sl_iter);
			}
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
			if(bb_outs->kind() == Edge::TAKEN
				|| bb_outs->kind() == Edge::NOT_TAKEN
				|| bb_outs->kind() == Edge::VIRTUAL
				|| bb_outs->kind() == Edge::VIRTUAL_RETURN) // Filter out irrelevant edges (calls...)
			{
				Edge* e = *bb_outs;
				/* wl <- sl ⊙ e; */
				SLList<Analysis::State> new_sl;
				for(SLList<Analysis::State>::Iterator sl_iter(sl); sl_iter; sl_iter++)
				{
					State s = sl_iter.item();
					s.appendEdge(e);
					// SMT call(s)
					DBG("Checking path " << s.pathToString() << ":")
					SMT smt;
					if(Option<Path> infeasible_path = smt.seekInfeasiblePaths(s.getLabelledPreds(), s.getConstants()))
					{
						DBG("Minimized path uses " << (*infeasible_path).count() << " out of " << s.getPath().count() << " edges.")
						infeasible_paths += *infeasible_path;
						infeasible_paths_count++;
						if((++paths_count % 100) == 0 || total_paths <= 1000)
						{
							cout << "(" << paths_count << "/" << total_paths << ") !*" << infeasible_paths_count << endl;
							infeasible_paths_count = 0;
						}
						DBG(color::BIYel() << "Current path identified as infeasible, stopping analysis")
					}
					else // do not add this state for future analysis if it's been identified as infeasible
						new_sl += s;
				}
				// if(new_sl) // new_sl may be empty if all its paths were infeasible, in this case we mustn't add a [] to the wl
					wl.push(new_sl);
			}
		}
		/* End For */
	}
	/* End While */
	/* end */

	// analysis complete, print infeasible paths
	infeasible_paths_count = infeasible_paths.count();
	int ms_diff;
	if(dbg_flags&DBG_NO_TIME)
		DBG(color::BIGre() << infeasible_paths_count << " infeasible path" << (infeasible_paths_count == 1 ? "" : "s") << " found: ")
	else
	{
		ms_diff = (clock()-timestamp)*1000/CLOCKS_PER_SEC;
		DBG(color::BIGre() << infeasible_paths_count << " infeasible path" << (infeasible_paths_count == 1 ? "" : "s") << " found: "
			<< "(" << (ms_diff>=1000 ? ((float)ms_diff)/(float(100)) : ms_diff) << (ms_diff>=1000 ? "s" : "ms") << ")")
	}
	for(Set<Path>::Iterator iter(infeasible_paths); iter; iter++)
	{
		Path l = *iter; // Path is Set<const Edge*>
		bool first = true;
		elm::String str = "    * [";
		for(Path::Iterator subiter(l); subiter; subiter++)
		{
			if(first)
				first = false;
			else
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
			cout << " (" << ms_diff/1000 << "." << ms_diff%1000 << "s)\n";
		cout << endl;
	}
}

/*
	0: continue
	1: stop analysis for this path
*/
int Analysis::processBB(State& s, BasicBlock* bb)
{	//  TODO! reorganize to put more stuff into subfunctions
	if(bb->isExit())
	{
		DBG(color::BBla() << color::On_Yel() << "EXIT block reached")
		return 1;
	}
	
	DBG(color::Whi() << "Processing " << bb)
	s.processBB(bb); // generates lists of predicates in generated_preds and generated_preds_taken	
	return 0;
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