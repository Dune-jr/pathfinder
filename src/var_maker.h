#ifndef VAR_MAKER_H
#define VAR_MAKER_H

class VarMaker : public elm::Lock {
	typedef Vector<OperandTop *> tops_t;
public:
	VarMaker() { }
	VarMaker(const VarMaker& vm) : tops(vm.tops.length()) {
		for(tops_t::Iter i(vm.tops); i; i++)
			tops.push(new OperandTop(**i));
	}
	~VarMaker() {
		for(tops_t::Iter atop(tops); atop; atop++)
			delete *atop;
	}
	void transfer(VarMaker& vm) {
		vm.scale(+tops.length());
		tops.addAll(vm.tops);
		vm.tops.clear();
	}

	const Operand* new_top(void) {
		ASSERT(tops.length() < 400);
		ASSERT(tops.length() < 4000);
		ASSERT(tops.length() < 40000);
		ASSERT(tops.length() < 400000);
		ASSERT(tops.length() < 4000000);
		OperandTop* r = new OperandTop(tops.length());
		tops.push(r);
		return r;
	}

private:
	tops_t tops;

	void scale(int offset) {
		for(tops_t::Iter i(tops); i; i++)
			i->scale(+offset);
	}
};

/*
 * VarMaker vm_f;
 * CFG* g;
 * process(g);
 * VarMaker vm_g_copy(CFG_VARS(g));
 * //vm_f.append(vm_g_copy); // loads the responsibility for all the pointers
 * //vm_g_copy->clear(); // clears the vector without deleting
 * vm_f.transfer(vm_g_copy);
 * //delete vm_g_copy;
 */

#endif