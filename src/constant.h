#ifndef _CONSTANT_H
#define _CONSTANT_H

#include <elm/util/HashKey.h>
#include <elm/int.h> // elm::t::int32
#include <elm/io.h> // elm::io

using namespace elm;

enum constant_kind_t
{
	CONSTANT_INVALID=0,
	CONSTANT_ABSOLUTE,
	// CONSTANT_RELATIVE,
	CONSTANT_PLUS_SP,
	CONSTANT_MINUS_SP,
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
	// Constant(t::int32 val, bool relative, bool sign = SIGN_POSITIVE);
	Constant(t::int32 val, constant_kind_t _kind = CONSTANT_ABSOLUTE); //, bool sign = SIGN_POSITIVE);
	Constant(const Constant& c);
	inline t::int32 val() const { return _val; }
	// inline constant_kind_t kind() const { return constant_kind_t(_kind); }
	inline bool sign() const { ASSERT(isRelative()); return _kind == CONSTANT_PLUS_SP ? SIGN_POSITIVE : SIGN_NEGATIVE; }
	inline bool isRelative() const { return _kind > CONSTANT_ABSOLUTE; }
	inline bool isAbsolute() const { return _kind == CONSTANT_ABSOLUTE; }
	inline bool isValid()    const { return _kind != CONSTANT_INVALID;  }
	inline bool isValidAddress() const { return isAbsolute() || isRelativePositive(); }
	inline bool isRelativePositive() const { return _kind == CONSTANT_PLUS_SP; }
	inline bool isRelativeNegative() const { return _kind == CONSTANT_MINUS_SP; }
	inline t::hash hash() const { return elm::Hasher() << _kind << _val; }

	Constant& operator=(const Constant& c);
	Constant& operator=(t::int32 val);
	bool operator==(const Constant& c) const;
	inline bool operator==(int val) const { return *this == Constant(val); }
	inline bool operator!=(const Constant& c) const { return !(*this == c); }
	Constant operator+(const Constant& c) const;
	Constant operator-(const Constant& c) const;
	Constant operator-() const;
	Constant operator~() const;
	Constant operator*(const Constant& c) const;
	Constant operator/(const Constant& c) const;
	Constant operator%(const Constant& c) const;
	Constant operator&(const Constant& c) const;
	Constant operator|(const Constant& c) const;
	Constant operator^(const Constant& c) const;
	Constant operator<<(const Constant& c) const;
	Constant operator>>(const Constant& c) const;
	Constant& operator+=(const Constant& c);
	Constant& operator-=(const Constant& c);
	inline Constant operator+(int val) const { return *this + Constant(val); }
	inline Constant operator-(int val) const { return *this - Constant(val); }
	inline Constant operator*(int val) const { return *this * Constant(val); }
	inline Constant operator/(int val) const { return *this / Constant(val); }
	inline Constant operator%(int val) const { return *this % Constant(val); }
	inline Constant operator<<(int val) const { return *this << Constant(val); }
	inline Constant operator>>(int val) const { return *this >> Constant(val); }
	inline Constant& operator+=(int val) { return (*this += Constant(val)); }
	inline Constant& operator-=(int val) { return (*this -= Constant(val)); }

	bool operator<(const Constant& c) const;
	bool operator>(const Constant& c) const; // for elm::Comparator in elm::genstruct::AVLMap

	inline elm::String toString() const { return _ << *this; } 
	friend io::Output& operator<<(io::Output& out, const Constant& c) { return c.print(out); }

private:
	t::int32 _val;
	char _kind; // saving space with 1 bye. will be casted to constant_kind_t
	// bool _sign;

	io::Output& print(io::Output& out) const;
};

extern const Constant SP; // SP + 0

class ConstantHash {
public:
	static inline t::hash hash(const Constant& c) { return c.hash(); }
	static inline bool equals(const Constant& c1, const Constant& c2) { return c1 == c2; }
};

#endif
