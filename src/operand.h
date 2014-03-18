#ifndef _OPERAND_H
#define _OPERAND_H

#include <iostream>

enum operand_kind_t
{
	OPERAND_UNDEF, // Undefined
	CONST,         // Constant
	VAR,           // Variable
	EXPR,          // Arithmetic Expression
};

class ArithExpr;



// Operands of predicates (can be constant or variables)
class Operand
{
protected:
	operand_kind_t kind;
	void* value;
	// We may have to add a "label" for the case where it is a variable, in the future
	
public:
	Operand();
	Operand(operand_kind_t kind, void* value);
	Operand(int value); // Constant
	Operand(unsigned int value); // Variable
	Operand(ArithExpr* e); // An arithmetic expression
	// TODO: we are forced here to use Operand* because Operand is incomplete type (not yet defined...)
	//       should we do it this way or the other way around? (we include arithexpr from this file)
	
	bool operator==(const Operand& p) const;
	friend std::ostream& operator<<(std::ostream& out, const Operand& o);
};

#endif
