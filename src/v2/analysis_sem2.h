#ifndef _ANALYSIS_SEM2_H
#define _ANALYSIS_SEM2_H

class Analysis::State::SemanticParser
{	// TODO: make it act like an Iterator, and make a SemanticParser for v1 too
public:
	SemanticParser(State& s, VarMaker& vm) : s(s), vm(vm), lvars(s.lvars), dag(*s.dag), last_condition(sem::NOP) { }
	int process(const sem::PathIter& seminsts);
	// int process(sem::inst inst);
	inline void set(const OperandVar& var, const Operand* expr, bool set_updated = true) { s.set(var, expr, set_updated); }
	inline void setMem(Constant addr, const Operand* expr) { s.setMem(addr, expr); }
	int store(OperandVar addr, const Operand* b);
	void scratch(const OperandVar& var);
	static bool affectsRegister(t::uint16 op);
	static bool affectsMemory(t::uint16 op);
	inline const Operand* getPtr(t::int32 var_id) const { return s.getPtr(var_id); }
	inline const sem::inst& lastCond() const { return last_condition; }
	inline const Operand* new_top() const { return vm.new_top(); }

private:
	State& s;
	VarMaker& vm;
	const LocalVariables& lvars;
	DAG& dag;
	sem::inst last_condition;
};

#endif