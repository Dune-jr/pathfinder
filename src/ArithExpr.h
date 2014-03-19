// This module is doomed to disappear

#ifndef _ARITHEXPR_H
#define _ARITHEXPR_H

#include <elm/io/Output.h>
#include "operand.h"

class ArithExpr
{
protected:
	arithoperator_t opr;
	Operand opd1;
	Operand opd2; // may be unused
	
public:
	ArithExpr();
	ArithExpr(arithoperator_t opr, Operand opd1, Operand opd2 = Operand());
	bool isUnary() const;
	bool isBinary() const;
	bool operator==(const ArithExpr& a) const;
	friend io::Output& operator<<(io::Output& out, const ArithExpr& a);
};

#endif 
