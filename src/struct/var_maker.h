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
 
#ifndef STRUCT_VAR_MAKER_H
#define STRUCT_VAR_MAKER_H

#include <elm/util/LockPtr.h>
#include <elm/data/Vector.h>
#include "operand.h"
#include "var_collector.h"

// forall i, t[i] = T_(i+start)!
class VarMaker : public elm::Lock {
	typedef Vector<OperandTop *> tops_t;
public:
	VarMaker() : start(0) { }
	VarMaker(const VarMaker& vm) : tops(vm.tops.length()), start(vm.start) {
		crash();
		for(tops_t::Iter i(vm.tops); i; i++)
			tops.push(new OperandTop(**i));
	}
	~VarMaker() { // this should be called at the end of the analysis
		for(tops_t::Iter topi(tops); topi; topi++)
			delete *topi;
	}
	inline bool isEmpty(void) const {
		return tops.isEmpty();
	}
	inline int length(void) const {
		return start + tops.length();
	}
	inline elm::Pair<int, int> sizes(void) const {
		return pair(tops.length(), start);
	}
	inline const Operand* new_top(void) {
		OperandTop* r = new OperandTop(length());
		ASSERTP(length() >= 0, "OperandTop ids overflowing")
		DBG("  " << color::Blu() << "Introducing " << *r)
		// ASSERT(tops.length() < 40000);
		tops.push(r);
		return r;
	}
	inline VarMaker& operator=(const VarMaker& vm) {
		tops = vm.tops;
		start = vm.start;
		return *this;
	}
	friend inline io::Output& operator<<(io::Output& out, const VarMaker& vm) {
		return vm.print(out);
	}
	void import(const VarMaker& vm);
	void shrink(const VarCollector& bv, bool clean);

private:
	tops_t tops;
	int start;

	void scale(int offset) {
		for(tops_t::Iter i(tops); i; i++)
			i->scale(+offset);
	}
	io::Output& print(io::Output& out) const;
};

#endif