/*
 * DAG.cpp
 *
 *  Created on: 15 avr. 2016
 *      Author: casse
 */

#include "debug.h"
#include "DAG.h"

using namespace elm::color;

void testSimplify(DAG& d)
{
	const Operand* zero  = d.cst(0);
	const Operand* one   = d.cst(1);
	const Operand* two   = d.cst(2);
	const Operand* three = d.cst(3);
	DBG("zero: "  << *zero)
	DBG("one: "   << *one)
	DBG("two: "   << *two)
	DBG("three: " << *three)

	const Operand* four =  d.cst(4);four=four;
	const Operand* t1 = d.var(-1);

	DBG("making neg(t1)")
	const Operand* e11 = d.neg(t1);
	DBG("neg(t1): " << *e11)
	const Operand* e1 = d.add(t1, e11);
	const Operand* f = d.sub(two, three);f=f;
	const Operand* fv2 = d.sub(two, three);fv2=fv2;

	DBG("e1: "    << *e1)
	DBG("f=fv2: " << DBG_TEST(f==fv2, true) << "  (" << *f << "==" << *fv2 << ")")
	// DBG("one+two: " << *d.add(one, two))
	// DBG("one+two simplified: " << **d.op(ARITHOPR_ADD, one, two)->simplify())
	// DBG("one+two simplified equal three: " << DBG_TEST(three == *d.op(ARITHOPR_ADD, one, two)->simplify(), true))
}

void testPredicates(DAG& d)
{
	const Operand* oprconst = d.cst(2);
	const Operand* oprvar = d.mem(0x4000);
	const Operand* e1 = d.mul(oprconst, oprvar); // e1 := 2 * @0x4000
	const Predicate* p1 = d.pred(CONDOPR_EQ, oprvar, e1); // p1 := @0x4000 = e1
	const Predicate* p1bis = d.eq(oprvar, e1); // p1 := @0x4000 = e1
	const Predicate* p2 = d.pred(CONDOPR_LE, oprconst, oprvar); // p2 := 2 <= @0x4000
	
	DBG("--- Equality over predicates ---")
	DBG("p1 = p1':\t"<< DBG_TEST(p1 == p1bis, true))
	DBG("p2 = p2:\t" << DBG_TEST(p2 == p2, true))
	DBG("p1 = p2:\t" << DBG_TEST(p1 == p2, false) << io::endl)
}

void testOperands(DAG& d)
{
	DBG("--- Equality over Operands  ---")
	const Operand* o1 = d.cst(12);
	const Operand* o1bis = d.cst(12);
	const Operand* o2 = d.cst(16);
	const Operand* o3 = d.mem(0x1004);
	const Operand* o4 = d.mem(0x1008);
	DBG("diag: " << d)
	DBG("o1 = o1:\t" << DBG_TEST(o1 == o1, true) 	<< " \t" << *o1 << "==" << *o1)
	DBG("o1 = o1':\t"<< DBG_TEST(o1 == o1bis, true) << " \t" << *o1 << "==" << *o1bis)
	DBG("o1 = o2:\t" << DBG_TEST(o1 == o2, false) 	<< " \t" << *o1 << "==" << *o2)
	DBG("o1 = o3:\t" << DBG_TEST(o1 == o3, false) 	<< " \t" << *o1 << "==" << *o3)
	DBG("o3 = o1:\t" << DBG_TEST(o3 == o1, false) 	<< " \t" << *o3 << "==" << *o1)
	DBG("o3 = o3:\t" << DBG_TEST(o3 == o3, true) 	<< " \t" << *o3 << "==" << *o3)
	DBG("o3 = o4:\t" << DBG_TEST(o3 == o4, false) 	<< " \t" << *o3 << "==" << *o4 << io::endl)
	
	const Operand* oae = d.mul(o1, o2);
	const Operand* oaebis = d.mul(o1, o2);
	DBG("o1*o2:\t" << *oae)
	const Operand* oae2 = d.mul(o2, o2);
	const Operand* reverse_oae = d.mul(o2, o1);
	const Operand* oae4 = d.add(o1, o2);
	DBG("oae = oae:\t"  << DBG_TEST(oae == oae, true))
	DBG("oae = oae':\t"  << DBG_TEST(oae == oaebis, true))
	DBG("oae = oae2:\t" << DBG_TEST(oae == oae2, false))
	DBG("oae = roae:\t" << DBG_TEST(oae == reverse_oae, true))
	DBG("oae = oae4:\t" << DBG_TEST(oae == oae4, false))
}

int _main(void)
{
	elm::log::Debug::setSourceInfoFlag(false);
	DAG dag(16,16);
	testOperands(dag);
	testPredicates(dag);
	testSimplify(dag);
	DBG("diag: " << dag)
	DBG("==================================\n")
	Predicate *p = dag.eq(dag.sub(dag.neg(dag.add(dag.cst(2),dag.cst(2))), dag.sub(dag.mul(dag.cst(2),dag.cst(3)), dag.add(dag.mul(dag.cst(2),dag.cst(47)),dag.mul(dag.cst(2),dag.cst(13))))),
		   dag.add(dag.cst(13),dag.cst(47)));
	DBG("diag: " << dag)
	DBG("Original predicate: -(2+2) - (2*3 - (2*47 + 2*13)) = 13+47")
	DBG("Predicate: " << *p)
	return 0;
}
