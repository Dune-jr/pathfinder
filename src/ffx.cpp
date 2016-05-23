#include "ffx.h"
#include "pretty_printing.h"

// TODO! do so that when there is NO LEx after a LEn, we use iteration=*
/**
 * @class FFX
 * @brief File output module of the infeasible path analysis 
 */
/**
 * @fn FFX::FFX(const Vector<DetailedPath>& ips);
 * @brief Initialize the FFX output module with the result of an Infeasible Path analysis
 * @param ips The Vector of infeasible paths returned by the analysis
 */
FFX::FFX(const Vector<DetailedPath>& ips) : infeasible_paths(ips), indent_level(0) { }

/**
 * @fn void FFX::output(const elm::String& function_name, const elm::String& ffx_filename, const elm::String& graph_filename);
 * @brief Output the result of the analysis in FFX format, and optionally in graph format
 * @param function_name Name of the function analysed
 * @param ffx_filename Full name of the FFX file to output to
 * @param graph_filename Full name of the Graph file to output to. Do not set or set to empty string if no graph output is desired
 */
void FFX::output(const elm::String& function_name, const elm::String& ffx_filename, const elm::String& graph_filename)
{
	io::OutFileStream FFXStream(ffx_filename);
	io::Output FFXFile(FFXStream);

	for(Vector<DetailedPath>::Iterator iter(infeasible_paths); iter; iter++)
		checkPathValidity(*iter);

	// header
	FFXFile << indent(  ) << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" << endl;
	FFXFile << indent(  ) << "<flowfacts>" /*" <!-- pathfinder, " << __DATE__ << " -->"*/ << endl;
	FFXFile << indent(  ) << "<function name=\"" << function_name << "\">" << endl; indent(+1);

	for(Vector<DetailedPath>::Iterator iter(infeasible_paths); iter; iter++)
		printInfeasiblePath(FFXFile, *iter);

	// footer
	FFXFile << indent(-1) << "</function>" << endl;
	FFXFile << indent(  ) << "</flowfacts>" << endl;
	
	if(! graph_filename.isEmpty())
	{		
		io::OutFileStream GraphStream(graph_filename);
		io::Output GraphFile(GraphStream);
		writeGraph(GraphFile, infeasible_paths);
	}
}

/**
 * @brief Print a single infeasible path
 * @param FFXFile file to print the path in
 * @param ip path to print
 */
void FFX::printInfeasiblePath(io::Output& FFXFile, const DetailedPath& ip)
{
	SLList<ffx_tag_t> open_tags;
	// int active_loops = 0, active_calls = 0;
	FFXFile	<< indent(  ) << "<not-all seq=\"true\">" << endl; indent(+1);
	for(DetailedPath::Iterator iter(ip); iter; iter++)
	{
		if(iter->isEdge())
		{
			const Edge* e = iter->getEdge();
			if(e->source()->isEntry()) { // main program pt 
				cerr << "WARNING: ignoring " << e->source() << "->" << e->target() << ", assuming program entry edge!" << endl;
				continue;
			}
			ASSERTP(e->source()->isBasic() || e->source()->isCall() || e->source()->isExit(), ": source not basic nor call nor exit: " << e->source() << "->" << e->target());
			ASSERTP(e->target()->isBasic() || e->target()->isCall() || e->target()->isExit(), ": target not basic nor call nor exit: " << e->source() << "->" << e->target());
			
			Block *source = NULL, *target = NULL;
			if(e->source()->isBasic())
				source = e->source();
			else if(e->source()->isSynth())
			{
				ASSERT(e->source()->isCall()) // otherwise would be virtual...
				// here problem: we have to include the returning edgeS in the infeasible path. What does this mean? simply that we have called A.
				// But we don't want to include the call to A because we would lose sequentiality... so let's just assert that we already have at least one edge from that sub-CFG we're returning from
				CFG* subcfg = e->source()->toSynth()->callee();
				// cerr << "WARNING: infeasible path includes return edge of sub-CFG \"" << subcfg->name() 
				// 	 << "\" called from \"" << e->source()->toSynth()->caller() << "\"" << endl;
				bool includes_edge_in_subCFG = false;
				for(DetailedPath::Iterator new_iter(ip); *new_iter != *iter; new_iter++)
					if(new_iter->isEdge() && new_iter->getEdge()->source()->cfg() == subcfg) {
						includes_edge_in_subCFG = true;
						// cerr << "\tfound some edge in called CFG: " << new_iter->getEdge() << ", skipping return edge" << endl;
						break;
					}
				if(!includes_edge_in_subCFG)
					ASSERTP(false, "ERROR: infeasible path includes return edge of sub-CFG" << subcfg->name() << ", but no edge from that CFG!")
				FFXFile << indent(  ) << "<!-- skipped return edge of " << subcfg->name() << " -->" << endl;
				continue;
			}
			else if(e->source()->isExit())
			{
				ASSERTP(false, "case requires testing, source is exit... I don't think it's possible?")
				/*CFG::CallerIter citer(e->source()->cfg()->callers());
				ASSERTP(citer, "exit from main in IP???");
				Block::EdgeIter caller_exit_edges_iter(citer->outs());
				ASSERT(caller_exit_edges_iter);
				source = (*caller_exit_edges_iter)->target()->toBasic();
				ASSERT(!(++caller_exit_edges_iter));
				ASSERTP(!(++citer), "must be at max 1 outedge from caller")*/
			} else assert(false);

			if(e->target()->isBasic())
				target = e->target();
			else if(e->target()->isSynth())
			{
				ASSERT(e->target()->isCall()) // otherwise would be virtual...
				if(nextElementisCall(iter, e->target()->toSynth()->callee()))
				{
					FFXFile << indent(  ) << "<!-- skipped call edge of " << e->target()->toSynth()->callee()->name() << " -->" << endl;
					continue;
				}
				cerr << "WARNING: found a call edge (" << e->source() << "->" << e->target() << ") not followed by a call element. end of path=" << !bool(iter) << endl;
				// target is a call block: replace with callee entry
				Block* callee_entry_target = e->target()->toSynth()->callee()->entry()->outs().item()->target();
				ASSERTP(callee_entry_target->isBasic(), "entry does not point to a BasicBlock???")
				target = callee_entry_target;
			}
			else if(e->target()->isExit())
			{
				CFG::CallerIter citer(e->target()->cfg()->callers());
				ASSERTP(citer, "exit from main in IP???");
				Block::EdgeIter caller_exit_edges_iter(citer->outs());
				ASSERT(caller_exit_edges_iter);
				target = caller_exit_edges_iter->target();
				ASSERT(!(++caller_exit_edges_iter));
				ASSERTP(!(++citer), "must be at max 1 outedge from caller")
			}

			FFXFile << indent(  ) << "<edge src=\"0x" << source->address() << "\" dst=\"0x" << target->address()
					<< "\" /> <!-- " << (Block*)source << " -> " << (Block*)target << " -->" << endl;

		}
		else if(iter->isLoopEntry())
		{
			BasicBlock* loop_header = iter->getLoopHeader();
			FFXFile << indent(  ) << "<loop address=\"0x" << loop_header->address() << "\" index=\"" << loop_header->index() << "\">"
					<< " <!-- loop " << loop_header->index() << " -->" << endl; indent(+1);
			if(edgeAfter(ip.find(DetailedPath::FlowInfo(DetailedPath::FlowInfo::KIND_LOOP_EXIT, loop_header)))) {
				FFXFile << indent(  ) << "<iteration number=\"n\">" << endl; indent(+1);
				// cout <<"detected iteration=n on ip:" << ip << color::RCol() << endl;
			}
			else {
				FFXFile << indent(  ) << "<iteration number=\"*\">" << endl; indent(+1);
			}
			open_tags += FFX_TAG_LOOP;
		}
		else if(iter->isLoopExit())
		{
			if(open_tags.isEmpty()) {
				elm::cout << "WARNING: </loop> found when no context is open" << endl;
				continue; // TODO! we should fix this LEx that doesn't have a previous LEn, that's a bug...
			}
			ASSERTP(open_tags.first() == FFX_TAG_LOOP, "</loop> found when not directly in loop context");
/*			while(open_tags.first() != FFX_TAG_LOOP)
			{
				FFXFile << indent(-1) << "</call>" << endl;
				open_tags.removeFirst();
			}
*/			FFXFile << indent(-1) << "</iteration>" << endl;
			if(const BasicBlock* loop_header = iter->getLoopHeader()) // if not NULL, we have info
				FFXFile << indent(-1) << "</loop> <!-- loop " << loop_header->index() << "-->" << endl;
			else
				FFXFile << indent(-1) << "</loop>" << endl;
			open_tags.removeFirst();
		}
		else if(iter->isCall())
		{
			SynthBlock* caller = iter->getCaller();
			BasicBlock* callpoint = caller->ins().item()->source()->toBasic(); // TODO add asserts
			// FFXFile << indent( ) << "<call address=\"0x" << caller->address() << "\">"
			FFXFile << indent( ) << "<call address=\"0x" << callpoint->control()->address() << "\" name=\"" << caller->callee()->name() << "\">"
				" <!-- call " << caller->cfg() << ":" << caller->index() << " -> " << caller->callee() << " -->" << endl; indent(+1);
			open_tags += FFX_TAG_CALL;
		}
		else if(iter->isReturn())
		{
			const SynthBlock* caller = iter->getCaller();
			FFXFile << indent(-1) << "</call>"
				" <!-- return " << caller->cfg() << ":" << caller->index() << " <- " << caller->callee() << " -->" << endl;
			ASSERTP(open_tags.first() == FFX_TAG_CALL, "return found when call is not the most recent open tag")
			open_tags.removeFirst();
		}
		else 
			ASSERT(false); // we should handle all kinds
	}
	// close running <loop ... > environments
	for(SLList<ffx_tag_t>::Iterator open_tags_iter(open_tags); !open_tags_iter.ended(); open_tags_iter++)
	{
		switch(*open_tags_iter)
		{
			case FFX_TAG_LOOP:
				FFXFile << indent(-1) << "</iteration>" << endl;
				FFXFile << indent(-1) << "</loop>" << endl;
				break;
			case FFX_TAG_CALL:
				FFXFile << indent(-1) << "</call>" << endl;
				break;
		}
	}
	/*for( ; active_loops > 0; active_loops--)
	{
		FFXFile << indent(-1) << "</iteration>" << endl;
		FFXFile << indent(-1) << "</loop>" << endl;
	}
	for( ; active_calls > 0; active_calls--)
		FFXFile << indent(-1) << "</call>" << endl;*/
	FFXFile << indent(-1) << "</not-all>" << endl;
}

/**
 * @brief write graph data in a .tsv file
 */
void FFX::writeGraph(io::Output& GFile, const Vector<DetailedPath>& ips)
{
	int max = 0;
	for(Vector<DetailedPath>::Iterator i(ips); i; i++)
	{
		int c = i->countEdges();
		if(c > max) max = c;
	}
	Vector<int> vals(max);
	for(int i =0; i<max; i++)
		vals.push(0);
	GFile << "Length \tCount\n";
	for(Vector<DetailedPath>::Iterator i(ips); i; i++)
	{
		int c = i->countEdges();
		vals[c-1]++;
	}
	int i = 0;
	for(Vector<int>::Iterator iter(vals); iter; iter++)
		GFile << ++i << " \t" << *iter << endl; // output 24 1 and so on
}

void FFX::checkPathValidity(const DetailedPath& ip) const
{
	SLList<DetailedPath::FlowInfo> sl;
	for(DetailedPath::Iterator iter(ip); iter; iter++)
		switch(iter->kind())
		{
			case DetailedPath::FlowInfo::KIND_EDGE: // Edge
				break;
			case DetailedPath::FlowInfo::KIND_LOOP_ENTRY: // BasicBlock
				sl.push(*iter);
				break;
			case DetailedPath::FlowInfo::KIND_LOOP_EXIT: // BasicBlock
				ASSERTP(DetailedPath::FlowInfo(DetailedPath::FlowInfo::KIND_LOOP_ENTRY, iter->getBasicBlock()) == sl.pop(), "path " << ip << " invalid: Loop exit not matching loop entry");
				break;
			case DetailedPath::FlowInfo::KIND_CALL: // SynthBlock
				sl.push(*iter);
				break;
			case DetailedPath::FlowInfo::KIND_RETURN: // SynthBlock
				ASSERTP(DetailedPath::FlowInfo(DetailedPath::FlowInfo::KIND_CALL, iter->getSynthBlock()) == sl.pop(), "path " << ip << " invalid: Return not matching call");
				break;
		}
}

/**
 * @brief Old FFX output (obsolete)
 */
void FFX::printInfeasiblePathOldNomenclature(io::Output& FFXFile, const DetailedPath& ip)
{
#ifdef v1
	// control-constraint header
	FFXFile	<< "\t\t<control-constraint>" << endl
			<< "\t\t\t<le>" << endl
			<< "\t\t\t\t<add>" << endl;

	bool first = true;
	int edge_count = 0;
	String ip_str = "[";
	for(DetailedPath::EdgeIterator iter(ip); iter; iter++)
	{
		if(first)
			first = false;
		else
			ip_str = _ << ip_str << ", ";
		ip_str = ip_str.concat(_ << (*iter)->source()->index() << "->" << (*iter)->target()->index());

		FFXFile << "\t\t\t\t\t<count src=\"0x" << (*iter)->source()->address() << "\" dst=\"0x" << (*iter)->target()->address() << "\" />" << endl;
		edge_count++;
	}
	ip_str = _ << ip_str << "]";

	// control-constraint footer
	FFXFile << "\t\t\t\t</add>" << endl
			<< "\t\t\t\t<const int=\"" << edge_count-1 << "\" />" << endl
			<< "\t\t\t</le>" << endl // <!-- @1->@3 + @5->@6 <= 1 -->
			<< "\t\t</control-constraint> <!-- " << ip_str << " infeasible path -->" << endl;
#endif
}

/**
 * bool FFX::nextElementisCall(const DetailedPath::Iterator& iter, CFG* cfg);
 * @brief test if the next element in the DetailedPath is a call to cfg
 * @param current iterator on the detailed path
 * @param cfg called CFG to match with
*/
bool FFX::nextElementisCall(const DetailedPath::Iterator& iter, CFG* cfg)
{
	DetailedPath::Iterator new_iter(iter);
	new_iter++;
	if(!new_iter)
		return false;
	return new_iter->isCall() && new_iter->getCaller()->callee() == cfg;
}

/**
 * elm::String FFX::indent(int indent_increase);
 * @brief Change the indent level, and return a string corresponding to the (new) indent level
 */
elm::String FFX::indent(int indent_increase)
{
	elm::String str;
	indent_level += indent_increase;
	if(indent_level <= 0)
		indent_level = 0; // cannot have negative indent levels
	else 
		for(int i = 0; i < indent_level; i++)
			str = str + "\t";
	return str;
}
