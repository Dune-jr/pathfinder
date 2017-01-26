#ifndef _COMPOSITOR_H
#define _COMPOSITOR_H

#include <elm/genstruct/HashTable.h>
#include "analysis_state.h"
#include "arith.h"
#include "DAG.h"
#include "local_variables.h"
#include "operand.h"

// Compositor: a class for State composition
class Compositor : public OperandEndoVisitor
{
	typedef HashTable<Constant, const Operand*, ConstantHash> mem_t;
public:
	Compositor(const Analysis::State& s)
		: dag(s.getDag()), sp(s.getSP()), lvars(s.getLocalVariables()), mem(s.getMemoryTable()) { }
	// Compositor(DAG& dag, const LocalVariables& lv, const mem_t& m) : lvars(lv), mem(m), app(NULL), r(NULL) { }

	const Operand* visit(const class OperandConst& g) {
		ASSERT(g.value().isValid());
		if(g.value().isAbsolute())
			return &g;
		else if(g.value().isRelativePositive())
			return Arith::add(&dag, lvars[sp], dag.cst(g.value().val()));
		else
			return Arith::sub(&dag, dag.cst(g.value().val()), lvars[sp]);
	}
	const Operand* visit(const class OperandVar& g)   { return lvars[g] ? lvars[g] : &g; }
	const Operand* visit(const class OperandMem& g)   { return mem.exists(g.addr()) ? mem[g.addr()] : &g; }
	const Operand* visit(const class OperandTop& g)   { return &g; } // this should be handled by allocating a new top, otherwise we'll get problems
	const Operand* visit(const class OperandArith& g)
		{ return Arith::autoOp(&dag, g.opr(), g.leftOperand().accept(*this), g.isBinary() ? g.rightOperand().accept(*this) : NULL); }

private:
	DAG &dag;
	OperandVar sp;
	const LocalVariables &lvars;
	const mem_t &mem;
};

#endif
