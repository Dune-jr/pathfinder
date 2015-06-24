/*
 * Constant is an enhanced int32 type allowing other constants such as the value of the Stack Pointer at the start of the program
 */

#include "constant.h"
#include "debug.h"

using namespace elm;

Constant::Constant() : _val(0), _kind(CONSTANT_INVALID), _sign(SIGN_POSITIVE) { }
Constant::Constant(t::int32 val, bool relative, bool sign) : _val(val), _kind(relative ? CONSTANT_RELATIVE : CONSTANT_ABSOLUTE), _sign(sign) { }
Constant::Constant(t::int32 val, constant_kind_t kind, bool sign) : _val(val), _kind(kind), _sign(sign) { }
Constant::Constant(const Constant& c) : _val(c._val), _kind(c._kind), _sign(c._sign) { }

Constant& Constant::operator=(const Constant& c)
{
	_val = c._val;
	_kind = c._kind;
	_sign = c._sign;
	return *this;
}
Constant& Constant::operator=(t::int32 val)
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
			return _val == c._val;
		case CONSTANT_RELATIVE:
			return (_val == c._val) && (_sign == c._sign);
		default:
			assert(false);
	}
}
Constant Constant::operator+(const Constant& c) const
{
	t::int32 new_val = _val + c._val;
	switch(_kind)
	{
		case CONSTANT_ABSOLUTE:
			switch(c._kind)
			{
				case CONSTANT_ABSOLUTE:
					return Constant(new_val, CONSTANT_ABSOLUTE);
				case CONSTANT_RELATIVE:
					return Constant(new_val, CONSTANT_RELATIVE, c._sign);
				default:
					return Constant();
			}
		case CONSTANT_RELATIVE:
			switch(c._kind)
			{
				case CONSTANT_ABSOLUTE:
					return Constant(new_val, CONSTANT_RELATIVE, _sign);
				case CONSTANT_RELATIVE:
					if(_sign == c._sign)
						return Constant(); // +-2SP
					return Constant(new_val, CONSTANT_ABSOLUTE); // SP + -SP
				default:
					return Constant();
			}
		default:
			return Constant(); // invalid
	}
}
Constant Constant::operator-(const Constant& c) const
{
	t::int32 new_val = _val - c._val;
	switch(_kind)
	{
		case CONSTANT_ABSOLUTE:
			switch(c._kind)
			{
				case CONSTANT_ABSOLUTE:
					return Constant(new_val, CONSTANT_ABSOLUTE);
				case CONSTANT_RELATIVE:
					return Constant(new_val, CONSTANT_RELATIVE, !c._sign);
				default:
					return Constant();
			}
		case CONSTANT_RELATIVE:
			switch(c._kind)
			{
				case CONSTANT_ABSOLUTE:
					return Constant(new_val, CONSTANT_RELATIVE, _sign);
				case CONSTANT_RELATIVE:
					if(_sign != c._sign)
						return Constant(); // +-2SP
					return Constant(new_val, CONSTANT_ABSOLUTE); // SP - SP or -SP - -SP
				default:
					return Constant();
			}
		default:
			return Constant(); // invalid
	}
}
// unary -
Constant Constant::operator-() const
{
	return Constant(-_val, _kind, !_sign);
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
bool Constant::operator>(const Constant& c) const
{
	if(kind() != c.kind())
		return kind() > c.kind(); // doesn't make sense, but doesn't need to
	switch(kind())
	{
		case CONSTANT_RELATIVE:
			if(sign() > c.sign())
				return true;
			if(sign() < c.sign())
				return false;
		case CONSTANT_ABSOLUTE: // or CONSTANT_RELATIVE with same sign
			return val() > c.val();
		default: // CONSTANT_INVALID
			return false;
	}

	return kind() > c.kind();
}
io::Output& Constant::print(io::Output& out) const
{
	switch(_kind)
	{
		case CONSTANT_ABSOLUTE:
			if(_val >= 64 || _val <= -63) // print large values in hex
				return (out << "0x" << io::hex(_val));
			return (out << _val);
		case CONSTANT_RELATIVE:
			if(isNegative())
				out << "-";
			out << "SP";
			if(_val < 0)
				return (out << "-" << -_val);
			return (out << "+" << _val);
		default:
			return (out << "(invalid cst)");
	}
}