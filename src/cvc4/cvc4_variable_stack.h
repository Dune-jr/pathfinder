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
 
#ifndef _CVC4_CVC4_VARIABLE_STACK_H
#define _CVC4_CVC4_VARIABLE_STACK_H

#include <elm/genstruct/AVLMap.h>
#include <cvc4/expr/expr_manager.h>
#include "../struct/operand.h"

using CVC4::Expr;
using elm::genstruct::AVLMap;

typedef enum
{
	DEFAULT = 0,
	VARIABLE_PREFIX, // "?"
	INITIAL_PREFIX,  // "r"
} stackmode_t;

class CVC4VariableStack
{
public:
	CVC4VariableStack(CVC4::ExprManager& em);
	Expr getExpr(CVC4::ExprManager& em, const OperandVar& o, stackmode_t mode = DEFAULT); // last parameter is to differentiate stuff like r0 = ?0 + 1 where ?0=r0_0
	Expr getExpr(CVC4::ExprManager& em, const OperandMem& o, stackmode_t mode = DEFAULT);
	Expr getExpr(CVC4::ExprManager& em, const OperandTop& o); // v2 only
	Expr getExpr(CVC4::ExprManager& em, const OperandIter& o); // v2 only
	inline Expr getExprSP() const { return expr_sp; }
	inline void setMode(stackmode_t mode) { def_mode = mode; }

private:
	inline char getRegChar(stackmode_t mode) const { return (mode == VARIABLE_PREFIX) ? '?' : 'r'; }
	inline char getMemChar(stackmode_t mode) const { return (mode == VARIABLE_PREFIX) ? ' ' : '0'; }
	inline char getRegId(t::int32 addr, stackmode_t mode) const { return (mode == VARIABLE_PREFIX) ? addr : -addr-1; }
	inline Constant getMemId(Constant addr, stackmode_t mode) const { return (mode == VARIABLE_PREFIX) ? addr : -addr; }

	AVLMap<t::int32, Expr> regmap; // registers
	AVLMap<Constant, Expr> memmap; // relative addresses in memory
	AVLMap<t::int32, Expr> topmap; // tops (v2 only)
	AVLMap<t::intptr, Expr> itermap; // tops (v2 only)
	const CVC4::Type integer; // Z
	Expr expr_sp;
	stackmode_t def_mode; // indicates what to default to (cannot be default)
};

#endif
