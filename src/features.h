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
 
#ifndef _PATHFINDER_FEATURES_H
#define _PATHFINDER_FEATURES_H

#include <otawa/proc/AbstractFeature.h>

namespace otawa {

	// PathFinder analysis parameters
	extern Identifier<int> ANALYSIS_FLAGS; // mandatory
	extern Identifier<int> MERGE_THRESOLD; // optional
	extern Identifier<int> NB_CORES; // optional

	// PathFinder output (on the called CFG)
	extern Identifier<Vector<DetailedPath> > INFEASIBLE_PATHS;

	// PathFinder provided features
	extern p::feature OLD_INFEASIBLE_PATHS_FEATURE; // v1
	extern p::feature INFEASIBLE_PATHS_FEATURE; // v2

} // otawa

#endif