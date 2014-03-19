#include <elm/io.h>
#include <elm/io/Output.h>
#include <elm/types.h>
#include <otawa/otawa.h>

#include "predicate.h"

using namespace elm;

#define DBG_INFO() "[" << __FILE__ << ":" << __LINE__ << "] "
#define DBG(str) cout << DBG_INFO() << str << io::endl;
#define DBG_TEST(tested_cond, expected_cond) \
	(tested_cond == expected_cond ? "\033[32m" : "\033[31m") << \
	(tested_cond ? "true" : "false") << "\033[0m"
	
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
	
	DBG("--- Equality over Operands  ---")
	OperandConst o1 = OperandConst(12);
	OperandConst o1bis = OperandConst(12);
	OperandConst o2 = OperandConst(16);
	OperandVar o3 = OperandVar(0x1004);
	OperandVar o4 = OperandVar(0x1008);
	DBG("o1 = o1:\t" << DBG_TEST(o1 == o1, true))
	DBG("o1 = o1':\t" << DBG_TEST(o1 == o1bis, true))
	DBG("o1 = o2:\t" << DBG_TEST(o1 == o2, false) << io::endl)
	DBG("o1 = o3:\t" << DBG_TEST(o1 == o3, false))
	DBG("o3 = o1:\t" << DBG_TEST(o3 == o1, false))
	DBG("o3 = o3:\t" << DBG_TEST(o3 == o3, true))
	DBG("o3 = o4:\t" << DBG_TEST(o3 == o4, false) << io::endl)
	
	OperandArithExpr oae = OperandArithExpr(ARITHOPR_MUL, o1, o2);
	OperandArithExpr oaebis = OperandArithExpr(ARITHOPR_MUL, o1, o2);
	DBG("o1*o2:\t" << oae)
	OperandArithExpr oae2 = OperandArithExpr(ARITHOPR_MUL, o2, o2);
	OperandArithExpr oae3 = OperandArithExpr(ARITHOPR_MUL, o2, o1);
	OperandArithExpr oae4 = OperandArithExpr(ARITHOPR_ADD, o1, o2);
	DBG("oae = oae:\t"  << DBG_TEST(oae == oae, true))
	DBG("oae = oae':\t"  << DBG_TEST(oae == oaebis, true))
	DBG("oae = oae2:\t" << DBG_TEST(oae == oae2, false))
	DBG("oae = oae3:\t" << DBG_TEST(oae == oae3, false))
	DBG("oae = oae4:\t" << DBG_TEST(oae == oae4, false))
	
	
	return 0;
}
