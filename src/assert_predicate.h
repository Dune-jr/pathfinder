#ifndef ASSERT_PREDICATE_H
#define ASSERT_PREDICATE_H

#include "analysis_state.h"
#include "struct/operand.h"

class SPEquals
{
public:
	inline bool operator()(const Analysis::State& s, const Operand* x) const
		{ return s.getLocalVariables()[s.getSP()] == x; }
};

class SPCanEqual
{
public:
	inline bool operator()(const Analysis::State& s, const OperandConst* x) const
		{ return ! (s.getLocalVariables()[s.getSP()]->isConstant() && s.getLocalVariables()[s.getSP()] != x); }
};

#endif