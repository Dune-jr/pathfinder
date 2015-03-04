#include "ffx.h"

FFX::FFX(const Set<Analysis::Path>& ips) : infeasible_paths(ips) { }

void FFX::output(const elm::String& filename) const
{
	io::OutFileStream FFXStream(filename);
	io::Output FFXFile(FFXStream);

	// header
	FFXFile << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" << endl
			<< "<flowfacts>" << endl
			<< "\t<function name=\"main\">" << endl;

	for(Set<Analysis::Path>::Iterator iter(infeasible_paths); iter; iter++)
		printInfeasiblePath(FFXFile, *iter);

	// footer
	FFXFile << "\t</function>" << endl
			<< "</flowfacts>" << endl;
}

void FFX::printInfeasiblePath(io::Output& FFXFile, const Analysis::Path& ip) const
{
	FFXFile	<< "\t\t<not_all>" << endl;
	String ip_str;
	bool first = true;
	for(Analysis::Path::Iterator iter(ip); iter; iter++)
	{
		if(first)
			first = false;
		else
			ip_str = _ << ", " << ip_str;
		ip_str = _ << (*iter)->source()->number() << "->" << (*iter)->target()->number() << ip_str;
		FFXFile << "\t\t\t<edge src=\"0x" << (*iter)->source()->address() << "\" dst=\"0x" << (*iter)->target()->address() << "\" />" << endl;
	}
	FFXFile << "\t\t</not_all> <!-- [" << ip_str << "] infeasible path -->" << endl;
}

void FFX::printInfeasiblePathOldNomenclature(io::Output& FFXFile, const Analysis::Path& ip) const
{
	// control-constraint header
	FFXFile	<< "\t\t<control-constraint>" << endl
			<< "\t\t\t<le>" << endl
			<< "\t\t\t\t<add>" << endl;

	bool first = true;
	int edge_count = 0;
	String ip_str = "[";
	for(Analysis::Path::Iterator iter(ip); iter; iter++)
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