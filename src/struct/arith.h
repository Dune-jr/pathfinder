/**
 * Smart arith module
 */

#ifndef _ARITH_H
#define _ARITH_H

#include "operand.h"

class DAG;

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
	
	static const Operand* autoOp(DAG& dag, arithoperator_t op, const Operand *arg1, const Operand *arg2);

private:
	Arith() { }
};

#endif // _ARITH_H