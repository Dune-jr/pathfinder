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
	features for virtualized CFGs
*/

#include "cfg_features.h"

/**
 * @class LoopHeaderIter
 * @author Jordy Ruiz 
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
		ASSERTP(!(++citer), cfg << " must have max. 1 caller (CFG not virtualized or recursive?)");
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
		ASSERTP(!(++citer), cfg << " must have max. 1 caller (CFG not virtualized or recursive?)");
		return rtn;
	}
	return def; // no caller: exiting main CFG
}
