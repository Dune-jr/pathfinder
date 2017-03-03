/**
 * Collection of States
 */
#include "analysis_states.h"
#include "struct/var_maker.h"

// if this has n states and ss has m states, this will explode into a cartesian product of n*m states
void Analysis::States::apply(const States& ss, VarMaker& vm, bool local_sp, bool dbg)
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
			(*this)[i*m + j].apply(*si, vm, local_sp);

	// this = [x1*i1, x2*i1, x3*i1,  x1*i2, x2*i2, x3*i2, ...
	ASSERT(s.length() == new_length);
}

// TODO: optimize
/**
 * @brief apply this to s. local_sp is set to false
 */
void Analysis::States::appliedTo(const State& s, VarMaker& vm)
{
	States x(this->count());
	x.push(s);
	x.apply(*this, vm, false, false); // no debug, no local sp
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

