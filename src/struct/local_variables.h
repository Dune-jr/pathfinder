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
 
#ifndef _LOCAL_VARIABLES_H
#define _LOCAL_VARIABLES_H

// #include <elm/avl/Set.h>
#include <elm/data/SortedList.h> // ListSet
#include <elm/data/ListSet.h>
#include <elm/util/BitVector.h>
#include <otawa/cfg/Edge.h>
#include "constant.h"
#include "operand.h"
#include "../pretty_printing.h"

// using namespace elm::genstruct; 
using otawa::Edge;
using elm::ListSet;

/*
	The following conventions are made:
	* size=0: object is invalid
	* o[x] is NULL: x | x, identity (variable has not changed since start of program)
*/
class LocalVariables {
public:
	typedef ListSet<Edge*> labels_t;

	LocalVariables() : size(0), thresold(0), o(NULL), l(NULL), u() { } // invalid
	LocalVariables(DAG& dag, short max_tempvars, short max_registers) : size(max_tempvars + max_registers), thresold(max_registers),
		o(new const Operand*[size]), l(new labels_t*[size]), u(size) {
		array::clear(l, size); // array::clear call is a fast one because it's an array of pointers
		array::clear(o, size); // all operands to NULL - aka Identity
		// array::clear(u, size); // all updates to false
	}
	LocalVariables(const LocalVariables& lv) : size(lv.size), thresold(lv.thresold),
		o(size ? new const Operand*[size] : NULL), l(size ? new labels_t*[size] : NULL), u() {
		if(size) {
			array::clear(l, size);
			u.resize(size);
			copy(lv);
		}
	}
	~LocalVariables() {
		delete[] o;
		// delete[] u;
		for(int i = 0; i < size; i++)
			delete l[i];
		delete[] l;
	}

private:
	void label(t::int32 id, Edge* e)
		{ return;
		  if(!l[id]) l[id] = new labels_t();
		  l[id]->add(e); }
	void label(t::int32 id, const labels_t& labs)
		{ return;
		  if(!l[id]) l[id] = new labels_t(labs);
		  else for(labels_t::Iter i(labs); i; i++) l[id]->add(*i); }

	inline int getIndex(t::int32 var_id) const
		{ return var_id >= 0 ? var_id : thresold-var_id-1; } // tempvars id start at -1 for t1 and so forth
public: // these are used in some methods of State like accel
	inline int getIndex(OperandVar var) const
		{ return getIndex(var.addr()); }
	inline t::int32 getId(int index) const
		{ return index < thresold ? index : thresold-index-1; }

public:
	inline bool isValid() const
		{ return size && thresold; }
	inline short getSize() const
		{ return size; }
	inline short maxRegisters() const
		{ return thresold; }
	inline bool isConst(OperandVar var) const
		{ Operand const* k = (*this)[var]; return k && k->kind() == CST; }
	inline const labels_t& labels(OperandVar var) const
		{ return l[getIndex(var)] ? *l[getIndex(var)] : Single<labels_t>::_; }
	inline void label(OperandVar var, Edge* e)
		{ label(getIndex(var), e); }
	inline void label(OperandVar var, const labels_t& labs)
		{ label(getIndex(var), labs); }
	inline void clearLabels(OperandVar var)
		{ delete l[getIndex(var)]; l[getIndex(var)] = NULL; }
	inline bool isUpdated(OperandVar var)
		{ return u[getIndex(var)]; }
	inline void markAsUpdated(OperandVar var)
		{ u.set(getIndex(var)); }
	inline void resetUpdatedMarks()
		{ u.clear(); }
	inline void resetTempVars()
		{ array::clear(o+thresold, size-thresold); }
	void onEdge(Edge* e);
	void merge(const LocalVariables& lv); // this = this ∩ lv

	LocalVariables& operator=(const LocalVariables& lv);
	inline bool operator==(const LocalVariables& lv) const // only compares operands!
		{ return array::cmp(o, lv.o, size) == 0; }
	inline bool operator!=(const LocalVariables& lv) const
		{ return !(*this == lv); }
	inline const Operand& operator()(OperandVar var) const // returns concrete value
		{ return (*this)[var] ? *(*this)[var] : var; }
	inline Operand const*& operator[](OperandVar var)
		{ return o[getIndex(var)]; }
	inline Operand const* operator[](OperandVar var) const
		{ return o[getIndex(var)]; }
	inline friend io::Output& operator<<(io::Output& out, const LocalVariables& lv)
		{ return lv.print(out); }

	// Iter class
	class Iter: public PreIterator<Iter, OperandVar> {
	public:
		inline Iter(const LocalVariables& lv) : n(0), size(lv.size), thresold(lv.thresold) { }
		inline Iter(const Iter& i) : n(i.n), size(i.size), thresold(i.thresold) { }
		inline bool ended(void) const { return n >= size; }
		inline OperandVar item(void) const { return OperandVar(n < thresold ? n : thresold-n-1); }
		inline void next(void) { n++; }
		inline const Iter* operator->(void) const { return this; }
	private:
		short n;
		short size;
		short thresold;
	};

private:
	short size; // size of arrays. save space with shorts
	short thresold; // == max_registers
	Operand const** o; // operands
	labels_t** l; // labels
	BitVector  u; // updated

	void copy(const LocalVariables& lv) {
		ASSERT(isValid());
		array::copy(o, lv.o, size);
		u = lv.u;
		for(int i = 0; i < size; i++) {
			delete l[i];
			l[i] = (lv.l[i] != NULL) ? new labels_t(*lv.l[i]) : NULL;		
		}
	}
	io::Output& print(io::Output& out) const;
}; // LocalVariables class

#endif

