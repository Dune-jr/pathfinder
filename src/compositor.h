#ifndef _COMPOSITOR_H
#define _COMPOSITOR_H

#include <elm/genstruct/HashTable.h>
#include "analysis_state.h"
#include "arith.h"
#include "struct/DAG.h"
#include "struct/local_variables.h"
#include "struct/operand.h"
#include "struct/predicate.h"

// Compositor: a class for State composition
class Compositor : public OperandEndoVisitor
{
	typedef HashTable<Constant, const Operand*, ConstantHash> mem_t;
public:
	Compositor(const Analysis::State& s, bool sp_is_local)
		: dag(s.getDag()), sp(NULL), lvars(s.getLocalVariables()), mem(s.getMemoryTable())
	{
		if(sp_is_local)
			sp = lvars[s.getSP()] ? lvars[s.getSP()] : dag.var(s.getSP());
		else
			sp = dag.cst(SP);
	}
	const Operand* visit(const class OperandConst& g) {
		ASSERT(g.value().isValid());
		if(g.value().isAbsolute())
			return &g;
		else if(g.value().isRelativePositive())
			return Arith::add(dag, sp, dag.cst(g.value().val()));
		else
			return Arith::sub(dag, dag.cst(g.value().val()), sp);
	}
	const Operand* visit(const class OperandVar& g)  { return lvars[g] ? lvars[g] : &g; }
	const Operand* visit(const class OperandMem& g)  { return mem.exists(g.addr()) ? mem[g.addr()] : &g; }
	const Operand* visit(const class OperandTop& g)  { return &g; } // this will be handled elsewhere by allocating a new top
	const Operand* visit(const class OperandIter& g) { ASSERTP(false, "OperandIter found by the Compositor"); }
	const Operand* visit(const class OperandArith& g)
		{ return Arith::autoOp(dag, g.opr(), g.leftOperand().accept(*this), g.isBinary() ? g.rightOperand().accept(*this) : NULL); }
	inline Predicate visit(const Predicate& p) { return Predicate(p.opr(), p.left()->accept(*this), p.right()->accept(*this)); }

private:
	DAG &dag;
	const Operand* sp;
	const LocalVariables &lvars;
	const mem_t &mem;
};

#endif
