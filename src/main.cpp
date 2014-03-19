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
	OperandConst oprconst = OperandConst(2);
	OperandVar oprvar = OperandVar(0x4000);
	OperandArithExpr e1 = OperandArithExpr(ARITHOPR_MUL, oprconst, oprvar); // e1 := 2 * @0x4000
	OperandArithExpr e1bis = OperandArithExpr(ARITHOPR_MUL, oprvar, oprconst);
	Predicate p1 = Predicate(CONDOPR_EQ, oprvar, e1); // p1 := @0x4000 = e1
	Predicate p2 = Predicate(CONDOPR_LE, oprconst, oprvar); // p2 := 2 <= @0x4000
	
	DBG("--- Pretty printing of predicates ---")
	DBG("p1:\t" << p1)
	DBG("p2:\t" << p2 << io::endl)
	
	DBG("--- Equality over predicates ---")
	DBG("p1 = p1:\t" << DBG_TEST(p1 == p1, true))
	DBG("p2 = p2:\t" << DBG_TEST(p2 == p2, true))
	DBG("p1 = p2:\t" << DBG_TEST(p1 == p2, false) << io::endl)
	
	DBG("--- Equality over Operands  ---")
	OperandConst o1 = OperandConst(12);
	OperandConst o1bis = OperandConst(12);
	OperandConst o2 = OperandConst(16);
	OperandVar o3 = OperandVar(0x1004);
	OperandVar o4 = OperandVar(0x1008);
	DBG("o1 = o1:\t" << DBG_TEST(o1 == o1, true))
	DBG("o1 = o1':\t" << DBG_TEST(o1 == o1bis, true))
	DBG("o1 = o2:\t" << DBG_TEST(o1 == o2, false))
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
