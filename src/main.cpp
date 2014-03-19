#include <elm/io.h>
#include <elm/io/Output.h>
#include <elm/types.h>
#include <otawa/otawa.h>

#include "predicate.h"

using namespace elm;

#define DEBUG_INFO() "[" << __FILE__ << ":" << __LINE__ << "] "
#define DBG(str) cout << DEBUG_INFO() << str << io::endl;
	
int main()
{
	ArithExpr e1 = ArithExpr(ARITHOPR_MUL, Operand(2), Operand((unsigned int)0x4000)); // e1 := 2 * @0x4000
	ArithExpr e1bis = ArithExpr(ARITHOPR_MUL, Operand((unsigned int)0x4000), Operand(2));
	Predicate p1 = Predicate(CONDOPR_EQ, Operand((unsigned int)0x4004), &e1); // p1 := @0x4004 = e1
	Predicate p2 = Predicate(CONDOPR_LE, Operand(-1), Operand((unsigned int)0x4000)); // p2 := -1 <= @0x4000
	
	/*
	DBG("--- Pretty Printing ---")
	DBG("-1:\t" << (unsigned int)0x4004)
	DBG("y = 2x:\t" << p1)
	DBG("x >= -1:\t" << p2 << io::endl)
	
	DBG("--- Equality over predicates ---")
	DBG("p1 = p1:\t" << (p1 == p1 ? "true" : "false"))
	DBG("p2 = p2:\t" << (p2 == p2 ? "true" : "false"))
	DBG("p1 = p2:\t" << (p1 == p2 ? "true" : "false") << io::endl)
	
	DBG("--- Equality over arithmetic expressions  ---")
	DBG("2x = 2x:\t" << (e1 == e1 ? "true" : "false"))
	DBG("2x = x*2:\t" << (e1 == e1bis ? "true" : "false") << io::endl)
	*/
	OperandConst o1 = OperandConst(12);
	OperandConst o2 = OperandConst(16);
	OperandVar o3 = OperandVar(0x1004);
	DBG("o1 = o1:\t" << (o1 == o1 ? "true" : "false"))
	DBG("o1 = o2:\t" << (o1 == o2 ? "true" : "false"))
	// DBG("o1 = o3:\t" << (o1 == (o3) ? "true" : "false")) // Doesn't work
	DBG("o1:\t" << o1)
	
	OperandArithExpr oae = OperandArithExpr(ARITHOPR_MUL, o1, o2);
	DBG(oae)
	
	
	return 0;
}
