#ifndef LOOP_BOUND_H
#define LOOP_BOUND_H

class LoopBound {
public:
	LoopBound() : n(-1), b(false) { }
	LoopBound(int bound) : n(bound), b(true) { }

	inline bool isBounded() const { return b; }
	inline int getBound() const { ASSERT(b); return n; }
	inline int operator*() const { return getBound(); }

	operator bool() const { return b; }
	inline void operator=(const LoopBound& lb) { n = lb.n, b = lb.b; }
	bool operator<(const LoopBound& lb) const { return (!(lb.b) && b) || (lb.b == b && n < lb.n); }  // better loop bound
	bool operator>(const LoopBound& lb) const { return (!b && lb.b) || (lb.b == b && n > lb.n); }  // worse loop bound

	friend inline io::Output& operator<<(io::Output& out, const LoopBound& lb) {
		if(lb.isBounded())
			return out << lb.getBound();
		else
			return out << "<unbounded>";
	}
	
private:
	int n;
	bool b; // bounded
};

#endif