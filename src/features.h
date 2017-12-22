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