/*
 * Debugging, printing tools
 */
#ifndef _DEBUG_H
#define _DEBUG_H

#include <elm/string/String.h>
#include <elm/string/AutoString.h>

// #define DBGG
// #define DBG_WARNINGS
#define DBG_NO_COLOR	   0b1 << 1
#define DBG_NO_INFO		   0b1 << 2
#define DBG_LINE_NB		   0b1 << 3
#define DBG_NO_TIME		   0b1 << 4
#define DBG_NO_PREDICATES  0b1 << 5
#define DBG_PREANALYSIS	   0b1 << 6
#define UNTESTED_CRITICAL true // Do not raise exception when executing untested code
#define DBG_SEPARATOR " "

#define DBG_VERBOSE_ALL			 0
#define DBG_VERBOSE_MINIMAL		 1
#define DBG_VERBOSE_RESULTS_ONLY 2
#define DBG_VERBOSE_NONE 		 3

extern int dbg_flags;
extern int dbg_verbose;
using namespace elm;

namespace debug {
namespace color
{
	inline elm::String RCol() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0m"; } // Reset colors

	inline elm::String Bla() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;30m"; } // Regular
	inline elm::String BBla() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[1;30m"; } // Bold
	inline elm::String UBla() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[4;30m"; } // Underline
	inline elm::String IBla() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;90m"; } // High intensity
	inline elm::String BIBla() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[1;90m"; } // Bold+High intensity
	inline elm::String On_Bla() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[40m"; } // Background
	inline elm::String On_IBla() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;100m"; } // High intensity background

	inline elm::String Red() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;31m"; }
	inline elm::String BRed() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[1;31m"; }
	inline elm::String URed() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[4;31m"; }
	inline elm::String IRed() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;91m"; }
	inline elm::String BIRed() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[1;91m"; }
	inline elm::String On_Red() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[41m"; }
	inline elm::String On_IRed() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;101m"; }

	inline elm::String Gre() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;32m"; }
	inline elm::String BGre() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[1;32m"; }
	inline elm::String UGre() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[4;32m"; }
	inline elm::String IGre() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;92m"; }
	inline elm::String BIGre() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[1;92m"; }
	inline elm::String On_Gre() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[42m"; }
	inline elm::String On_IGre() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;102m"; }

	inline elm::String Yel() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;33m"; }
	inline elm::String BYel() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[1;33m"; }
	inline elm::String UYel() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[4;33m"; }
	inline elm::String IYel() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;93m"; }
	inline elm::String BIYel() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[1;93m"; }
	inline elm::String On_Yel() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[43m"; }
	inline elm::String On_IYel() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;103m"; }

	inline elm::String Blu() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;34m"; }
	inline elm::String BBlu() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[1;34m"; }
	inline elm::String UBlu() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[4;34m"; }
	inline elm::String IBlu() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;94m"; }
	inline elm::String BIBlu() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[1;94m"; }
	inline elm::String On_Blu() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[44m"; }
	inline elm::String On_IBlu() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;104m"; }

	inline elm::String Pur() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;35m"; }
	inline elm::String BPur() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[1;35m"; }
	inline elm::String UPur() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[4;35m"; }
	inline elm::String IPur() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;95m"; }
	inline elm::String BIPur() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[1;95m"; }
	inline elm::String On_Pur() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[45m"; }
	inline elm::String On_IPur() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;105m"; }

	inline elm::String Cya() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;36m"; }
	inline elm::String BCya() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[1;36m"; }
	inline elm::String UCya() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[4;36m"; }
	inline elm::String ICya() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;96m"; }
	inline elm::String BICya() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[1;96m"; }
	inline elm::String On_Cya() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[46m"; }
	inline elm::String On_ICya() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;106m"; }

	inline elm::String Whi() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;37m"; }
	inline elm::String BWhi() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[1;37m"; }
	inline elm::String UWhi() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[4;37m"; }
	inline elm::String IWhi() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;97m"; }
	inline elm::String BIWhi() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[1;97m"; }
	inline elm::String On_Whi() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[47m"; }
	inline elm::String On_IWhi() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[0;107m"; }

	inline elm::String Bold() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[1m"; }
	inline elm::String NoBold() { return (dbg_flags&DBG_NO_COLOR) ? "" : "\e[21m"; }
} // color

class Debug
{
	enum {
		DEBUG_HEADERSIZE=20, // should be > 3
	};
	
public:
	static elm::String formattedDbgInfo(const char* file, int line){
		elm::String str = _ << file << ":" << line;
		
		if(str.length() > (int)DEBUG_HEADERSIZE)
		{
			str = str.substring(str.length() + 3 - DEBUG_HEADERSIZE);
			return _ << "..." << str;
		}
		else
		{
			elm::String whitespaces;
			for(unsigned int i = 0, len = str.length(); i < DEBUG_HEADERSIZE - len; i++)
				whitespaces = whitespaces.concat(elm::CString(" "));
			return _ << whitespaces << str;
		}		
		return str;
	}
	static bool shouldPrint(const elm::String& str)
	{
		if(dbg_flags&DBG_NO_PREDICATES)
		{	
			if(str.startsWith(_ << &color::Pur() << " "))
				return false;
			if(str.startsWith(_ << &color::BIPur() << " "))
				return false;
			if(str.startsWith(_ << &color::IPur() << " "))
				return false;
			if(str.startsWith(_ << &color::IYel() << "-"))
				return false;
			if(str.startsWith("Predicates generated:") || str.startsWith("|-> "))
				return false;
			if(str.startsWith(_ << &color::IBlu() << ""))
				return false;
			if(str.startsWith(_ << &color::Blu() << "  "))
				return false;
		}
		return true;
	}
	inline static elm::String dbgInfo(const char* file, int line)
	{
		static int line_nb = 0;
		if(dbg_flags&DBG_NO_INFO)
			return (dbg_flags&DBG_LINE_NB) ? color::Yel() << "[" << io::align(io::RIGHT, io::width(6, ++line_nb)) << "] " << color::RCol() : (elm::String)"";
		if(!(dbg_flags&DBG_LINE_NB)) return color::Yel() << "[" << Debug::formattedDbgInfo(file, line) << "] " << color::RCol();
	  	return color::Yel() << "[" << Debug::formattedDbgInfo(file, line) << "|" << io::align(io::RIGHT, io::width(6, ++line_nb)) << "] " << color::RCol(); 
	}
};
} // Debug class

// macros for debugging
#define DBG_INFO() color::Yel() << "[" << Debug::formattedDbgInfo(__FILE__, __LINE__) << "] " << color::RCol()
#define DBG_INFO_STD() color::Yel() << "[" << Debug::formattedDbgInfo(__FILE__, __LINE__).chars() << "] " << color::RCol()

#define DBG(str) { if(dbg_verbose == DBG_VERBOSE_ALL) {\
		elm::String stringed_str = _ << str;\
		if(Debug::shouldPrint(stringed_str)) \
			cout << Debug::dbgInfo(__FILE__, __LINE__) << stringed_str << color::RCol() << io::endl; } }
#define DBG_STD(str) { if(dbg_verbose == DBG_VERBOSE_ALL) {\
		elm::String stringed_str = _ << str;\
		if(Debug::shouldPrint(_ << str)) \
			std::cout << Debug::dbgInfo(__FILE__, __LINE__).chars() << str << color::RCol().chars() << io::endl; } }
#define DBG_TEST(tested_cond, expected_cond) \
	((tested_cond) == (expected_cond) ? "\033[92m" : "\033[91m") << \
	((tested_cond) ? "true" : "false") << "\033[0m"

#endif
