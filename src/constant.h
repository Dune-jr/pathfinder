#ifndef _CONSTANT_H
#define _CONSTANT_H

#include <elm/util/Option.h> // TODO! find weaker way to include elm::t::int32

using namespace elm;

enum constant_kind_t
{
	CONSTANT_INVALID=0,
	CONSTANT_ABSOLUTE,
	CONSTANT_RELATIVE,
};

enum
{
	SIGN_POSITIVE=false,
	SIGN_NEGATIVE=true,
};

class Constant
{
public:
	Constant();
	Constant(t::int32 val, bool relative, bool sign = SIGN_POSITIVE);
	Constant(t::int32 val, constant_kind_t _kind = CONSTANT_ABSOLUTE, bool sign = SIGN_POSITIVE);
	Constant(const Constant& c);
	inline t::int32 val() const { return _val; }
	inline constant_kind_t kind() const { return _kind; }
	inline bool sign() const { return _sign; }
	inline bool isRelative() const { return _kind == CONSTANT_RELATIVE; }
	inline bool isAbsolute() const { return _kind == CONSTANT_ABSOLUTE; }
	inline bool isValid()    const { return _kind != CONSTANT_INVALID;  }

	inline operator bool() { return isValid(); }
	Constant& operator=(const Constant& c);
	Constant& operator=(t::int32 val);
	bool operator==(const Constant& c) const;
	inline bool operator!=(const Constant& c) const { return !(*this == c); }
	Constant operator+(const Constant& c) const;
	Constant operator-(const Constant& c) const;
	Constant operator-() const;
	Constant operator*(const Constant& c) const;
	Constant operator/(const Constant& c) const;
	Constant operator%(const Constant& c) const;
	Constant& operator+=(const Constant& c);
	Constant& operator-=(const Constant& c);
	inline Constant operator+(int val) const { return *this + Constant(val); }
	inline Constant operator-(int val) const { return *this - Constant(val); }
	inline Constant operator*(int val) const { return *this * Constant(val); }
	inline Constant operator/(int val) const { return *this / Constant(val); }
	inline Constant operator%(int val) const { return *this % Constant(val); }
	inline Constant& operator+=(int val) { return (*this += Constant(val)); }
	inline Constant& operator-=(int val) { return (*this -= Constant(val)); }

	friend io::Output& operator<<(io::Output& out, const Constant& c) { return c.print(out); }

private:
	t::int32 _val;
	constant_kind_t _kind;
	bool _sign;

	io::Output& print(io::Output& out) const;
};

static const Constant SP(0, CONSTANT_RELATIVE); // SP + 0

#endif