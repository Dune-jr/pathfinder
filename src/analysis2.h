#ifndef _ANALYSIS2_H
#define _ANALYSIS2_H

#include "oracle.h"

class Analysis2 : public DefaultAnalysis
{
public:
	Analysis2(WorkSpace *ws, PropList &props, int flags, int merge_thresold, int nb_cores)
		: DefaultAnalysis(ws, props, flags, merge_thresold, nb_cores) { }
private:
	void processCFG(CFG* cfg, bool use_initial_data);
	void I(Block* b, LockPtr<States> s);
};

#endif