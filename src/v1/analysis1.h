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
 
#ifndef _ANALYSIS1_H
#define _ANALYSIS1_H

#include "../oracle.h"

class Analysis1 : public DefaultAnalysis, public otawa::Processor
{
	using Analysis::flags;
	using Analysis::progress;

	// otawa::Processor inherited methods
public:
	Analysis1(AbstractRegistration& _reg = reg) : DefaultAnalysis(), otawa::Processor(_reg) { }
	virtual void configure(const PropList &props) { Processor::configure(props); Analysis::configure(props); }
	static p::declare reg;

protected:
	virtual void processWorkSpace(WorkSpace *ws) { Analysis::processWorkSpace(ws); }

	// some private methods
private:
	void processCFG(CFG* cfg, bool use_initial_data);
	void I(Block* b, LockPtr<States> s);
};

#endif