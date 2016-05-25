/*
	features for virtualized CFGs
*/

#include "cfg_features.h"

/**
 * @class LoopHeaderIter
 * @brief Iterates on the enclosing loops of a Block, including those of caller CFGs (only works with a virtualized CFG!)
 */
/**
 * @brief return unique caller of CFG (none if no caller exists) in virtualized scenario
 */
Option<Block*> getCaller(CFG* cfg) // { return citer ? elm::some(theOnly(cfg->callers())) : elm::none(); }
{
	CFG::CallerIter citer(cfg->callers());
	if(citer)
	{
		Block* rtn = citer;
		ASSERTP(!(++citer), "must be max. 1 caller (CFG not virtualized or recursive?)");
		return elm::some(rtn);
	}
	return elm::none; // no caller: exiting main CFG
}

/**
 * @brief return unique caller of CFG (defs if no caller exists) in virtualized scenario
 */
Block* getCaller(CFG* cfg, Block* def)
{
	CFG::CallerIter citer(cfg->callers());
	if(citer)
	{
		Block* rtn = citer;
		ASSERTP(!(++citer), "must be max. 1 caller (CFG not virtualized or recursive?)");
		return rtn;
	}
	return def; // no caller: exiting main CFG
}