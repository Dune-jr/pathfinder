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
 
#ifndef _LABELLED_PREDICATE_H
#define _LABELLED_PREDICATE_H

#include <elm/avl/Set.h>
#include <otawa/cfg/Edge.h>
#include "predicate.h"

using namespace otawa;
using namespace elm::avl;
// using namespace elm::genstruct;

// LabelledPredicate class
class LabelledPredicate {
public:
	LabelledPredicate(const Predicate& pred, const Set<Edge*>& labels); // TODO!!!
	LabelledPredicate(const LabelledPredicate& lp);
	inline Predicate& pred()
		{ return _pred; }
	inline const Predicate& pred() const
		{ return _pred; }
	inline const Set<Edge*>& labels() const
		{ return _labels; }
	inline Set<Edge*>& labels()
		{ return _labels; }
	inline bool updatePred(DAG& dag, const Operand* opd, const Operand* expr)
		{ return _pred.update(dag, opd, expr); }
	inline void addLabel(Edge* label)
		{ _labels += label; }
	inline void addLabels(const Set<Edge*>& labels)
		{ _labels.addAll(labels); }
	inline void collectTops(VarCollector& vc) const
		{ _pred.leftOperand().collectTops(vc); _pred.rightOperand().collectTops(vc); }
	inline bool involves(const Operand* o) const
		{ return _pred.involves(o); }
	inline int involvesOperand(const Operand* o) const
		{ return _pred.involvesOperand(*o); }

	LabelledPredicate& operator=(const LabelledPredicate& lp);
	bool operator==(const LabelledPredicate& lp) const;
	friend io::Output& operator<<(io::Output& out, const LabelledPredicate& lv)
		{ return lv.print(out); }
	inline const LabelledPredicate* operator->(void) const
		{ return this; }
private:
	io::Output& print(io::Output& out) const;

	Predicate _pred;
	Set<Edge*> _labels; // Analysis::Path
};

#endif
