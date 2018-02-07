/*
 *	
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2006-2018, IRIT UPS.
 * 
 *	OTAWA is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	OTAWA is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OTAWA; if not, write to the Free Software 
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
/*
 * Debugging, printing tools
 */
#ifndef _DEBUG_H
#define _DEBUG_H

#define ELM_NO_DBG // do not use standard macro names
#include <elm/log/Log.h>
#define DBG(str) { ELM_DBGV(1, str << elm::io::endl) }
// v1:debug loop analysis and many other
// v2: macro debugs
#define DBGG ELM_DBGLN

#include <ostream>
#include <elm/string/String.h>
#include <elm/string/AutoString.h>

// #define DBG_WARNINGS

#define UNTESTED_CRITICAL true // Raise exceptions when executing untested code if true
#define UNTESTED_CODE(str){\
	DBG(color::BIRed() << "Untested " str " running!")\
	ASSERT(!UNTESTED_CRITICAL);\
}

#define DBG_SEPARATOR " "

enum
{
	DBG_RESULT_IPS	    = 1 << 3,
	DBG_DETERMINISTIC   = 1 << 4,
	DBG_PREANALYSIS	    = 1 << 6,
	// DBG_PROGRESS	    = 1 << 7,
	DBG_FORMAT_FLOWINFO = 1 << 8,
	DBG_DETAILED_STATS  = 1 << 9,
	DBG_ONLY_LOOP_BOUNDS  = 1 << 10,
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
extern int dbg_;

// #define DBG_STD(str) { if(elm::color::flags&elm::color::DEBUG) std::cout << elm::color::Debug::debugPrefix(__FILE__, __LINE__).chars() << str << elm::color::RCol << std::endl; }
 #define DBG_TEST(tested_cond, expected_cond) \
 	((tested_cond) == (expected_cond) ? color::IGre() : color::IRed()) << \
 	((tested_cond) ? "true" : "false") << color::RCol()

// DBG a warning
#ifdef DBG_WARNINGS
	#define DBGW(str) { elm::cout << color::IYel() << "WARNING: " << str << color::RCol() << endl; }
#else
	#define DBGW(str) { DBGG(color::IYel() << "WARNING: " << str) }
#endif

#endif