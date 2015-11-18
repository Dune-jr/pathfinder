/*
 * Debugging, printing tools
 */
#ifndef _ELM_DEBUG_H
#define _ELM_DEBUG_H

#include <elm/string/CString.h>
#include <elm/io/Output.h>
#include <ostream>

namespace elm
{
	namespace color
	{
		extern int flags;
		enum
		{
			DEBUG=		  1 << 1, // print any debugs at all
			SOURCE_INFO=  1 << 2, // show source file and line info prefix
			NUMBERING=	  1 << 3, // number debug outputs
			COLOR=		  1 << 4, // use colors
			COLORS=COLOR,
		};

		class Color
		{
		public:
			Color(const elm::CString& str) : _str(str) { }
			inline elm::CString operator()() const // Red()
				{ return (flags&COLOR) ? _str : ""; }
			inline operator elm::CString() const // (CString)Red
				{ return (*this)(); }
			inline operator const char*() const // (const char*)Red
				{ return (*this)().chars(); }
			friend inline elm::io::Output& operator<<(elm::io::Output& out, const Color& color) // elm::cout << Red
				{ return (out << color()); }
			friend inline std::ostream& operator<<(std::ostream& out, const Color& color) // std::cout << Red
				{ return out << color().chars(); }
		private:
			elm::CString _str;
		}; // Color class

		//			Regular			 Bold              Underline         High Intensity    Bold+High Intens.  Background        High Intens. Background         
		const Color Bla("\e[0;30m"), BBla("\e[1;30m"), UBla("\e[4;30m"), IBla("\e[0;90m"), BIBla("\e[1;90m"), On_Bla("\e[40m"), On_IBla("\e[0;100m");
		const Color Red("\e[0;31m"), BRed("\e[1;31m"), URed("\e[4;31m"), IRed("\e[0;91m"), BIRed("\e[1;91m"), On_Red("\e[41m"), On_IRed("\e[0;101m");
		const Color Gre("\e[0;32m"), BGre("\e[1;32m"), UGre("\e[4;32m"), IGre("\e[0;92m"), BIGre("\e[1;92m"), On_Gre("\e[42m"), On_IGre("\e[0;102m");
		const Color Yel("\e[0;33m"), BYel("\e[1;33m"), UYel("\e[4;33m"), IYel("\e[0;93m"), BIYel("\e[1;93m"), On_Yel("\e[43m"), On_IYel("\e[0;103m");
		const Color Blu("\e[0;34m"), BBlu("\e[1;34m"), UBlu("\e[4;34m"), IBlu("\e[0;94m"), BIBlu("\e[1;94m"), On_Blu("\e[44m"), On_IBlu("\e[0;104m");
		const Color Pur("\e[0;35m"), BPur("\e[1;35m"), UPur("\e[4;35m"), IPur("\e[0;95m"), BIPur("\e[1;95m"), On_Pur("\e[45m"), On_IPur("\e[0;105m");
		const Color Cya("\e[0;36m"), BCya("\e[1;36m"), UCya("\e[4;36m"), ICya("\e[0;96m"), BICya("\e[1;96m"), On_Cya("\e[46m"), On_ICya("\e[0;106m");
		const Color Whi("\e[0;37m"), BWhi("\e[1;37m"), UWhi("\e[4;37m"), IWhi("\e[0;97m"), BIWhi("\e[1;97m"), On_Whi("\e[47m"), On_IWhi("\e[0;107m");

		// const Color Reset("\e[0m");// Reset colors //TODO: which one is better? I think Reset may prevent "using namespace elm::color"
		const Color RCol("\e[0m"); 	  // Reset colors
		const Color Bold("\e[1m"); 	  // Switch bold text ON
		const Color Dim("\e[2m"); 	  // Switch darker text ON
		const Color NoBold("\e[21m"); // Switch bold text OFF
		const Color NoDim("\e[22m");  // Switch darker text OFF
		// No support for Blink (\e[5m) as it is deprecated
	} // color namespace
} // elm namespace

#define ELM_DBG(str) { if(elm::color::flags&elm::color::DEBUG) elm::cout << /*dbgInfo(__FILE__, __LINE__) <<*/ str << elm::color::RCol << elm::io::endl; }
#define ELM_DBGLN(str) { if(elm::color::flags&elm::color::DEBUG) ELM_DBG(str << elm::color::RCol << elm::io::endl); }

#ifndef ELM_NO_DBG
	#define DBG ELM_DBG
	#define DBGLN ELM_DBGLN
#endif

#endif
