/**
 * Smart arith module
 */

#ifndef _ARITH_H
#define _ARITH_H

#include "struct/DAG.h"
#include "struct/operand.h"

class Arith
{
public:
	static const Operand* add	(DAG& dag, const Operand* a, const Operand* b);
	static const Operand* add	(DAG& dag, Constant x, 		 const Operand* y);
	static const Operand* addmul(DAG& dag, const Operand* x, const Operand* a, const Operand* b);
	static const Operand* sub	(DAG& dag, const Operand* a, const Operand* b);
	static const Operand* sub	(DAG& dag, Constant x, 		 const Operand* y);
	static const Operand* submul(DAG& dag, const Operand* a, const Operand* b, const Operand* x);
	static const Operand* mul	(DAG& dag, const Operand* a, const Operand* b);
	static const Operand* mul	(DAG& dag, const Operand* a, Constant c);
	static const Operand* div	(DAG& dag, const Operand* a, const Operand* b);
	static const Operand* div	(DAG& dag, const Operand* a, Constant c);
	static const Operand* divmul(DAG& dag, const Operand* x, Constant k, Constant c);
	static const Operand* muldiv(DAG& dag, const Operand* x, Constant k, Constant c);
	static const Operand* autoOp(DAG& dag, arithoperator_t op, const Operand *arg1, const Operand *arg2) {
		switch(op) {
			case ARITHOPR_ADD: return add(dag, arg1, arg2);
			case ARITHOPR_SUB: return sub(dag, arg1, arg2);
			case ARITHOPR_MUL: return mul(dag, arg1, arg2);
			case ARITHOPR_DIV: return div(dag, arg1, arg2);
			default: return dag.autoOp(op, arg1, arg2);
		}
	}

private:
	Arith() { }
};

#endif // _ARITH_H