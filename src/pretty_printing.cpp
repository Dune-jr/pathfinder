#include "pretty_printing.h"

/**
 * Print n indents to the given output
 */
void addIndents(elm::io::Output& out, int n)
{
	for(int i=0; i<n; i++)
		out << "\t";
}
