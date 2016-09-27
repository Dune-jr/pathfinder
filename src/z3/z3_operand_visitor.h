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
	bool visit(const class OperandArith& o);
	
private:
	bool visited;
	z3::context &c;
	z3::expr e;
	const z3::expr &sp_expr;
};

#endif
