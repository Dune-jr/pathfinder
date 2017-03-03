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
		DBG("  " << color::IBlu() << "Introducing " << *r)
		// ASSERT(tops.length() < 40000);
		tops.push(r);
		return r;
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