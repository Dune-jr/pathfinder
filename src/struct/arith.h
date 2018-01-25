/*
 *	
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