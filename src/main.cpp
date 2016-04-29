#include <elm/io/Output.h>
#include <elm/types.h>
#include <elm/options.h>
#include <otawa/app/Application.h> // main Display class
#include <otawa/cfg/features.h> // COLLECTED_CFG_FEATURE
#include <otawa/hard/Platform.h>
#include <otawa/dfa/State.h> // INITIAL_STATE_FEATURE
#include <otawa/prog/WorkSpace.h>
#include <otawa/app/Application.h>
#include "/home/jruiz/Documents/oslice/blockBased/cfg_v2_plugin/oslice_features.h"
#include "analysis.h"
#include "ffx.h"
#include "debug.h"
#include "oracle.h"

using namespace elm;
using namespace otawa;
using namespace option;

void dumpOptions(int dbg_flags, int dbg_verbose, int analysis_flags, int merge_thresold);
void testPredicates();
void testOperands();
void testSimplify();	

int dbg_flags = 0b00000000; // global analysis flags for debugging
int dbg_verbose = 0; // global verbose level (higher = less verbose)
	
class Display: public Application {
public:
	Display(void): Application("display", Version(1, 0, 0)),
		// opt1(SwitchOption::Make(manager).cmd("-o").cmd("--com").description("option 1")) { }
		opt_s1(SwitchOption::Make(*this).cmd("-s").cmd("--s1").cmd("--silent").description("run with minimal output")),
		opt_s2(SwitchOption::Make(*this).cmd("--s2").description("only display results")),
		opt_s3(SwitchOption::Make(*this).cmd("--s3").cmd("--fullsilent").description("run with zero output")),
		opt_output(ValueOption<bool>::Make(*this).cmd("-o").cmd("--output").description("output the result of the analysis to a FFX file").def(false)),
		opt_graph_output(SwitchOption::Make(*this).cmd("-g").cmd("--graph-output").description("also output as a gnuplot .tsv graph file (requires -o)")),
		opt_nocolor(SwitchOption::Make(*this).cmd("--no-color").cmd("--no-colors").description("do not use colors")),
		opt_src_info(SwitchOption::Make(*this).cmd("-i").cmd("--src-info").description("print file/line number info")),
		opt_nolinenumbers(SwitchOption::Make(*this).cmd("--nl").cmd("--no-line-nb").description("do not number lines of the output")),
		opt_progress(SwitchOption::Make(*this).cmd("-p").cmd("--progress").description("display analysis progress (forces --s2+)")),
		opt_dumpoptions(SwitchOption::Make(*this).cmd("--dump-options").description("print the selected options for the analysis")),
		opt_notime(SwitchOption::Make(*this).cmd("--no-time").description("do not print execution time")),
		// opt_nopred(SwitchOption::Make(*this).cmd("--no-predicates").description("do not print debug info about predicates")), // no longer working
		opt_noipresults(SwitchOption::Make(*this).cmd("--no-ip-results").description("do not print the list of IPs found")),
		opt_noflowinfo(SwitchOption::Make(*this).cmd("--no-flowinfo").description("do not print context flowinfo in path debugs")),
		// opt_preanalysis(SwitchOption::Make(*this).cmd("--preanalysis").description("run pre-analysis (obsolete)")),
		opt_avgiplength(SwitchOption::Make(*this).cmd("--average-ip-length").description("display average length of infeasible_paths found")),
		opt_nolinearcheck(SwitchOption::Make(*this).cmd("--no-linear-check").description("do not check for predicates linearity before submitting to SMT solver")),
		opt_nounminimized(SwitchOption::Make(*this).cmd("--no-unminimized-paths").description("do not output infeasible paths for which minimization job failed")),
		opt_slice(SwitchOption::Make(*this).cmd("--slice").description("slice away instructions that do not impact the control flow")),
		opt_dry(SwitchOption::Make(*this).cmd("--dry").description("dry run (no solver calls)")),
		opt_automerge(SwitchOption::Make(*this).cmd("--automerge").description("let the algorithm decide when to merge")),
		opt_virtualize(ValueOption<bool>::Make(*this).cmd("-z").cmd("--virtualize").description("virtualize the CFG (default: true)").def(true)),
		opt_merge(ValueOption<int>::Make(*this).cmd("--merge").description("merge when exceeding X states at a control point").def(0)) { }

protected:
	virtual void work(const string &entry, PropList &props) throw (elm::Exception) {
		// workspace()->require(COLLECTED_CFG_FEATURE, props); // INVOLVED_CFGS
		workspace()->require(dfa::INITIAL_STATE_FEATURE, props); // dfa::INITIAL_STATE
		if(opt_virtualize.get())
			workspace()->require(VIRTUALIZED_CFG_FEATURE, props); // inline calls
		workspace()->require(LOOP_HEADERS_FEATURE, props); // LOOP_HEADER, BACK_EDGE
		workspace()->require(LOOP_INFO_FEATURE, props); // LOOP_EXIT_EDGE
		if(opt_slice) {
			// otawa::oslice::SLICING_CFG_OUTPUT_PATH(props) = "slicing.dot";
			// otawa::oslice::SLICED_CFG_OUTPUT_PATH(props) = "sliced.dot";
			workspace()->require(otawa::oslice::COND_BRANCH_COLLECTOR_FEATURE, props);
			workspace()->require(otawa::oslice::SLICER_FEATURE, props);
		}
		const CFGCollection *cfgs = INVOLVED_CFGS(workspace()); // retrieving the main CFG
		const dfa::State *inital_state = dfa::INITIAL_STATE(workspace()); // retrieving the initial state
		ASSERTP(cfgs->count() > 0, "no CFG found"); // make sure we have at least one CFG
		CFG *cfg = cfgs->get(0); // then get the first CFG
		// simpleCFGparse(cfg);
		int sp_id = workspace()->platform()->getSP()->number(); // retrieve the id of the stack pointer
		unsigned int max_registers = (unsigned int)workspace()->platform()->regCount(); // retrieve the count of registers
		unsigned int max_tempvars = (unsigned int)workspace()->process()->maxTemp(); // retrieve the maximum number of tempvars used
		int analysis_flags = 0, merge_thresold = 0;

		// high verbose numbers are more silent. TODO: that is counterintuitive
		if(opt_s1) dbg_verbose = 1;
		if(opt_s2) dbg_verbose = 2;
		if(opt_s3) dbg_verbose = 3;
		elm::log::Debug::setDebugFlag(dbg_verbose == DBG_VERBOSE_ALL || dbg_verbose == DBG_VERBOSE_MINIMAL);
		elm::log::Debug::setVerboseLevel(dbg_verbose == DBG_VERBOSE_ALL ? 1 : 0);
		elm::log::Debug::setColorFlag(! opt_nocolor);
		elm::log::Debug::setSourceInfoFlag(opt_src_info);
		elm::log::Debug::setNumberingFlag(! opt_nolinenumbers);
		elm::log::Debug::setPrefixColor(elm::color::Yel);
		if(opt_notime)
			dbg_flags |= DBG_NO_TIME;
		// if(opt_nopred)
			// dbg_flags |= DBG_NO_PREDICATES;
		if(! opt_noipresults)
			dbg_flags |= DBG_RESULT_IPS;
		if(! opt_noflowinfo)
			dbg_flags |= DBG_PRINT_FLOWINFO;
		// if(opt_preanalysis)
		// 	dbg_flags |= DBG_PREANALYSIS;
		if(opt_avgiplength)
			dbg_flags |= DBG_AVG_IP_LENGTH; 

		if(opt_progress) {
			analysis_flags |= Analysis::SHOW_PROGRESS;
			dbg_verbose = max(dbg_verbose, 2); // minimum 2
		}
		if(! opt_nolinearcheck)
			analysis_flags |= Analysis::SMT_CHECK_LINEAR;
		if(! opt_nounminimized)
			analysis_flags |= Analysis::UNMINIMIZED_PATHS;
		if(opt_dry)
			analysis_flags |= Analysis::DRY_RUN;
		if(opt_merge || opt_automerge)
		{
			analysis_flags |= Analysis::MERGE;
			merge_thresold = opt_automerge ? 250 : opt_merge.get(); // 250 is good 
		}
		// if(opt_virtualize.get())
		// 	analysis_flags |= Analysis::FOLLOW_CALLS;
		if(opt_dumpoptions)
			dumpOptions(dbg_flags, dbg_verbose, analysis_flags, merge_thresold);
		DefaultAnalysis analysis({inital_state, sp_id, max_tempvars, max_registers}, merge_thresold, analysis_flags);
		analysis.run(cfg);

		// outputing to .ffx
		if(opt_output.get())
		{
			const Vector<DetailedPath>& infeasible_paths = analysis.infeasiblePaths();
			FFX ffx_output(infeasible_paths);
			const elm::String name = entry + "_ips.ffx"; // TODO: use args= arguments();
			const elm::String gname = opt_graph_output ? entry + "_ips.tsv" : "";
			ffx_output.output(elm::String(entry), name, gname);
			if(dbg_verbose < DBG_VERBOSE_NONE)
			{
				cout << "output to " + name << endl;
				if(opt_graph_output)
					cout << "graph output to " + gname << endl;
			}
		}
	}

private:
	// option::Manager manager;
	SwitchOption opt_s1, opt_s2, opt_s3;
	ValueOption<bool> opt_output;
	SwitchOption opt_graph_output, opt_nocolor, opt_src_info, opt_nolinenumbers, opt_progress, opt_dumpoptions, opt_notime, opt_noipresults, opt_noflowinfo,
		/*opt_preanalysis,*/ opt_avgiplength, opt_nolinearcheck, opt_nounminimized, opt_slice, opt_dry, opt_automerge; //, opt_virtualize;
	ValueOption<bool> opt_virtualize;
	ValueOption<int> opt_merge;
};

OTAWA_RUN(Display);

void dumpOptions(int dbg_flags, int dbg_verbose, int analysis_flags, int merge_thresold)
{
	#define DBGPREFIX(str) "\t-" << elm::io::StringFormat(str).width(30) << ": " 
	#define DBGOPT(str, val, normal) cout << DBGPREFIX(str) << (bool(val)==normal?color::IGre():color::IRed()) << ((val) ? "YES" : "NO") << color::RCol() << endl;
	cout << "============== DUMPING OPTIONS ==============" << endl;
	cout << "Logging:" << endl;
	DBGOPT("LOGGING", elm::log::Debug::getDebugFlag(), true)
	DBGOPT("COLOR", elm::log::Debug::getColorFlag(), true)
	DBGOPT("SOURCEINFO", elm::log::Debug::getSourceInfoFlag(), false)
	DBGOPT("NUMBERING", elm::log::Debug::getNumberingFlag(), true)
	cout << DBGPREFIX("PREFIX COLOR") << elm::log::Debug::getPrefixColor() << "THIS" << color::RCol() << endl;
	cout << DBGPREFIX("LOG VERBOSE MASK") << (elm::log::Debug::getVerboseLevel()?color::IGre():color::IRed()) << elm::log::Debug::getVerboseLevel() << color::RCol() << endl;
	cout << "Debugging:" << endl;
	cout << DBGPREFIX("VERBOSE LEVEL") << color::ICya() << "[";
	for(int i = 3; i > 0; i--)
		cout << (i>dbg_verbose ? "=" : " ");
	cout << "]" << color::RCol() << endl;
	DBGOPT("DISPLAY TIME", !(dbg_flags&DBG_NO_TIME), true)
	DBGOPT("DISPLAY RESULT INF. PATHS", dbg_flags&DBG_RESULT_IPS, true)
	DBGOPT("DISPLAY FLOWINFO", dbg_flags&DBG_PRINT_FLOWINFO, true)
	DBGOPT("DISPLAY AVERAGE IP LENGTH", dbg_flags&DBG_AVG_IP_LENGTH, false)
	cout << "Analysis:" << endl;
	DBGOPT("DISPLAY PROGRESS", analysis_flags&Analysis::SHOW_PROGRESS, false)
	DBGOPT("CHECK LINEARITY BEFORE SMT CALL", analysis_flags&Analysis::SMT_CHECK_LINEAR, true)
	DBGOPT("KEEP UNMINIMIZED PATHS", analysis_flags&Analysis::UNMINIMIZED_PATHS, true)
	DBGOPT("RUN DRY (NO SMT SOLVER)", analysis_flags&Analysis::DRY_RUN, false)
	cout << DBGPREFIX("MERGING THRESOLD");
	if(analysis_flags&Analysis::MERGE)
		cout << color::IRed() << merge_thresold << color::RCol() << endl;
	else
		cout << color::IGre() << "NONE" << color::RCol() << endl;
	cout << "=============================================" << endl;
	#undef DBGOPT
}

void simpleCFGparse(CFG* cfg)
{
	Vector<Block*> todo;
	todo.push(cfg->entry());
	while(todo)
	{
		Block *b = todo.pop();
		DBG(b << " (id: " << b->id() << ")")
		for(Block::EdgeIter iter(b->outs()); iter; iter++)
		{
			if(! BACK_EDGE(*iter) )
				todo.push(iter->target());
		}
		if(b->isSynth())
		{
			DBG(color::IPur() << "new call" << color::RCol())
			todo.push(b->toSynth()->callee()->entry());
		}
	}
}
/*
void testSimplify()
{
	OperandConst zero  = OperandConst(0);
	OperandConst one   = OperandConst(1);
	OperandConst two   = OperandConst(2);
	OperandConst three = OperandConst(3);
	OperandVar t1 = OperandVar(-1);
	OperandArithExpr e11 = OperandArithExpr(ARITHOPR_NEG, t1);
	// OperandArithExpr e12 = OperandArithExpr(ARITHOPR_SUB, two, three);
	OperandArithExpr e1 = OperandArithExpr(ARITHOPR_ADD, t1, e11);

	DBG("zero: "  << zero)
	DBG("one: "   << one)
	DBG("two: "   << two)
	DBG("three: " << three)
	DBG("e1: "    << e1)
	if(Option<Operand*> o = e1.simplify())
		DBG("e1 simplified: " << **o)
}

void testPredicates()
{
	OperandConst oprconst = OperandConst(2);
	OperandVar oprvar = OperandVar(0x4000);
	OperandArithExpr e1 = OperandArithExpr(ARITHOPR_MUL, oprconst, oprvar); // e1 := 2 * @0x4000
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
*/