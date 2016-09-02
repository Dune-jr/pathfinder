#ifndef HALFPREDICATE_H
#define HALFPREDICATE_H

#include "predicate.h"

// HalfPredicate class
class HalfPredicate
{
public:
	HalfPredicate(condoperator_t opr, const Operand& opd);
	HalfPredicate(const HalfPredicate& p);
	~HalfPredicate();
	
	inline condoperator_t opr() const { return (condoperator_t)_opr; }
	inline Operand& opd() const { return *_opd; }
	
	HalfPredicate* copy() const;

	HalfPredicate& operator=(const HalfPredicate& p);
	bool operator==(const HalfPredicate& p) const;
	inline bool operator!=(const HalfPredicate& p) const { return !(*this == p); }
	friend io::Output& operator<<(io::Output& out, const HalfPredicate& p);
	
private:
	char _opr; // operator, on one byte to save memory
	Operand* _opd; // left operand
};

#endif