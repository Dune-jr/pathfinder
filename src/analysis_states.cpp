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
 
/**
 * Collection of States
 */
#include "analysis_states.h"
#include "loop_bound.h"
#include "struct/var_maker.h"

// if this has n states and ss has m states, this will explode into a cartesian product of n*m states
void Analysis::States::apply(const States& ss, VarMaker& vm, bool local_sp, bool dbg, bool clear_path)
{
	int new_cap, m = this->count(), n = ss.count(), new_length = m * n;
	ASSERTP(n > 0, "TODO: handle empty states in entry")
	if(dbg && ss.first().getDetailedPath().hasAnEdge())
		DBGG("Applying " << Dim() << ss.first().getDetailedPath().lastEdge()->target()->cfg() << RCol()
			<< "(" << n << ") to " << m << " states, giving " << new_length << ".")

	for(new_cap = 1; new_cap < new_length; new_cap *= 2); // adjust to closest higher power of 2
	if(new_cap > this->s.capacity())
		this->s.grow(new_cap);

	// copy n-1 times 
	for(int i = 1; i < n; i++)
		for(int j = 0; j < m; j++)
			this->push((*this)[j]);

	// ss = [i1, i2, ..., in]
	// this = [x1, x2, x3,  x1, x2, x3,  ...] (n times)

	// apply n times
	Iter si(ss.s);	
	for(int i = 0; i < n; i++, si++)
		for(int j = 0; j < m; j++)
			(*this)[i*m + j].apply(*si, vm, local_sp, clear_path);

	// this = [x1*i1, x2*i1, x3*i1,  x1*i2, x2*i2, x3*i2, ...
	ASSERT(s.length() == new_length);
}

// TODO: optimize
/**
 * @brief apply this to s. local_sp is set to false. The state that we'll applied to is cleared
 */
void Analysis::States::appliedTo(const State& s, VarMaker& vm)
{
	if(this->count() == 0) // bottom state
	{
		DBGW("applying bottom state (" << *this << ") to " << s)
		return; // nothing to do
	}
	this->removeTautologies();
	for(States::Iter i(*this); i; i++)
		(this->s)[i].clearPreds(); // TODO: hack, without this there are fake IPs
	States x(this->count());
	x.push(s);
	x.apply(*this, vm, false, false, true); // no debug, no local sp, clear path
	*this = x;
}

void Analysis::States::minimize(VarMaker& vm, bool clean) const
{
	if(vm.isEmpty())
		return;
	VarCollector vc(vm.sizes());
	for(Vector<State>::Iter i(s); i; i++)
		s[i].collectTops(vc);
	DBG("Collecting Tops... " << vc)
	vm.shrink(vc, clean);
}

void Analysis::States::checkForSatisfiableSP(void) const
{
	Option<Constant> sp = none;
	for(States::Iter i(*this); i; i++)
	{
		if(i->getLocalVariables()[i->getSP()] && i->getLocalVariables()[i->getSP()]->kind() == CST)
		{
			Constant x = i->getLocalVariables()[i->getSP()]->toConstant();
			ASSERTP(!sp || *sp == x, "Merging definitely different SPs: " << *sp << "=/=" << x)
			sp = x;
		}
	}
}

void Analysis::States::printLoopBoundOf(const Operand *oi) const
{
	LoopBound n;
	for(Iter i(this->s); i; i++)
		if(LoopBound lb = s[i].getLoopBound(oi))
		{
			ASSERTP(!n || n == lb, "Loop bounds not matching!  " << n << " =/=" << lb)
			n = lb;
		}
	if(n)
	{
		if(dbg_flags & DBG_ONLY_LOOP_BOUNDS)
			elm::cout << "Loop bound found for " << oi->toIter().loop()->toBasic()->address() << ": " << n << endl;
		else
			DBGG(IRed << "Loop bound found for " << oi->toIter().loop()->toBasic()->address() << ": " << n)
	}
}
