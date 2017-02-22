/*
 * Constant is an enhanced int32 type allowing other constants such as the value of the Stack Pointer at the start of the program
 */

#include "constant.h"
#include "../debug.h"

/**
 * @class Constant
 * @brief A constant of the abstract interpretation analysis. Can be relative to SP0 (the initial value of the stack pointer)
 */
const Constant SP(0, CONSTANT_PLUS_SP);

Constant::Constant() : _val(0), _kind(CONSTANT_INVALID)/*, _sign(SIGN_POSITIVE)*/ { }
// Constant::Constant(int32 val, bool relative, bool sign)
// 	: _val(val), _kind(relative ? (sign ? CONSTANT_PLUS_SP0 : CONSTANT_MINUS_SP0) : CONSTANT_ABSOLUTE) { }
Constant::Constant(int32 val, constant_kind_t kind) : _val(val), _kind(kind) { }
Constant::Constant(const Constant& c) : _val(c._val), _kind(c._kind)/*, _sign(c._sign)*/ { }

Constant& Constant::operator=(const Constant& c)
{
	_val = c._val;
	_kind = c._kind;
	return *this;
}
Constant& Constant::operator=(int32 val)
{
	_val = val;
	_kind = CONSTANT_ABSOLUTE;
	return *this;
}
bool Constant::operator==(const Constant& c) const
{
	if(_kind != c._kind)
		return false; // kinds do not match
	switch(_kind)
	{
		case CONSTANT_INVALID:
			return true; // no need to test anything else
		case CONSTANT_ABSOLUTE:
		case CONSTANT_PLUS_SP:
		case CONSTANT_MINUS_SP:
			return _val == c._val;
		default:
			return false; // never read, but gcc is too stupid
	}
}
Constant Constant::operator+(const Constant& c) const
{
	int32 new_val = _val + c._val;
	switch(_kind)
	{
		case CONSTANT_ABSOLUTE:
			return Constant(new_val, constant_kind_t(c._kind));
		case CONSTANT_INVALID:
			return Constant();
		default: // relative
			switch(c._kind)
			{
				case CONSTANT_ABSOLUTE:
					return Constant(new_val, constant_kind_t(_kind));
				case CONSTANT_INVALID:
					return Constant();
				default: // relative
					if(sign() == c.sign())
						return Constant(); // +-2*SP
					return Constant(new_val, CONSTANT_ABSOLUTE); // SP + -SP
			}
	}
}
Constant Constant::operator-(const Constant& c) const
{
	return *this + (-c);
}
// unary -
Constant Constant::operator-() const
{
	return Constant(-_val, isRelative() ? (_kind == CONSTANT_PLUS_SP ? CONSTANT_MINUS_SP : CONSTANT_PLUS_SP) : (constant_kind_t)_kind);
}
// bit-to-bit NOT
Constant Constant::operator~() const
{
	if(!isAbsolute())
		return Constant();
	return Constant(~_val, CONSTANT_ABSOLUTE);
}
Constant Constant::operator*(const Constant& c) const
{
	if(c == 0 || *this == 0)
		return Constant(0);
	if(c == 1)
		return *this;
	if(c == -1)
		return -*this;
	if(*this == 1)
		return c;
	if(*this == -1)
		return -c;
	if(isAbsolute() && c.isAbsolute())
		return Constant(_val * c._val);
	return Constant();
}
Constant Constant::operator/(const Constant& c) const
{
	if(c == 0)
		return Constant();
	if(*this == 0)
		return Constant(0);
	if(c == 1)
		return *this; // id
	if(c == -1)
		return -*this;
	if(isAbsolute() && c.isAbsolute())
		return Constant(_val / c._val);
	return Constant();
}
Constant Constant::operator%(const Constant& c) const
{
	if(c == 0 || !c.isAbsolute())
		return Constant();
	if(*this == 0)
		return Constant(0);
	if(c == 1)
		return *this; // id
	if(isAbsolute())
		return Constant(_val % c._val);
	return Constant();
}
Constant Constant::operator&(const Constant& c) const
{
	if(c == 0 || *this == 0)
		return 0;
	if(isAbsolute() && c.isAbsolute())
		return Constant(_val & c._val);
	return Constant();
}
Constant Constant::operator|(const Constant& c) const
{
	if(c == 0)
		return *this;
	if(*this == 0)
		return c;
	if(isAbsolute() && c.isAbsolute())
		return Constant(_val | c._val);
	return Constant();
}
Constant Constant::operator^(const Constant& c) const
{
	if(c == 0)
		return *this;
	if(*this == 0)
		return c;
	if(isAbsolute() && c.isAbsolute())
		return Constant(_val ^ c._val);
	return Constant();
}
Constant Constant::operator<<(const Constant& c) const
{
	if(c == 0)
		return *this; // id
	if(!c.isAbsolute() || !isAbsolute())
		return Constant();
	return Constant(_val << c._val);
}
Constant Constant::operator>>(const Constant& c) const
{
	if(c == 0)
		return *this; // id
	if(!c.isAbsolute() || !isAbsolute())
		return Constant();
	return Constant(_val >> c._val);
}
Constant& Constant::operator+=(const Constant& c)
{
	return (*this = *this + c);
}
Constant& Constant::operator-=(const Constant& c)
{
	return (*this = *this - c);
}
bool Constant::operator<(const Constant& c) const
{
	if(_kind != c._kind)
		return _kind < c._kind; // doesn't make sense, but doesn't have to
	switch(_kind)
	{
		case CONSTANT_PLUS_SP:
		case CONSTANT_MINUS_SP:
			if(sign() < c.sign())
				return false;
			if(sign() > c.sign())
				return true;
		case CONSTANT_ABSOLUTE: // or CONSTANT_RELATIVE with same sign
			return val() < c.val();
		default: // CONSTANT_INVALID:
			return false;
	}
}
bool Constant::operator>(const Constant& c) const
{
	if(_kind != c._kind)
		return _kind > c._kind; // doesn't make sense, but doesn't have to
	switch(_kind)
	{
		case CONSTANT_PLUS_SP:
		case CONSTANT_MINUS_SP:
			if(sign() < c.sign())
				return true;
			if(sign() > c.sign())
				return false;
		case CONSTANT_ABSOLUTE: // or CONSTANT_RELATIVE with same sign
			return val() > c.val();
		default: // CONSTANT_INVALID:
			return false;
	}
}

Output& Constant::print(Output& out) const
{
	switch(_kind)
	{
		case CONSTANT_ABSOLUTE:
			if(_val >= 64 || _val <= -63) // print large values in hex
				return (out << "0x" << elm::io::hex(_val));
			return (out << _val);
		case CONSTANT_MINUS_SP:
			out << "-";
		case CONSTANT_PLUS_SP:
			out << "SP";
			if(_val < 0)
				return (out << "-" << -_val);
			else
				return (out << "+" << _val);
		default:
			return (out << "(invalid cst)");
	}
}
