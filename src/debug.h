/*
 * Debugging, printing tools
 */
#ifndef _DEBUG_H
#define _DEBUG_H

#define ELM_NO_DBG // do not use standard macro names
#include <elm/log/Log.h>
#define DBG ELM_DBGLN // default DBG to ELM_DBGLN. If we want to get the no-newline behavior, we have to use ELM_DBG (or elm::cout)
#include <ostream>
#include <elm/string/String.h>
#include <elm/string/AutoString.h>

// v1:debug loop analysis and many other
// v2: macro debugs
#define DBGG(str) { if(dbg_verbose < DBG_VERBOSE_RESULTS_ONLY) elm::cout << elm::log::Debug::debugPrefix(__FILE__, __LINE__) << str << elm::color::RCol() << elm::io::endl; }
// #define DBG_WARNINGS

// #define DBG_NO_COLOR	   0b1 << 1
// #define DBG_NO_INFO		   0b1 << 2
// #define DBG_LINE_NB		   0b1 << 3
#define DBG_NO_TIME		   0b1 << 4
// #define DBG_NO_PREDICATES  0b1 << 5
#define DBG_PREANALYSIS	   0b1 << 6
#define DBG_PROGRESS	   0b1 << 7
#define DBG_PRINT_FLOWINFO 0b1 << 8
#define DBG_AVG_IP_LENGTH  0b1 << 9
#define UNTESTED_CRITICAL true // Raise exceptions when executing untested code
#define DBG_SEPARATOR " "

#define DBG_VERBOSE_ALL			 0
#define DBG_VERBOSE_MINIMAL		 1
#define DBG_VERBOSE_RESULTS_ONLY 2
#define DBG_VERBOSE_NONE 		 3

extern int dbg_flags;
extern int dbg_verbose;

// #define DBG_STD(str) { if(elm::color::flags&elm::color::DEBUG) std::cout << elm::color::Debug::debugPrefix(__FILE__, __LINE__).chars() << str << elm::color::RCol << std::endl; }
/* #define DBG_TEST(tested_cond, expected_cond) \
 	((tested_cond) == (expected_cond) ? color::IGre() : color::IRed()) << \
 	((tested_cond) ? "true" : "false") << color::RCol()*/

#endif
