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
Option<Block*> getCaller(CFG* cfg)
{
	CFG::CallerIter citer(cfg->callers());
	if(citer)
	{
		Block* rtn = citer;
		ASSERTP(!(++citer), "must be max. 1 caller (CFG not virtualized?)");
		return elm::some(rtn);
	}
	else // no caller: exiting main CFG
		return elm::none;
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
		ASSERTP(!(++citer), "must be max. 1 caller (CFG not virtualized?)");
		return rtn;
	}
	else // no caller: exiting main CFG
		return def;
}