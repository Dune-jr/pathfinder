#include "ffx.h"
#include "pretty_printing.h"

// TODO! do so that when there is NO LEx after a LEn, we use iteration=*
FFX::FFX(const Vector<DetailedPath>& ips) : infeasible_paths(ips), indent_level(0) { }
void FFX::output(const elm::String& function_name, const elm::String& ffx_filename, const elm::String& graph_filename)
{
	io::OutFileStream FFXStream(ffx_filename);
	io::Output FFXFile(FFXStream);

	// header
	FFXFile << indent(  ) << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" << endl;
	FFXFile << indent(  ) << "<flowfacts>" << endl;
	FFXFile << indent(+1) << "<function name=\"" << function_name << "\">" << endl; indent(+1);

	for(Vector<DetailedPath>::Iterator iter(infeasible_paths); iter; iter++)
		printInfeasiblePath(FFXFile, *iter);

	// footer
	FFXFile << indent(-1) << "</function>" << endl;
	FFXFile << indent(-1) << "</flowfacts>" << endl;
	
	if(! graph_filename.isEmpty())
	{		
		io::OutFileStream GraphStream(graph_filename);
		io::Output GraphFile(GraphStream);
		writeGraph(GraphFile, infeasible_paths);
	}
}

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
			ASSERTP(e->source()->isBasic() || e->source()->isCall() || e->source()->isExit(), ": source not basic nor call nor exit: " << e->source() << "->" << e->target());
			ASSERTP(e->target()->isBasic() || e->target()->isCall() || e->target()->isExit(), ": target not basic nor call nor exit: " << e->source() << "->" << e->target());
			
			BasicBlock *source, *target;
			if(e->source()->isBasic())
				source = e->source()->toBasic();
			else if(e->source()->isSynth())
			{
				ASSERT(e->source()->isCall()) // otherwise would be virtual...
				ASSERTP(false, "source is caller, we should replace with the exit or better, close a call context")
				source = e->source()->toSynth()->ins().item()->source()->toBasic(); // source is a call block: replace with caller
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
			}
			if(e->target()->isBasic())
				target = e->target()->toBasic();
			else if(e->target()->isSynth())
			{
				ASSERT(e->target()->isCall()) // otherwise would be virtual...
				// target is a call block: replace with callee entry (TODOv2 ffx: temporary! we should add <call> context!)
				Block* callee_entry_target = e->target()->toSynth()->callee()->entry()->outs().item()->target();
				ASSERTP(callee_entry_target->isBasic(), "entry does not point to a BasicBlock???")
				target = callee_entry_target->toBasic();
			}
			else if(e->target()->isExit())
			{
				CFG::CallerIter citer(e->target()->cfg()->callers());
				ASSERTP(citer, "exit from main in IP???");
				Block::EdgeIter caller_exit_edges_iter(citer->outs());
				ASSERT(caller_exit_edges_iter);
				target = *caller_exit_edges_iter->target()->toBasic();
				ASSERT(!(++caller_exit_edges_iter));
				ASSERTP(!(++citer), "must be at max 1 outedge from caller")
			}
			FFXFile << indent(  ) << "<edge src=\"0x" << source->address() << "\" dst=\"0x" <<target->address()
					<< "\" /> <!-- " << (Block*)source << " -> " << (Block*)target << " -->" << endl;
		}
		else if(iter->isLoopEntry())
		{
			const BasicBlock* loop_header = iter->getLoopHeader();
			FFXFile << indent(  ) << "<loop address=\"0x" << loop_header->address() << "\" index=\"" << loop_header->index() << "\">"
					<< " <!-- loop " << loop_header->index() << " -->" << endl; indent(+1);
			FFXFile << indent(  ) << "<iteration number=\"n\">" << endl; indent(+1);
			open_tags += FFX_TAG_LOOP;
		}
		else if(iter->isLoopExit())
		{
			if(open_tags.isEmpty()) {
				elm::cout << "WARNING: </loop> found when no context is open" << endl;
				continue; // TODO! we should fix this LEx that doesn't have a previous LEn, that's a bug...
			}
			//ASSERTP(open_tags.first() == FFX_TAG_LOOP, "</loop> found when not directly in loop context");
			while(open_tags.first() != FFX_TAG_LOOP)
			{
				FFXFile << indent(-1) << "</call>" << endl;
				open_tags.removeFirst();
			}
			FFXFile << indent(-1) << "</iteration>" << endl;
			if(const BasicBlock* loop_header = iter->getLoopHeader()) // if not NULL, we have info
				FFXFile << indent(-1) << "</loop> <!-- loop " << loop_header->index() << "-->" << endl;
			else
				FFXFile << indent(-1) << "</loop>" << endl;
			open_tags.removeFirst();
		}
#ifdef v1
		else if(iter->isCall())
		{
			const BasicBlock* source = iter->getEdge()->source();
			FFXFile << indent( ) << "<call address=\"0x" << source->address() << "\"> <!-- call " << source->index() << " -->" << endl; indent(+1);
			open_tags += FFX_TAG_CALL;
		}
#endif
		else assert(false); // we should handle all kinds
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
