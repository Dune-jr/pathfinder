#include "pretty_printing.h"

/**
 * Print n indents to the given output
 */
void addIndents(elm::io::Output& out, int n)
{
	for(int i=0; i<n; i++)
		out << "\t";
}

#ifndef NO_UTF8
	// #define UTF8_Block_start_delim "⟪"
	// #define UTF8_Block_end_delim "⟫"
	// #define UTF8_Block_start_delim "❮"
	// #define UTF8_Block_start_delim "｢"
	// #define UTF8_Block_start_delim "⌈"
	// #define UTF8_Block_end_delim "⌋"
	#define UTF8_Block_end_delim "⟩"
	#define UTF8_Block_start_delim "⟨"
#else
	#define UTF8_Block_start_delim "<"
	#define UTF8_Block_end_delim ">"
#endif

/**
  * @fn Output& otawa::operator<<(Output& out, otawa::Block* b)
  * @brief Custom pretty printing of otawa::Block
  */
Output& otawa::operator<<(Output& out, otawa::Block* b)
{
	if(b->isBasic())
		return (out << UTF8_Block_start_delim << b->cfg() << ":" << b->index() << UTF8_Block_end_delim);
	if(b->isEntry())
		return (out << UTF8_Block_start_delim "^" << b->cfg() << UTF8_Block_end_delim);
	if(b->isExit())
		return (out << UTF8_Block_start_delim << b->cfg() << ":" << b->index() << "$" UTF8_Block_end_delim);
	if(b->isUnknown())
		return (out << UTF8_Block_start_delim "?" << b->cfg() << ":" << b->index() << "?" UTF8_Block_end_delim);
	if(b->isVirtual())
		return (out << UTF8_Block_start_delim "~" << b->cfg() << ":" << b->index() << UTF8_Block_end_delim);
	if(b->isCall())
#		ifndef NO_UTF8
			return (out << UTF8_Block_start_delim "@" << b->cfg() << ":" << b->index() << "→" << b->toSynth()->callee() << UTF8_Block_end_delim);
#		else
			return (out << UTF8_Block_start_delim "@" << b->cfg() << ":" << b->index() << "->" << b->toSynth()->callee() << UTF8_Block_end_delim);
#		endif
	assert(false);
}
