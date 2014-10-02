// #define DBG_NO_DEBUG

#include <elm/io.h>
#include <elm/io/Output.h>
#include <elm/types.h>
#include <otawa/otawa.h>
#include <otawa/app/Application.h> // main Display class
#include <otawa/cfg/features.h> // COLLECTED_CFG_FEATURE
#include <otawa/hard/Platform.h>
#include <elm/options.h>

#include "analysis.h"
#include "predicate.h"
#include "debug.h"

using namespace elm;
using namespace otawa;

void testPredicates();
void testOperands();
void testSimplify();
void testAnalysis(CFG *cfg);
void makeRainbow();

int dbg_flags = 0;

class Display: public Application {
public:
    Display(void): Application("display", Version(1, 0, 0)),
    	// opt1(option::SwitchOption::Make(manager).cmd("-o").cmd("--com").description("option 1")) { }
    	opt_silent(option::SwitchOption::Make(*this).cmd("-s").cmd("--silent").description("run with minimal output")) { }
        
protected:
	virtual void work(const string &entry, PropList &props) throw (elm::Exception) {
		// makeRainbow(); // to easily see where the output begins
		workspace()->require(COLLECTED_CFG_FEATURE, props); 
        const CFGCollection *cfgs = INVOLVED_CFGS(workspace()); // retrieving the main CFG
		assert(cfgs->count() > 0); // make sure we have at least one CFG
		CFG *cfg = cfgs->get(0); // then get the first CFG
		int sp_id = workspace()->platform()->getSP()->number(); // retrieve the id of the stack pointer
		int max_registers = workspace()->platform()->regCount(); // retrieve the count of registers
		int max_tempvars = workspace()->process()->maxTemp(); // retrieve the maximum number of tempvars used

		if(opt_silent)
			dbg_flags |= DBG_NO_DEBUG;
		Analysis analysis = Analysis(cfg, sp_id, max_tempvars, max_registers);
	}

private:
	option::Manager manager;
	option::SwitchOption opt_silent;
};

OTAWA_RUN(Display)

void testSimplify()
{
	OperandConst zero = OperandConst(0);
	OperandConst one = OperandConst(1);
	OperandConst two = OperandConst(2);
	OperandConst three = OperandConst(3);
	OperandVar t1 = OperandVar(-1);
	OperandArithExpr e11 = OperandArithExpr(ARITHOPR_NEG, t1);
	// OperandArithExpr e12 = OperandArithExpr(ARITHOPR_SUB, two, three);
	OperandArithExpr e1 = OperandArithExpr(ARITHOPR_ADD, t1, e11);

	DBG("zero: " << zero)
	DBG("one: " << one)
	DBG("two: " << two)
	DBG("three: " << three)
	DBG("e1: " << e1)
	if(Option<Operand*> o = e1.simplify())
		DBG("e1 simplified: " << **o)
}

void testPredicates()
{
	OperandConst oprconst = OperandConst(2);
	OperandVar oprvar = OperandVar(0x4000);
	OperandArithExpr e1 = OperandArithExpr(ARITHOPR_MUL, oprconst, oprvar); // e1 := 2 * @0x4000
	// OperandArithExpr e1bis = OperandArithExpr(ARITHOPR_MUL, oprvar, oprconst);
	Predicate p1 = Predicate(CONDOPR_EQ, oprvar, e1); // p1 := @0x4000 = e1
	Predicate p2 = Predicate(CONDOPR_LE, oprconst, oprvar); // p2 := 2 <= @0x4000
	
	DBG("--- Pretty printing of predicates ---")
	DBG("p1:\t" << p1)
	DBG("p2:\t" << p2 << io::endl)
	
	DBG("--- Equality over predicates ---")
	DBG("p1 = p1:\t" << DBG_TEST(p1 == p1, true))
	DBG("p2 = p2:\t" << DBG_TEST(p2 == p2, true))
	DBG("p1 = p2:\t" << DBG_TEST(p1 == p2, false) << io::endl)
}

void testOperands()
{
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
}

void testAnalysis(CFG *cfg)
{
#	if 0 // Does not compile when Analysis respects encapsulation rules
		Analysis::Path path;
		BasicBlock::OutIterator outs(cfg->firstBB());
		Edge* edge = *outs;
		path += edge;
#	endif
}

void makeRainbow()
{
	const char* colors[] = {
		COLOR_BIBla,
		COLOR_BIBlu,
		COLOR_BICya,
		COLOR_BIGre,
		COLOR_BIPur,
		COLOR_BIRed,
		COLOR_BIWhi,
		COLOR_BIYel
	};
	for(int i = 0; i < 160; i++)
		cout << colors[(i%8)] << "#";
	cout << COLOR_RCol << io::endl;
}
