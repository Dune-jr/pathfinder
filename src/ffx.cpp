#include "ffx.h"

FFX::FFX(const Vector<DetailedPath>& ips) : infeasible_paths(ips), indent_level(0) { }

void FFX::output(const elm::String& filename)
{
	io::OutFileStream FFXStream(filename);
	io::Output FFXFile(FFXStream);

	// header
	FFXFile << indent(  ) << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" << endl;
	FFXFile << indent(  ) << "<flowfacts>" << endl;
	FFXFile << indent(+1) << "<function name=\"main\">" << endl; indent(+1);

	for(Vector<DetailedPath>::Iterator iter(infeasible_paths); iter; iter++)
		printInfeasiblePath(FFXFile, *iter);

	// footer
	FFXFile << indent(-1) << "</function>" << endl;
	FFXFile << indent(-1) << "</flowfacts>" << endl;
}

void FFX::printInfeasiblePath(io::Output& FFXFile, const DetailedPath& ip)
{
	int active_loops = 0;
	FFXFile	<< indent(  ) << "<not-all seq=\"true\">" << endl; indent(+1);
	for(DetailedPath::Iterator iter(ip); iter; iter++)
	{
		if(iter->isEdge())
		{
			const Edge* e = iter->getEdge();
			FFXFile << indent(  ) << "<edge src=\"0x" << e->source()->address() << "\" dst=\"0x" << e->target()->address() << "\" />" 
					<< " <!-- " << e->source()->number() << "->" << e->target()->number() << " -->" << endl;
		}
		else if(iter->isLoopEntry())
		{
			const BasicBlock* loop_header = iter->getLoopHeader();
			FFXFile << indent(  ) << "<loop address=\"0x" << loop_header->address() << "\">"
					<< " <!-- loop " << loop_header->number() << " -->" << endl; indent(+1);
			FFXFile << indent(  ) << "<iteration number=\"n\">" << endl; indent(+1);
			active_loops++;
		}
		else if(iter->isLoopExit())
		{
			if(active_loops > 0)
			{
				FFXFile << indent(-1) << "</iteration>" << endl;
				if(const BasicBlock* loop_header = iter->getLoopHeader()) // if not NULL, we have info
					FFXFile << indent(-1) << "</loop> <!-- loop " << loop_header->number() << "-->" << endl;
				else
					FFXFile << indent(-1) << "</loop>" << endl;
				active_loops--;
			}
			// TODO: else problem!
		}
	}
	// close running <loop ... > environments
	for( ; active_loops > 0; active_loops--)
	{
		FFXFile << indent(-1) << "</iteration>" << endl;
		FFXFile << indent(-1) << "</loop>" << endl;
	}
	FFXFile << indent(-1) << "</not-all>" << endl;
}

void FFX::printInfeasiblePathOldNomenclature(io::Output& FFXFile, const DetailedPath& ip)
{
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
		ip_str = ip_str.concat(_ << (*iter)->source()->number() << "->" << (*iter)->target()->number());

		FFXFile << "\t\t\t\t\t<count src=\"0x" << (*iter)->source()->address() << "\" dst=\"0x" << (*iter)->target()->address() << "\" />" << endl;
		edge_count++;
	}
	ip_str = _ << ip_str << "]";

	// control-constraint footer
	FFXFile << "\t\t\t\t</add>" << endl
			<< "\t\t\t\t<const int=\"" << edge_count-1 << "\" />" << endl
			<< "\t\t\t</le>" << endl // <!-- @1->@3 + @5->@6 <= 1 -->
			<< "\t\t</control-constraint> <!-- " << ip_str << " infeasible path -->" << endl;
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
