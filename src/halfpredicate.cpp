#include <elm/io/Output.h>
#include "halfpredicate.h"

HalfPredicate::HalfPredicate(condoperator_t opr, const Operand& opd) : _opr(opr)
{
	_opd = opd.copy();
}
HalfPredicate::HalfPredicate(const HalfPredicate& p) : _opr(p._opr)
{
	_opd = p._opd->copy();
}
HalfPredicate::~HalfPredicate()
{
	delete _opd;
}

/**
 * @fn inline condoperator_t HalfPredicate::opr() const;
 * @brief Get the operator of a half-predicate
 */

/**
 * @fn inline Operand& HalfPredicate::opd() const;
 * @brief Get the operand of a half-predicate
 */

HalfPredicate* HalfPredicate::copy() const
{
	return new HalfPredicate(*this);
}

HalfPredicate& HalfPredicate::operator=(const HalfPredicate& p)
{
	this->_opr = p._opr;
	delete this->_opd;
	this->_opd = p._opd->copy();
	return *this;
}

bool HalfPredicate::operator==(const HalfPredicate& p) const
{
	return (_opr == p._opr) && (*_opd == *p._opd);
}

io::Output& operator<<(io::Output& out, const HalfPredicate& p)
{
	return (out << (condoperator_t)p._opr << " " << *p._opd);
}
