#include "smt_job.h"

SMTJob::SMTJob(const Analysis::State& state_to_analyse) : s(state_to_analyse)
{ }

void SMTJob::run()
{
	SMT smt;
	result = smt.seekInfeasiblePaths(s); // SMT call
}