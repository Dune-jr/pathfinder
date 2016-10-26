/**
 * SMT Job for multithreading
 */

#ifndef SMT_JOB_H
#define SMT_JOB_H

#include <elm/sys/Thread.h>

template<class SMT> class SMTJob : public elm::sys::Runnable {
	typedef Pair<const Analysis::State*, Option<Analysis::Path*> > pair_t;
	typedef Vector<pair_t > data_t;

public:
	SMTJob(int flags) : flags(flags) { }

	void run() {
		for(data_t::Iter iter(data); iter; iter++) {
			SMT smt(flags);
			const Analysis::State* s = (*iter).fst;
			data[iter].snd = smt.seekInfeasiblePaths(*s);
		}
	}

	void addState(const Analysis::State* s)
		{ data.push(pair_t(s, elm::none)); }

	const data_t& getResults() const
		{ return data; }

	typedef data_t::Iter Iterator;

private:
	int flags;
	data_t data;
};

#endif
