#include "arith.h"
#include "debug.h"

// this is minimal and a bit unoptimized
const Operand* Arith::add(DAG* dag, const Operand* a, const Operand* b)
{
	Option<Constant> av = a->evalConstantOperand(), bv = b->evalConstantOperand();
	if(av && bv)
		return dag->cst(*av+*bv);
	else if(av)
		return Arith::add(dag, *av, b);
	else if(bv)
		return Arith::add(dag, *bv, a);
	else //(!av && !bv)
		return dag->add(a, b);
}

const Operand* Arith::add(DAG* dag, Constant x, const Operand* y)
{
	if(x == 0)
		return y;
	if(y->kind() == ARITH)
	{
		const OperandArith& z = y->toArith();
		Option<Constant> av = z.leftOperand().evalConstantOperand();
		Option<Constant> bv = z.rightOperand().evalConstantOperand();
		if(z.opr() == ARITHOPR_ADD)
		{
			if(av) // x + (av + z2)
				return dag->add(z.right(), dag->cst(x + *av));
			if(bv) // x + (z1 + bv)
				return dag->add(z.left(), dag->cst(x + *bv));
		}
		else if(z.opr() == ARITHOPR_SUB)
		{
			if(av) // x + (av - z2)
				return dag->sub(dag->cst(x + *av), z.right());
			if(bv) // x + (z1 - bv)
				return dag->add(z.left(), dag->cst(x - *bv));
		}
	}
	return dag->add(y, dag->cst(x));
}

const Operand* Arith::sub(DAG* dag, const Operand* a, const Operand* b)
{
	Option<Constant> av = a->evalConstantOperand(), bv = b->evalConstantOperand();
	if(av && bv)
		return dag->cst(*av-*bv);
	else if(!av && !bv)
		return dag->sub(a, b);
	else if(av) // k - b
		return Arith::sub(dag, *av, b);
	else if(bv) // a - k = a+ (-k)
		return Arith::add(dag, -*bv, a);
	else
		return dag->sub(a, b);
}

const Operand* Arith::sub(DAG* dag, Constant x, const Operand* y)
{
	if(x == 0)
		return y;
	if(y->kind() == ARITH)
	{
		const OperandArith& z = y->toArith();
		Option<Constant> av = z.leftOperand().evalConstantOperand();
		Option<Constant> bv = z.rightOperand().evalConstantOperand();
		if(z.opr() == ARITHOPR_ADD)
		{
			if(av) // x - (av + z2)  ==>  [x-av] - z2
				return dag->sub(dag->cst(x - *av), z.right());
			if(bv) // x - (z1 + bv)  ==>  [x-bv] - z1
				return dag->sub(dag->cst(x - *bv), z.left());
		}
		else if(z.opr() == ARITHOPR_SUB)
		{
			if(av) // x - (av - z2)  ==>  [x-av] + z2
				return dag->add(dag->cst(x - *av), z.right());
			if(bv) // x - (z1 - bv)  ==>  [x+bv] - z1
				return dag->sub(dag->cst(x + *bv), z.left());
		}
	}
	return dag->sub(dag->cst(x), y);
}

const Operand* Arith::mul(DAG* dag, const Operand* a, const Operand* b)
{
	if(Option<Constant> av = a->evalConstantOperand())
		return Arith::mul(dag, b, *av);
	else if(Option<Constant> bv = b->evalConstantOperand())
		return Arith::mul(dag, a, *bv);
	else
		return dag->mul(a, b);
}

const Operand* Arith::mul(DAG* dag, const Operand* a, Constant c)
{
	if(c == 1)
		return a;
	if(c == 0)
		return dag->cst(0);
	Option<Constant> k;
	if(a->kind() == ARITH && a->toArith().opr() == ARITHOPR_MUL)
	{ // try to reduce a*c=(k*x)*c to [k*c]*x by exploring both operands of a and identifying one of them as a constant
		const OperandArith& aa = a->toArith();
		if(k = aa.left()->evalConstantOperand())
			return dag->mul(aa.right(), dag->cst(*k * c));
		else if(k = aa.rightOperand().evalConstantOperand())
			return dag->mul(aa.left(), dag->cst(*k * c));
		else
			return dag->mul(a, dag->cst(c));
	}
	else if(a->kind() == ARITH && a->toArith().opr() == ARITHOPR_DIV)
	{
		const OperandArith& aa = a->toArith();
		if(k = aa.left()->evalConstantOperand())
		{	// if a = k/x, k constant, reduce a*c=(k/x)*c to k*c/x
			DBG(color::IBlu() << "  Simplified (" << *k << "/" << *aa.right() << ")*" << c << " to " << *k*c << "/" << *aa.right())
			UNTESTED_CODE("simplification");
			return dag->div(dag->cst(*k * c), aa.right());
		}
		else if(k = aa.rightOperand().evalConstantOperand())
		{	// if a = x/k, k constant, try to reduce a*c=(x/k)*c to x/[k/c] or x*[c/k]
			return Arith::divmul(dag, aa.left(), *k, c);
		}
		else // give up
			return dag->mul(a, dag->cst(c));
	}
	else if(a->kind() == CST)
		return dag->cst(a->toConstant() * c);
	else
		return dag->mul(a, dag->cst(c));
}

/**
 * @brief Tries to simplify x/k * c
 * @return A (possibly) simplified form of x/k * c
 */
const Operand* Arith::divmul(DAG* dag, const Operand* x, Constant k, Constant c)
{
	if(c == k) // (x/k)*c = x
	{
		DBG(color::IBlu() << "  Simplified (" << *x << "/" << k << ")*" << c << " to " << *x << " [because " << k << " = " << c << "]")
		return x;
	}
	if(k % c == 0) // k = c*K ==> (x/k)*c = (x/(c*K))*c = x/K
	{
		const Constant K = k/c;
		DBG(color::IBlu() << "  Simplified (" << *x << "/" << k << ")*" << c << " to " << *x << "/" << K << " [because " << k << " = " << c << "*" << K << "]")
		return dag->div(x, dag->cst(K));
	}
	else if(c % k == 0) // c = k*K ==> (x/k)*c = (x/k)*k*K = x*K
	{
		const Constant K = c/k;
		DBG(color::IBlu() << "  Simplified (" << *x << "/" << k << ")*" << c << " to " << *x << "*" << K << " [because " << c << " = " << k << "*" << K << "]")
		return dag->mul(x, dag->cst(K));
	}
	else // give up
		return dag->mul(dag->div(x, dag->cst(k)), dag->cst(c));
}

/**
 * @brief Tries to simplify a/b
 * @return A (possibly) simplified form of x/k * c
 */
const Operand* Arith::div(DAG* dag, const Operand* a, const Operand* b)
{
	if(Option<Constant> bv = b->evalConstantOperand())
	{
		if(Option<Constant> av = a->evalConstantOperand())
			return dag->cst(*av / *bv); // 2 constants, easy simplification
		else
			return Arith::div(dag, a, *bv);
	}
	else
		return dag->div(a, b); // 0 constants, give up
}

const Operand* Arith::div(DAG* dag, const Operand* a, Constant c)
{
	if(c == 1) // ignore division by 0 problems
		return a;
	Option<Constant> k;
	if(a->kind() == ARITH && a->toArith().opr() == ARITHOPR_DIV)
	{
		const OperandArith& aa = a->toArith();
		if(k = aa.left()->evalConstantOperand()) // a/c = (k/x)/c = [k/c]/x
			return dag->div(dag->cst(*k/c), aa.right());
		else if(k = aa.right()->evalConstantOperand()) // a/c = (x/k)/c = x/[k*c]
			return dag->div(aa.left(), dag->cst(*k * c));
		else
			return dag->div(a, dag->cst(c));
	}
	else if(a->kind() == ARITH && a->toArith().opr() == ARITHOPR_MUL)
	{	// a = (a1*a2)/c
		const OperandArith& aa = a->toArith();
		if(k = aa.left()->evalConstantOperand()) // a/c = (k*x)/c = (x*k)/c
			return Arith::muldiv(dag, aa.right(), *k, c);
		else if(k = aa.right()->evalConstantOperand()) // a/c = (x*k)/c
			return Arith::muldiv(dag, aa.left(), *k, c);
		else
			return dag->div(a, dag->cst(c));
	}
	else
		return dag->div(a, dag->cst(c));
}

/**
 * @brief Tries to simplify x*k / c
 * @return A (possibly) simplified form of x*k / c
 */
const Operand* Arith::muldiv(DAG* dag, const Operand* x, Constant k, Constant c)
{
	if(c == k) // (x*k)/c = x
	{
		DBG(color::IBlu() << "  Simplified (" << *x << "*" << k << ")/" << c << " to " << *x << " [because " << k << " = " << c << "]")
		return x;
	}
	if(k % c == 0) // k = c*K ==> (x*k)/c = (x*c*K)/c = x*K
	{
		const Constant K = k/c;
		DBG(color::IBlu() << "  Simplified (" << *x << "*" << k << ")/" << c << " to " << *x << "*" << K << " [because " << k << " = " << c << "*" << K << "]")
		return dag->mul(x, dag->cst(K));
	}
	else if(c % k == 0) // c = k*K ==> (x*k)/c = (x/k)/(k*K) = x/K
	{
		const Constant K = c/k;
		DBG(color::IBlu() << "  Simplified (" << *x << "*" << k << ")/" << c << " to " << *x << "/" << K << " [because " << c << " = " << k << "*" << K << "]")
		return dag->div(x, dag->cst(K));
	}
	else // give up
		return dag->mul(dag->div(x, dag->cst(k)), dag->cst(c));
}
