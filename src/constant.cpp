#include "constant.h"

using namespace elm;

Constant::Constant() : _val(0), _kind(CONSTANT_INVALID) { }
Constant::Constant(t::int32 val, bool relative) : _val(val), _kind(relative ? CONSTANT_RELATIVE : CONSTANT_ABSOLUTE) { }
Constant::Constant(t::int32 val, constant_kind_t kind) : _val(val), _kind(kind) { }
Constant::Constant(const Constant& c) : _val(c._val), _kind(c._kind) { }

Constant& Constant::operator=(const Constant& c)
{
	_val = c._val;
	_kind = c._kind;
	return *this;
}
Constant& Constant::operator=(t::int32 val)
{
	_val = val;
	return *this;
}
bool Constant::operator==(const Constant& c) const
{
	return (_kind == c._kind) && (_val == c._val);
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
					return Constant(new_val, CONSTANT_RELATIVE);
				default:
					return Constant();
			}
		case CONSTANT_RELATIVE:
			switch(c._kind)
			{
				case CONSTANT_ABSOLUTE:
					return Constant(new_val, CONSTANT_RELATIVE);
				case CONSTANT_RELATIVE:
					return Constant(); // SP + SP
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
					return Constant(); // -SP
				default:
					return Constant();
			}
		case CONSTANT_RELATIVE:
			switch(c._kind)
			{
				case CONSTANT_ABSOLUTE:
					return Constant(new_val, CONSTANT_RELATIVE);
				case CONSTANT_RELATIVE:
					return Constant(new_val, CONSTANT_ABSOLUTE); // SP - SP
				default:
					return Constant();
			}
		default:
			return Constant(); // invalid
	}
}
Constant Constant::operator*(const Constant& c) const
{
	if(c == 0 || *this == 0)
		return Constant(0);
	if(c == 1)
		return *this;
	if(*this == 1)
		return c;
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
			return (out << "sp+" << _val);
		default:
			return (out << "(invalid)");
	}
}