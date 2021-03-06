/*
 *	Smart arith module
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
 
#include "arith.h"
#include "DAG.h"
#include "../debug.h"

/**
 * @class Arith
 * @author Jordy Ruiz
 * @brief Handles simplifications of arithmetic expressions, using integer algebraic properties
 */

// this is minimal and a bit unoptimized
const Operand* Arith::add(DAG& dag, const Operand* a, const Operand* b)
{
	if(a == Top || b == Top) return Top;
	Option<Constant> av = a->evalConstantOperand(), bv = b->evalConstantOperand();
	if(av && bv)
		return dag.cst(*av+*bv);
	else if(av)
		return Arith::add(dag, *av, b);
	else if(bv)
		return Arith::add(dag, *bv, a);
	else if(a->kind() == ARITH && a->toArith().opr() == ARITHOPR_MUL)
		return Arith::addmul(dag, b, a->toArith().left(), a->toArith().right());
	else if(b->kind() == ARITH && b->toArith().opr() == ARITHOPR_MUL)
		return Arith::addmul(dag, a, b->toArith().left(), b->toArith().right());
	else //(!av && !bv)
		return dag.add(a, b);
}

const Operand* Arith::add(DAG& dag, Constant x, const Operand* y)
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
				return dag.add(z.right(), dag.cst(x + *av));
			else if(bv) // x + (z1 + bv)
				return dag.add(z.left(), dag.cst(x + *bv));
			else
			{
				elm::Pair<const Operand*, Constant> p = z.extractAdditiveConstant(dag);
				ASSERTP(p.fst, "all constant should've been handled by evalConstantOperand")
				if(x+p.snd == 0)
					return p.fst;
				else
					return dag.add(p.fst, dag.cst(x+p.snd));
			}
		}
		else if(z.opr() == ARITHOPR_SUB)
		{
			if(av) // x + (av - z2)
				return dag.sub(dag.cst(x + *av), z.right());
			if(bv) // x + (z1 - bv)
				return dag.add(z.left(), dag.cst(x - *bv));
		}
	}
	return dag.add(y, dag.cst(x));
}

// x + y * z
const Operand* Arith::addmul(DAG& dag, const Operand* x, const Operand* a, const Operand* b)
{
	if(x == a) // x + x * b
		return dag.mul(Arith::add(dag, x, dag.cst(1)), b);
	if(x == b) // x + a * x
		return dag.mul(a, Arith::add(dag, x, dag.cst(1)));
	return dag.add(x, dag.mul(a, b));
}

const Operand* Arith::sub(DAG& dag, const Operand* a, const Operand* b)
{
	if(a == Top || b == Top) return Top;
	Option<Constant> av = a->evalConstantOperand();
	Option<Constant> bv = b->evalConstantOperand();
	if(a == b)
		return dag.cst(0);
	else if(av && bv)
		return dag.cst(*av-*bv);
	else if(av && !bv) // k - b
		return Arith::sub(dag, *av, b);
	else if(!av && bv) // a - k = a+ (-k)
		return Arith::add(dag, -*bv, a);
	else if(a->kind() == ARITH && a->toArith().opr() == ARITHOPR_MUL)
		return Arith::submul(dag, a->toArith().left(), a->toArith().right(), b);
	else
		return dag.sub(a, b);
}

const Operand* Arith::sub(DAG& dag, Constant x, const Operand* y)
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
				return dag.sub(dag.cst(x - *av), z.right());
			if(bv) // x - (z1 + bv)  ==>  [x-bv] - z1
				return dag.sub(dag.cst(x - *bv), z.left());
		}
		else if(z.opr() == ARITHOPR_SUB)
		{
			if(av) // x - (av - z2)  ==>  [x-av] + z2
				return dag.add(dag.cst(x - *av), z.right());
			if(bv) // x - (z1 - bv)  ==>  [x+bv] - z1
				return dag.sub(dag.cst(x + *bv), z.left());
		}
	}
	return dag.sub(dag.cst(x), y);
}

// a * b - x
const Operand* Arith::submul(DAG& dag, const Operand* a, const Operand* b, const Operand* x)
{
	if(x == a) // x*b - x
		return dag.mul(Arith::sub(dag, x, dag.cst(1)), b);
	if(x == b) // a*x - x
		return dag.mul(a, Arith::sub(dag, x, dag.cst(1)));
	return dag.sub(dag.mul(a, b), x);
}

const Operand* Arith::mul(DAG& dag, const Operand* a, const Operand* b)
{
	if(a == Top || b == Top) return Top;
	if(Option<Constant> av = a->evalConstantOperand())
		return Arith::mul(dag, b, *av);
	else if(Option<Constant> bv = b->evalConstantOperand())
		return Arith::mul(dag, a, *bv);
	else
		return dag.mul(a, b);
}

const Operand* Arith::mul(DAG& dag, const Operand* a, Constant c)
{
	if(c == 1)
		return a;
	if(c == 0)
		return dag.cst(0);
	Option<Constant> k;
	if(a->kind() == ARITH && a->toArith().opr() == ARITHOPR_MUL)
	{ // try to reduce a*c=(k*x)*c to [k*c]*x by exploring both operands of a and identifying one of them as a constant
		const OperandArith& aa = a->toArith();
		if(k = aa.left()->evalConstantOperand())
			return dag.mul(aa.right(), dag.cst(*k * c));
		else if(k = aa.rightOperand().evalConstantOperand())
			return dag.mul(aa.left(), dag.cst(*k * c));
		else
			return dag.mul(a, dag.cst(c));
	}
	else if(a->kind() == ARITH && a->toArith().opr() == ARITHOPR_DIV)
	{
		const OperandArith& aa = a->toArith();
		if(k = aa.left()->evalConstantOperand())
		{	// if a = k/x, k constant, reduce a*c=(k/x)*c to k*c/x
			DBG(color::IBlu() << "  Simplified (" << *k << "/" << *aa.right() << ")*" << c << " to " << *k*c << "/" << *aa.right())
			UNTESTED_CODE("simplification");
			return dag.div(dag.cst(*k * c), aa.right());
		}
		else if(k = aa.rightOperand().evalConstantOperand())
		{	// if a = x/k, k constant, try to reduce a*c=(x/k)*c to x/[k/c] or x*[c/k]
			return Arith::divmul(dag, aa.left(), *k, c);
		}
		else // give up
			return dag.mul(a, dag.cst(c));
	}
	else if(a->kind() == ARITH && a->toArith().opr() == ARITHOPR_ADD)
	{
		const OperandArith& aa = a->toArith();
		return dag.add(Arith::mul(dag, aa.left(), c), Arith::mul(dag, aa.right(), c));
	}
	else if(a->kind() == ARITH && a->toArith().opr() == ARITHOPR_SUB)
	{
		const OperandArith& aa = a->toArith();
		return dag.sub(Arith::mul(dag, aa.left(), c), Arith::mul(dag, aa.right(), c));
	}
	else if(a->kind() == CST)
		return dag.cst(a->toConstant() * c);
	else
		return dag.mul(a, dag.cst(c));
}

/**
 * @brief Tries to simplify x/k * c
 * @return A (possibly) simplified form of x/k * c
 */
const Operand* Arith::divmul(DAG& dag, const Operand* x, Constant k, Constant c)
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
		return dag.div(x, dag.cst(K));
	}
	else if(c % k == 0) // c = k*K ==> (x/k)*c = (x/k)*k*K = x*K
	{
		const Constant K = c/k;
		DBG(color::IBlu() << "  Simplified (" << *x << "/" << k << ")*" << c << " to " << *x << "*" << K << " [because " << c << " = " << k << "*" << K << "]")
		return dag.mul(x, dag.cst(K));
	}
	else // give up
		return dag.mul(dag.div(x, dag.cst(k)), dag.cst(c));
}

/**
 * @brief Tries to simplify a/b
 * @return A (possibly) simplified form of x/k * c
 */
const Operand* Arith::div(DAG& dag, const Operand* a, const Operand* b)
{
	if(a == Top || b == Top) return Top;
	if(Option<Constant> bv = b->evalConstantOperand())
	{
		if(Option<Constant> av = a->evalConstantOperand())
			return dag.cst(*av / *bv); // 2 constants, easy simplification
		else
			return Arith::div(dag, a, *bv);
	}
	else
		return dag.div(a, b); // 0 constants, give up
}

const Operand* Arith::div(DAG& dag, const Operand* a, Constant c)
{
	if(c == 1) // ignore division by 0 problems
		return a;
	Option<Constant> k;
	if(a->kind() == ARITH && a->toArith().opr() == ARITHOPR_DIV)
	{
		const OperandArith& aa = a->toArith();
		if(k = aa.left()->evalConstantOperand()) // a/c = (k/x)/c = [k/c]/x
			return dag.div(dag.cst(*k/c), aa.right());
		else if(k = aa.right()->evalConstantOperand()) // a/c = (x/k)/c = x/[k*c]
			return dag.div(aa.left(), dag.cst(*k * c));
		else
			return dag.div(a, dag.cst(c));
	}
	else if(a->kind() == ARITH && a->toArith().opr() == ARITHOPR_MUL)
	{	// a = (a1*a2)/c
		const OperandArith& aa = a->toArith();
		if(k = aa.left()->evalConstantOperand()) // a/c = (k*x)/c = (x*k)/c
			return Arith::muldiv(dag, aa.right(), *k, c);
		else if(k = aa.right()->evalConstantOperand()) // a/c = (x*k)/c
			return Arith::muldiv(dag, aa.left(), *k, c);
		else
			return dag.div(a, dag.cst(c));
	}
	else
		return dag.div(a, dag.cst(c));
}

/**
 * @brief Tries to simplify x*k / c
 * @return A (possibly) simplified form of x*k / c
 */
const Operand* Arith::muldiv(DAG& dag, const Operand* x, Constant k, Constant c)
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
		return dag.mul(x, dag.cst(K));
	}
	else if(c % k == 0) // c = k*K ==> (x*k)/c = (x/k)/(k*K) = x/K
	{
		const Constant K = c/k;
		DBG(color::IBlu() << "  Simplified (" << *x << "*" << k << ")/" << c << " to " << *x << "/" << K << " [because " << c << " = " << k << "*" << K << "]")
		return dag.div(x, dag.cst(K));
	}
	else // give up
		return dag.mul(dag.div(x, dag.cst(k)), dag.cst(c));
}

const Operand* Arith::autoOp(DAG& dag, arithoperator_t op, const Operand *arg1, const Operand *arg2)
{
	switch(op) {
		case ARITHOPR_ADD: return add(dag, arg1, arg2);
		case ARITHOPR_SUB: return sub(dag, arg1, arg2);
		case ARITHOPR_MUL: return mul(dag, arg1, arg2);
		case ARITHOPR_DIV: return div(dag, arg1, arg2);
		default: return dag.autoOp(op, arg1, arg2);
	}
}