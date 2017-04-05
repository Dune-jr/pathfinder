#include <elm/io/Output.h>
#include <elm/types.h>
#include <elm/options.h>
#include <otawa/cfg/features.h> // COLLECTED_CFG_FEATURE
#include <otawa/prog/WorkSpace.h>
#include <otawa/app/Application.h>
#include "v1/analysis1.h"
#include "v2/analysis2.h"
#include "ffx.h"
#include "debug.h"
#include "oracle.h"

using namespace otawa;
using namespace option;

int dbg_flags = 0b00000000; // global analysis flags for debugging
int dbg_verbose = 0; // global verbose level (higher = less verbose)
int dbg_ = 0;
	
class Pathfinder: public Application {
public:
	Pathfinder(void): Application("Pathfinder", Version(1, 0, 0), "An infeasible path detection analysis", "J. Ruiz"),
		opt_s0 			 (SwitchOption::Make(*this).cmd("-V").cmd("--vv").cmd("--s0").cmd("--nosilent").description("run with maximal output")),
		opt_s1 			 (SwitchOption::Make(*this).cmd("-s").cmd("--s1").description("only display results")),
		opt_s2	  		 (SwitchOption::Make(*this).cmd("-S").cmd("--s2").cmd("--fullsilent").description("run with zero output")),
		opt_progress	 (SwitchOption::Make(*this).cmd("-p").cmd("--progress").description("display analysis progress (forces -s or +)")),
		opt_src_info	 (SwitchOption::Make(*this).cmd("-i").cmd("--src-info").description("print file/line number info")),
		opt_nocolor		 (SwitchOption::Make(*this).cmd("--nc").cmd("--no-color").cmd("--no-colors").description("do not use colors")),
		opt_nolinenumbers(SwitchOption::Make(*this).cmd("--nl").cmd("--no-line-nb").description("do not number lines of the output")),
		opt_noipresults	 (SwitchOption::Make(*this).cmd("--nir").cmd("--no-ip-results").description("do not print the list of IPs found")),
		opt_detailedstats(SwitchOption::Make(*this).cmd("--ds").cmd("--detailed-stats").description("display detailed stats, including average length of infeasible_paths found")),
		opt_output 		 (ValueOption<bool>::Make(*this).cmd("-o").cmd("--output").description("output the result of the analysis to a FFX file").def(false)),
		opt_graph_output (SwitchOption::Make(*this).cmd("-g").cmd("--graph-output").description("also output as a gnuplot .tsv graph file (requires -o)")),
		opt_noformattedflowinfo(SwitchOption::Make(*this).cmd("--nffi").cmd("--no-formatted-flowinfo").description("format flowinfo in paths like a list of items instead of pretty-printing it")),
		opt_automerge	 (SwitchOption::Make(*this).cmd("-a").cmd("--automerge").description("let the algorithm decide when to merge")),
		opt_applymerge	 (SwitchOption::Make(*this).cmd("--maf").cmd("--merge-after-apply").description("allow the algorithm to merge immediately after applying")),
		opt_dry			 (SwitchOption::Make(*this).cmd("-d").cmd("--dry").description("dry run (no solver calls)")),
		opt_v1			 (SwitchOption::Make(*this).cmd("-1").cmd("--v1").description("Run v1 of abstract interpretation (symbolic predicates)")),
		opt_v2			 (SwitchOption::Make(*this).cmd("-2").cmd("--v2").description("Run v2 of abstract interpretation (smarter structs)")),
		opt_v3			 (SwitchOption::Make(*this).cmd("-3").cmd("--v3").description("Run v3 of abstract interpretation (no inlining)")),
		opt_deterministic(SwitchOption::Make(*this).cmd("-D").cmd("--deterministic").description("Ensure deterministic output (two executions give the same output)")),
		opt_nolinearcheck(SwitchOption::Make(*this).cmd("--no-linear-check").description("do not check for predicates linearity before submitting to SMT solver")),
		opt_no_initial_data(SwitchOption::Make(*this).cmd("--nid").cmd("--no-initial-data").description("Do not include initial data from FFX (multitask mode)")),
		opt_sp_critical  (SwitchOption::Make(*this).cmd("--sp-critical").description("Abort analysis on loss of SP info")),
		opt_nounminimized(SwitchOption::Make(*this).cmd("--no-unminimized-paths").description("do not output infeasible paths for which minimization job failed")),
		opt_allownonlinearoperators(SwitchOption::Make(*this).cmd("--allow-nonlinear-operators").description("assert linear predicates that use *,/,mod in the SMT solver (unsafe)")),
		opt_nocleantops  (SwitchOption::Make(*this).cmd("--no-clean-tops").description("do not clean introduced variables Tk (unstable as of early 2017)")),
		opt_dontassumeidsp(SwitchOption::Make(*this).cmd("--dasp").cmd("--dont-assume-id-sp").description("do not assume sp = SP0 at the end of a function")),
		opt_nowidening 	 (SwitchOption::Make(*this).cmd("--no-widening").description("Scratch instead of using induction variables")),
		opt_slice		 (SwitchOption::Make(*this).cmd("--slice").description("slice away instructions that do not impact the control flow")),
		opt_dumpoptions	 (SwitchOption::Make(*this).cmd("--dump-options").cmd("--do").description("print the selected options for the analysis")),
		// opt_virtualize	 (ValueOption<bool>::Make(*this).cmd("-z").cmd("--virtualize").description("virtualize the CFG (default: true)").def(true)),
		opt_merge 		 (ValueOption<int>::Make(*this).cmd("-m").cmd("--merge").description("merge when exceeding X states at a control point").def(0)),
		opt_multithreading(ValueOption<int>::Make(*this).cmd("-j").description("enable multithreading on the given amount of cores (0/1=no multithreading, -1=autodetect)").def(0)),
		opt_x 			 (ValueOption<int>::Make(*this).cmd("-x").description("(for internal debugging)").def(0)) { }

protected:
	virtual void work(const string &entry, PropList &props) throw (elm::Exception)
	{
		int analysis_flags, merge_thresold, nb_cores;
		initializeLoggingOptions();
		setFlags(analysis_flags, merge_thresold, nb_cores);
		if(opt_dumpoptions)
			dumpOptions(analysis_flags, merge_thresold, nb_cores);

		Analysis* analysis =
			((analysis_flags & Analysis::IS_V1) || (analysis_flags & Analysis::IS_V2))
			? (Analysis*)new Analysis1(workspace(), props, analysis_flags, merge_thresold, nb_cores)  // v1, v2
			: (Analysis*)new Analysis2(workspace(), props, analysis_flags, merge_thresold, nb_cores); // v3
		analysis->run(workspace()); // TODO: make that default for (workspace) overloard fnct

		// outputing to .ffx
		if(opt_output.get())
		{
			FFX ffx_output(analysis->infeasiblePaths());
			ffx_output.output(elm::String(entry), entry + "_ips.ffx", opt_graph_output ? entry + "_ips.tsv" : "");
		}
		delete analysis;
	}

private:
	SwitchOption opt_s0, opt_s1, opt_s2, opt_progress, opt_src_info, opt_nocolor, opt_nolinenumbers, opt_noipresults, opt_detailedstats;
	ValueOption<bool> opt_output;
	SwitchOption opt_graph_output, opt_noformattedflowinfo, opt_automerge, opt_applymerge, opt_dry, opt_v1, opt_v2, opt_v3, opt_deterministic, opt_nolinearcheck,
			     opt_no_initial_data, opt_sp_critical, opt_nounminimized, opt_allownonlinearoperators, opt_nocleantops, opt_dontassumeidsp, opt_nowidening, opt_slice, opt_dumpoptions;
	ValueOption<int> opt_merge, opt_multithreading, opt_x;

	void setFlags(int& analysis_flags, int& merge_thresold, int& nb_cores) {
		nb_cores = getNumberofCores();
		merge_thresold = getMergeThresold();
		dbg_flags = 
			  (opt_deterministic 		? DBG_DETERMINISTIC : 0)
			| (!opt_noipresults 		? DBG_RESULT_IPS : 0)
			| (!opt_noformattedflowinfo ? DBG_FORMAT_FLOWINFO : 0)
			| (opt_detailedstats 		? DBG_DETAILED_STATS : 0)
		;
		analysis_flags =
			  (opt_slice					? Analysis::SLICE_CFG : 0)
			| (opt_progress					? Analysis::SHOW_PROGRESS : 0)
			| (!opt_nolinearcheck			? Analysis::SMT_CHECK_LINEAR : 0)
			| (!opt_nounminimized			? Analysis::UNMINIMIZED_PATHS : 0)
			| (opt_allownonlinearoperators	? Analysis::ALLOW_NONLINEAR_OPRS : 0)
			| (!opt_nocleantops				? Analysis::CLEAN_TOPS : 0)
			| (!opt_dontassumeidsp			? Analysis::ASSUME_IDENTICAL_SP : 0)
			| (opt_nowidening				? Analysis::NO_WIDENING : 0)
			| (opt_dry						? Analysis::DRY_RUN : 0)
			| (opt_v1						? Analysis::IS_V1 : 0)
			| (opt_v2						? Analysis::IS_V2 : 0)
			| (opt_v3						? Analysis::IS_V3 : 0)
			| ((opt_v1||opt_v2)				? Analysis::VIRTUALIZE_CFG : 0)
			| (!opt_no_initial_data			? Analysis::USE_INITIAL_DATA : 0)
			| (opt_sp_critical				? Analysis::SP_CRITICAL : 0)
			| (opt_applymerge				? Analysis::MERGE_AFTER_APPLY : 0)
			| (nb_cores > 1					? Analysis::MULTITHREADING : 0)
			| ((opt_merge || opt_automerge)	? Analysis::MERGE : 0)
			| (true 						? Analysis::POST_PROCESSING : 0)
		;
		if(opt_x)
			dbg_ = opt_x.get();
		ASSERTP(opt_v1 + opt_v2 + opt_v3 == 1, "One analysis must be selected (-1, -2 or -3).")
	}
	void initializeLoggingOptions() {
		// high verbose numbers are more silent. TODO: that is counterintuitive
		dbg_verbose = 1; // default
		if(opt_s1) dbg_verbose = 2;
		if(opt_s0) dbg_verbose = 0;
		if(opt_s2) dbg_verbose = 3;
		if(opt_progress)
			dbg_verbose = max(dbg_verbose, 2); // minimum 2	
		elm::log::Debug::setDebugFlag(dbg_verbose == DBG_VERBOSE_ALL || dbg_verbose == DBG_VERBOSE_MINIMAL);
		elm::log::Debug::setVerboseLevel(dbg_verbose == DBG_VERBOSE_ALL ? 1 : 0);
		elm::log::Debug::setColorFlag(! opt_nocolor);
		elm::log::Debug::setSourceInfoFlag(opt_src_info);
		elm::log::Debug::setNumberingFlag(! opt_nolinenumbers);
		elm::log::Debug::setPrefixColor(elm::color::Yel);	
	}
	int getNumberofCores() {
		if(opt_multithreading.get() == -1) { // autodetect
			// int nb_cores = AUTODETECT();
			int nb_cores = 4;
			DBG("Autodetected " << nb_cores << " cores. (TODO)")
			return nb_cores;
		}
		return opt_multithreading.get(); // either no multithreading (=0/1) or the amount of desired cores
	}
	int getMergeThresold() {
		if(opt_merge)
			return opt_merge.get();
		if(opt_automerge)
			return 250; // 250 is good 
		return 0;
	}
	void dumpOptions(int analysis_flags, int merge_thresold, int nb_cores) {
		#define DBGPREFIX(str) "\t-" << elm::io::StringFormat(str).width(30) << ": " 
		#define DBGOPT(str, val, normal) cout << DBGPREFIX(str) << (bool(val)==normal?color::IGre():color::IRed()) << (val ? "YES" : "NO") << color::RCol() << endl;
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
		for(int i = 3; i >= 1; i--)
			cout << (i>dbg_verbose ? "=" : " ");
		cout << "]" << color::RCol() << endl;
		DBGOPT("DISPLAY TIME & NON-DTMSTIC INFO", !(dbg_flags&DBG_DETERMINISTIC), true)
		DBGOPT("DISPLAY RESULT INF. PATHS"		, dbg_flags&DBG_RESULT_IPS, true)
		DBGOPT("PRETTY PRINTING FOR FLOWINFO"	, dbg_flags&DBG_FORMAT_FLOWINFO, true)
		DBGOPT("DISPLAY DETAILED STATS"			, dbg_flags&DBG_DETAILED_STATS, false)
		cout << "Analysis:" << endl;
		DBGOPT("VIRTUALIZE"						, analysis_flags&Analysis::VIRTUALIZE_CFG, true)
		DBGOPT("SLICE"							, analysis_flags&Analysis::SLICE_CFG, false)
		DBGOPT("DISPLAY PROGRESS"				, analysis_flags&Analysis::SHOW_PROGRESS, false)
		DBGOPT("CHECK LINEARITY BEFORE SMT CALL", analysis_flags&Analysis::SMT_CHECK_LINEAR, true)
		DBGOPT("ALLOW NONLINEAR OPERATORS (unsafe)", analysis_flags&Analysis::ALLOW_NONLINEAR_OPRS, false)
		DBGOPT("CLEAN TOPS" 					, analysis_flags&Analysis::CLEAN_TOPS, true)
		DBGOPT("KEEP UNMINIMIZED PATHS"			, analysis_flags&Analysis::UNMINIMIZED_PATHS, true)
		DBGOPT("USE INITIAL DATA"				, analysis_flags&Analysis::USE_INITIAL_DATA, true)
		DBGOPT("NO WIDENING"					, analysis_flags&Analysis::NO_WIDENING, false)
		DBGOPT("RUN DRY (NO SMT SOLVER)"		, analysis_flags&Analysis::DRY_RUN, false)
		DBGOPT("RUN V1 A.I."					, analysis_flags&Analysis::IS_V1, false)
		DBGOPT("RUN V2 A.I."					, analysis_flags&Analysis::IS_V2, false)
		DBGOPT("RUN V3 A.I."					, analysis_flags&Analysis::IS_V3, false)
		cout << DBGPREFIX("MERGING THRESOLD");
		if(analysis_flags&Analysis::MERGE)
			cout << color::IRed() << merge_thresold << color::RCol() << endl;
		else
			cout << color::IGre() << "NONE" << color::RCol() << endl;
		cout << DBGPREFIX("MULTITHREADING");
		if(analysis_flags&Analysis::MULTITHREADING)
			cout << color::IRed() << "x" << nb_cores << color::RCol() << endl;
		else
			cout << color::IGre() << "NONE" << color::RCol() << endl;
		cout << "=============================================" << endl;
		#undef DBGOPT
		#undef DBGPREFIX
	}
};

OTAWA_RUN(Pathfinder);

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
	OperandArith e11 = OperandArith(ARITHOPR_NEG, t1);
	// OperandArith e12 = OperandArith(ARITHOPR_SUB, two, three);
	OperandArith e1 = OperandArith(ARITHOPR_ADD, t1, e11);

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
	OperandArith e1 = OperandArith(ARITHOPR_MUL, oprconst, oprvar); // e1 := 2 * @0x4000
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
	
	OperandArith oae = OperandArith(ARITHOPR_MUL, o1, o2);
	OperandArith oaebis = OperandArith(ARITHOPR_MUL, o1, o2);
	DBG("o1*o2:\t" << oae)
	OperandArith oae2 = OperandArith(ARITHOPR_MUL, o2, o2);
	OperandArith oae3 = OperandArith(ARITHOPR_MUL, o2, o1);
	OperandArith oae4 = OperandArith(ARITHOPR_ADD, o1, o2);
	DBG("oae = oae:\t"  << DBG_TEST(oae == oae, true))
	DBG("oae = oae':\t"  << DBG_TEST(oae == oaebis, true))
	DBG("oae = oae2:\t" << DBG_TEST(oae == oae2, false))
	DBG("oae = oae3:\t" << DBG_TEST(oae == oae3, false))
	DBG("oae = oae4:\t" << DBG_TEST(oae == oae4, false))
}
*/