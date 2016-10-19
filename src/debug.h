/*
 * Debugging, printing tools
 */
#ifndef _DEBUG_H
#define _DEBUG_H

#define ELM_NO_DBG // do not use standard macro names
#include <elm/log/Log.h>
#define DBG(str) { ELM_DBGV(1, str << elm::io::endl) }
#define DBGG ELM_DBGLN

#include <ostream>
#include <elm/string/String.h>
#include <elm/string/AutoString.h>

// v1:debug loop analysis and many other
// v2: macro debugs
// #define DBGG(str) { if(dbg_verbose < DBG_VERBOSE_RESULTS_ONLY) elm::cout << elm::log::Debug::debugPrefix(__FILE__, __LINE__) << str << elm::color::RCol() << elm::io::endl; }
#define DBG_WARNINGS

#define UNTESTED_CRITICAL true // Raise exceptions when executing untested code if true

#define DBG_SEPARATOR " "

enum
{
	DBG_RESULT_IPS	    = 1 << 3,
	DBG_DETERMINISTIC   = 1 << 4,
	DBG_PREANALYSIS	    = 1 << 6,
	// DBG_PROGRESS	    = 1 << 7,
	DBG_FORMAT_FLOWINFO = 1 << 8,
	DBG_DETAILED_STATS  = 1 << 9
};
extern int dbg_flags;

enum
{
	DBG_VERBOSE_ALL			 = 0,
	DBG_VERBOSE_MINIMAL		 = 1,
	DBG_VERBOSE_RESULTS_ONLY = 2,
	DBG_VERBOSE_NONE		 = 3
};
extern int dbg_verbose;

// #define DBG_STD(str) { if(elm::color::flags&elm::color::DEBUG) std::cout << elm::color::Debug::debugPrefix(__FILE__, __LINE__).chars() << str << elm::color::RCol << std::endl; }
 #define DBG_TEST(tested_cond, expected_cond) \
 	((tested_cond) == (expected_cond) ? color::IGre() : color::IRed()) << \
 	((tested_cond) ? "true" : "false") << color::RCol()

// DBG a warning
#ifdef DBG_WARNINGS
	#define DBGW(str) elm::cout << color::IYel() << "WARNING: " << str << color::RCol() << endl;
#else
	#define DBGW(str) DBGG(color::IYel() << "WARNING: " << str)
#endif

#endif