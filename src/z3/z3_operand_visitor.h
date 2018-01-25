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
 
#ifndef _Z3_Z3_OPERAND_VISITOR_H
#define _Z3_Z3_OPERAND_VISITOR_H

#include "../operand.h"
#include <z3++.h>

class Z3OperandVisitor : public OperandVisitor
{
public:
	Z3OperandVisitor(z3::context& c, const z3::expr& sp);
	z3::expr result();		
	bool visit(const class OperandConst& o);
	bool visit(const class OperandVar& o);
	bool visit(const class OperandMem& o);
	bool visit(const class OperandTop& o);
	bool visit(const class OperandArith& o);
	
private:
	bool visited;
	z3::context &c;
	z3::expr e;
	const z3::expr &sp_expr;
};

#endif
