/*
 *	
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2006-2018, IRIT UPS.
 * 
 *	OTAWA is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	OTAWA is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OTAWA; if not, write to the Free Software 
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
#ifndef LOOP_BOUND_H
#define LOOP_BOUND_H

class LoopBound {
public:
	LoopBound() : n(-1), b(false) { }
	LoopBound(int bound) : n(bound), b(true) { }

	inline bool isBounded() const { return b; }
	inline int getBound() const { ASSERT(b); return n; }
	inline int operator*() const { return getBound(); }

	operator bool() const { return b; }
	inline void operator=(const LoopBound& lb) { n = lb.n, b = lb.b; }
	bool operator<(const LoopBound& lb) const { return (!(lb.b) && b) || (lb.b == b && n < lb.n); }  // better loop bound
	bool operator>(const LoopBound& lb) const { return (!b && lb.b) || (lb.b == b && n > lb.n); }  // worse loop bound

	friend inline io::Output& operator<<(io::Output& out, const LoopBound& lb) {
		if(lb.isBounded())
			return out << lb.getBound();
		else
			return out << "<unbounded>";
	}
	
private:
	int n;
	bool b; // bounded
};

#endif