#ifndef _DEBUG_H
#define _DEBUG_H

// macros for debugging
#define DBG_INFO() "\033[33m[" << __FILE__ << ":" << __LINE__ << "]\033[0m "
#define DBG(str) cout << DBG_INFO() << str << io::endl;
#define DBG_TEST(tested_cond, expected_cond) \
	(tested_cond == expected_cond ? "\033[92m" : "\033[91m") << \
	(tested_cond ? "true" : "false") << "\033[0m"
	
// Color macros
#define COLOR_RCol "\e[0m" // Reset colors

#define COLOR_Bla "\e[0;30m" // Regular
#define COLOR_BBla "\e[1;30m" // Bold
#define COLOR_UBla "\e[4;30m" // Underline
#define COLOR_IBla "\e[0;90m" // High intensity
#define COLOR_BIBla "\e[1;90m" // Bold+High intensity
#define COLOR_On_Bla "\e[40m" // Background
#define COLOR_On_IBla "\e[0;100m" // High intensity background

#define COLOR_Red "\e[0;31m"
#define COLOR_BRed "\e[1;31m"
#define COLOR_URed "\e[4;31m"
#define COLOR_IRed "\e[0;91m"
#define COLOR_BIRed "\e[1;91m"
#define COLOR_On_Red "\e[41m"
#define COLOR_On_IRed "\e[0;101m"

#define COLOR_Gre "\e[0;32m"
#define COLOR_BGre "\e[1;32m"
#define COLOR_UGre "\e[4;32m"
#define COLOR_IGre "\e[0;92m"
#define COLOR_BIGre "\e[1;92m"
#define COLOR_On_Gre "\e[42m"
#define COLOR_On_IGre "\e[0;102m"

#define COLOR_Yel "\e[0;33m"
#define COLOR_BYel "\e[1;33m"
#define COLOR_UYel "\e[4;33m"
#define COLOR_IYel "\e[0;93m"
#define COLOR_BIYel "\e[1;93m"
#define COLOR_On_Yel "\e[43m"
#define COLOR_On_IYel "\e[0;103m"

#define COLOR_Blu "\e[0;34m"
#define COLOR_BBlu "\e[1;34m"
#define COLOR_UBlu "\e[4;34m"
#define COLOR_IBlu "\e[0;94m"
#define COLOR_BIBlu "\e[1;94m"
#define COLOR_On_Blu "\e[44m"
#define COLOR_On_IBlu "\e[0;104m"

#define COLOR_Pur "\e[0;35m"
#define COLOR_BPur "\e[1;35m"
#define COLOR_UPur "\e[4;35m"
#define COLOR_IPur "\e[0;95m"
#define COLOR_BIPur "\e[1;95m"
#define COLOR_On_Pur "\e[45m"
#define COLOR_On_IPur "\e[0;105m"

#define COLOR_Cya "\e[0;36m"
#define COLOR_BCya "\e[1;36m"
#define COLOR_UCya "\e[4;36m"
#define COLOR_ICya "\e[0;96m"
#define COLOR_BICya "\e[1;96m"
#define COLOR_On_Cya "\e[46m"
#define COLOR_On_ICya "\e[0;106m"

#define COLOR_Whi "\e[0;37m"
#define COLOR_BWhi "\e[1;37m"
#define COLOR_UWhi "\e[4;37m"
#define COLOR_IWhi "\e[0;97m"
#define COLOR_BIWhi "\e[1;97m"
#define COLOR_On_Whi "\e[47m"
#define COLOR_On_IWhi "\e[0;107m"

#endif
