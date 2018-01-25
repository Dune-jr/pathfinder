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
 
#include "z3_operand_visitor.h"

using z3::expr;

Z3OperandVisitor::Z3OperandVisitor(z3::context& c_, const expr& sp)
	: visited(false), c(c_), e(c.int_val(0)), sp_expr(sp) { }

expr Z3OperandVisitor::result()
{
	assert(visited);
	return e;
}

bool Z3OperandVisitor::visit(const class OperandConst& o)
{
	if(!o.value().isValid())
		return false; // fail
	if((o.value().isAbsolute()))
		e = c.int_val(o.value().val());
	else if(o.value().isRelativePositive())
		e = c.int_val(o.value().val()) + sp_expr;
	else if(o.value().isRelativeNegative())
		e = c.int_val(o.value().val()) - sp_expr;
	visited = true;
	return true;
}

bool Z3OperandVisitor::visit(const class OperandVar& o)
{
	elm::String label;
	if(!o.isTempVar())
		label = _ << "r" << o.addr(); // register
	else
		label = _ << "t" << -o.addr(); // temporary
	e = c.int_const(label.chars());
	visited = true;
	return true;
}

bool Z3OperandVisitor::visit(const class OperandMem& o)
{
	const Constant& addr = o.addr().value();
	assert(addr.isValid());
	int sp_factor = 0;
	if(addr.isRelative())
		sp_factor = (addr.isRelativePositive()) ? 1 : -1;
	elm::String label;
	switch(sp_factor)
	{	// these must be unique labels
		case -1:
			label = _ << "[-SP+" << addr.val() << "]";
			break;
		case 0:
			label = _ << "[" << addr.val() << "]";
			break;
		case +1:
			label = _ << "[SP+" << addr.val() << "]";
			break;
		default:
			assert(false);
	}
	e = c.int_const(label.chars());
	visited = true;
	return true;
}

bool Z3OperandVisitor::visit(const class OperandTop& o)
{
	ASSERTP(false, "v2 not supported by z3 yet");
}

bool Z3OperandVisitor::visit(const class OperandArith& o)
{
	if(!o.isComplete())
		return false; // fail
	if(!o.leftOperand().accept(*this))
		return false;
	expr expr_left = e;
	
	if(o.isBinary())
	{
		if(!o.rightOperand().accept(*this))
			return false;
		expr expr_right = e;
		switch(o.opr())
		{
			case ARITHOPR_ADD:
				e = expr_left + expr_right;			
				break;
			case ARITHOPR_SUB:
				e = expr_left - expr_right;
				break;
			case ARITHOPR_MUL:
				e = expr_left * expr_right;
				break;
			case ARITHOPR_MULH:
				e = (expr_left * expr_right) / c.int_val((0x100000000ull));
				break;
			case ARITHOPR_DIV:
				e = expr_left / expr_right;
				break;
			case ARITHOPR_MOD:
				e = expr_left - (expr_left / expr_right); // bit of a hack...
				break;
			default:
				assert(false);
		}
	}
	else
	{
		switch(o.opr())
		{
			case ARITHOPR_NEG:
				e = -expr_left;
				break;
			default:
				assert(false);
		}
	}		
	visited = true;
	return true;
}
