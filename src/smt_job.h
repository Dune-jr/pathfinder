#ifndef _SMT_JOB_H
#define _SMT_JOB_H

#include <elm/sys/Thread.h>
#include "smt.h"
#include "analysis.h"

// TODO: I have to write public explicitly, dunno why
class SMTJob : public elm::sys::Runnable
{
public:
	SMTJob(const Analysis::State& state_to_analyse);
	inline Option<Analysis::Path> getResult() const { return result; }
	inline Analysis::State getState() const { return s; }
	void run();

private:
	Analysis::State s;
	Option<Analysis::Path> result;
};

#endif