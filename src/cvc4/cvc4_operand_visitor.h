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
 
#ifndef _CVC4_CVC4_OPERAND_VISITOR_H
#define _CVC4_CVC4_OPERAND_VISITOR_H

#include "cvc4_variable_stack.h"

using namespace CVC4::kind;

class CVC4OperandVisitor : public OperandVisitor
{
public:
	CVC4OperandVisitor(CVC4::ExprManager &em, CVC4VariableStack &variables);
	CVC4::Expr result();		
	bool visit(const class OperandConst& o);
	bool visit(const class OperandVar& o);
	bool visit(const class OperandMem& o);
	bool visit(const class OperandTop& o);
	bool visit(const class OperandIter& o);
	bool visit(const class OperandArith& o);
	
private:
		   Kind_t getKind(arithoperator_t opr);
	inline Kind_t getKind(const OperandArith& o) { return getKind(o.opr()); }

	bool visited;
	CVC4::ExprManager &em;
	CVC4VariableStack &variables;
	CVC4::Expr expr;
};

#endif
