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