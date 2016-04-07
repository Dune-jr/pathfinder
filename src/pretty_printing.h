#ifndef _PRETTY_PRINTING_H
#define _PRETTY_PRINTING_H

#include <elm/genstruct/SLList.h>
#include <elm/genstruct/Vector.h>
#include <elm/io/Output.h>
#include "debug.h"
using elm::io::Output;
using elm::genstruct::SLList;
using elm::genstruct::Vector;

// pretty printing
void addIndents(Output& out, int n);
template <class C> Output& printCollection(Output& out, const C& items);
template <class T> inline Output& operator<<(Output& out, const SLList<T>& l) { return printCollection(out, l); }
template <class T> inline Output& operator<<(Output& out, const Vector<T>& v) { return printCollection(out, v); }

template <class C> elm::io::Output& printCollection(elm::io::Output& out, const C& items)
{
	int count = items.count();
	if(count > 25)
	{
		out << "#" << count << "#";
		return out; // just exit if too many items
	}
	
	static int indent = 0;
	bool indented_output = (count > 5) && false; // TODO!
	
	if(indented_output)
		addIndents(out, indent++);
	out << elm::color::Bold() << "[" << elm::color::NoBold();
	if(indented_output)
		out << elm::io::endl;
	bool first = true;
	for(typename C::Iterator elems(items); elems; elems++)
	{
		if(first)
		{
			if(indented_output)
				addIndents(out, indent);
			out << *elems;
			first = false;
		}
		else
		{
			out << ", ";
			if(indented_output)
			{
				out << elm::io::endl;
				addIndents(out, indent);
			}
			out << *elems;
		}
	}
	if(indented_output)
	{
		out << elm::io::endl;
		addIndents(out, --indent);
	}	
	return (out << elm::color::Bold() << "]" << elm::color::NoBold());
}

#endif