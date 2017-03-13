#ifndef _ANALYSIS_STATES_H
#define _ANALYSIS_STATES_H

#include "analysis_state.h"

class Analysis::States : public elm::Lock {
public:
	class Iter;
	inline States() { }
	inline States(int cap) : s(cap) { }
	inline States(const Vector<Analysis::State>& state_vector) : s(state_vector) { }
	inline States(const States& ss) : s(ss.s) { }
	// return the unique state, or bottom if none. it is an error to call this when s.count() > 1
	inline const State& one() const { ASSERT(s.count() <= 1); return s ? s.first() : bottom; }
	// inline State& one() { ASSERT(s.count() <= 1); return s ? s.first() : bottom; }
	inline bool isEmpty() const { return s.isEmpty(); }
	inline int count() const { return s.count(); }
	inline const State& first() const { return s.first(); }
	inline const Vector<State>& states() const { return s; }
	inline Vector<State>& states() { return s; }
	inline void push(const Analysis::State& state) { return s.push(state); }
	inline void remove(const Iter& i) { s.remove(i); }

	void apply(const States& ss, VarMaker& vm, bool local_sp, bool dbg = true);
	void appliedTo(const State& s, VarMaker& vm);
	void minimize(VarMaker& vm, bool clean) const;
	inline void removeTautologies(void) 			{ for(Iter i(this->s); i; i++) s[i].removeTautologies(); }
	inline void prepareFixPoint(void) 				{ for(Iter i(this->s); i; i++) s[i].prepareFixPoint(); }
	void finalizeLoop(OperandIter* n, VarMaker& vm)	{ for(Iter i(this->s); i; i++) s[i].finalizeLoop(n, vm); }
	inline void widening(const Operand* n) { ASSERT(s.count() <= 1); if(s.count() == 1) s[0].widening(n); } // needs max one state
	void checkForSatisfiableSP() const;

	inline void resetSP() { for(Iter i(this->s); i; i++) s[i].resetSP(); }
	inline void onCall(SynthBlock* sb)   { for(Iter i(this->s); i; i++) s[i].onCall(sb); }
	inline void onReturn(SynthBlock* sb) { for(Iter i(this->s); i; i++) s[i].onReturn(sb); }
	inline void onLoopExit (Block* b)    { for(Iter i(this->s); i; i++) s[i].onLoopExit(b); }
	// inline void onLoopEntry(Block* b)    { for(Iter i(this->s); i; i++) s[i].onLoopEntry(b); }
	void onLoopExit (Edge* e) {
		Block* h = LOOP_EXIT_EDGE(e);
		for(LoopHeaderIter i(e->source()); i.item() != h; i++) 
			onLoopExit(*i);
		onLoopExit(h);
	}

	// inline operator Vector<State>() { return s; }
	inline State& operator[](int i) { return s[i]; }
	inline State& operator[](const Iter& i) { return s[i]; }
	inline States& operator=(const Vector<State>& sv) { s = sv; return *this; }
	inline States& operator=(const States& ss) { s = ss.s; return *this; }
	inline friend io::Output& operator<<(io::Output& out, const States& ss) { return out << ss.s; };
	elm::String dump() const {
		elm::String rtn;
		for(Iter i(this->s); i; i++)
			rtn = rtn.concat(i->dumpEverything());
		return rtn;
	};

	class Iter: public Vector<State>::Iter {
	public:
		Iter(const States& ss) : Vector<State>::Iter(ss.s) { }
		Iter(const Vector<State>& sv) : Vector<State>::Iter(sv) { }
	};

private:
	Vector<State> s;
};

#endif