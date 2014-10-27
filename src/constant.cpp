#include "constant.h"

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
			assert(false); // TODO: why do I need this
	}
}
Constant Constant::operator+(const Constant& c) const
{
	cout << "call, *this="<<*this<<", c="<<c<<io::endl;
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
					return Constant(new_val, CONSTANT_RELATIVE, -c._sign);
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
		return *this;
	if(c == -1)
		return -*this;
	if(isAbsolute() && c.isAbsolute())
		return Constant(_val / c._val);
	return Constant();
}
Constant Constant::operator%(const Constant& c) const
{
	if(c == 0 || c.kind() != CONSTANT_ABSOLUTE)
		return Constant();
	if(*this == 0)
		return Constant(0);
	if(c == 1)
		return *this;
	if(isAbsolute())
		return Constant(_val % c._val);
	return Constant();
}
Constant& Constant::operator+=(const Constant& c)
{
	return (*this = *this + c);
}
Constant& Constant::operator-=(const Constant& c)
{
	return (*this = *this - c);
}
io::Output& Constant::print(io::Output& out) const
{
	switch(_kind)
	{
		case CONSTANT_ABSOLUTE:
			return (out << _val);
		case CONSTANT_RELATIVE:
			if(_sign==SIGN_POSITIVE)
				return (out << "sp+" << _val);
			else
				return (out << "-sp+" << _val);
		default:
			return (out << "(invalid)");
	}
}