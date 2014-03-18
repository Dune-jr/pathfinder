#include <elm/io.h> // iostream
#include <elm/types.h>
#include <otawa/otawa.h>

using namespace elm;

t::uint32 x;

#include "predicate.h"

#define DEBUG_INFO() "[" << __FILE__ << ":" << __LINE__ << "] "
#define DBG(str) std::cout << DEBUG_INFO() << str << std::endl;

int main()
{
	ArithExpr e1 = ArithExpr(ARITHOPR_MUL, Operand(2), Operand((unsigned int)0x4000)); // e1 := 2 * @0x4000
	ArithExpr e1bis = ArithExpr(ARITHOPR_MUL, Operand((unsigned int)0x4000), Operand(2));
	Predicate p1 = Predicate(CONDOPR_EQ, Operand((unsigned int)0x4004), &e1); // p1 := @0x4004 = e1
	Predicate p2 = Predicate(CONDOPR_LE, Operand(-1), Operand((unsigned int)0x4000)); // p2 := -1 <= @0x4000
	
	DBG("--- Pretty Printing ---")
	DBG("y = 2x:\t" << p1)
	DBG("x >= -1:\t" << p2 << std::endl)
	
	DBG("--- Equality over predicates ---")
	DBG("p1 = p1:\t" << (p1 == p1 ? "true" : "false"))
	DBG("p2 = p2:\t" << (p2 == p2 ? "true" : "false"))
	DBG("p1 = p2:\t" << (p1 == p2 ? "true" : "false") << std::endl)
	
	DBG("--- Equality over arithmetic expressions  ---")
	DBG("2x = 2x:\t" << (e1 == e1 ? "true" : "false"))
	DBG("2x = x*2:\t" << (e1 == e1bis ? "true" : "false") << std::endl)
	return 0;
}
