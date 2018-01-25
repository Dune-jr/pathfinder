/*
 *	
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2006-2018, IRIT UPS.
 * 
 *	OTAWA is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	OTAWA is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OTAWA; if not, write to the Free Software 
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
#ifndef _CONSTANT_H
#define _CONSTANT_H

#include <elm/hash.h>
#include <elm/int.h> // elm::t::int32
#include <elm/io.h> // elm::io

using elm::_;
using elm::t::int32;
using elm::io::Output;

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
	Constant(int32 val, constant_kind_t _kind = CONSTANT_ABSOLUTE); //, bool sign = SIGN_POSITIVE);
	Constant(const Constant& c);
	inline int32 val() const { return _val; }
	// inline constant_kind_t kind() const { return constant_kind_t(_kind); }
	inline bool sign() const { ASSERT(isRelative()); return _kind == CONSTANT_PLUS_SP ? SIGN_POSITIVE : SIGN_NEGATIVE; }
	inline bool isRelative() const { return _kind > CONSTANT_ABSOLUTE; }
	inline bool isAbsolute() const { return _kind == CONSTANT_ABSOLUTE; }
	inline bool isValid()    const { return _kind != CONSTANT_INVALID;  }
	inline bool isValidAddress() const { return isAbsolute() || isRelativePositive(); }
	inline bool isRelativePositive() const { return _kind == CONSTANT_PLUS_SP; }
	inline bool isRelativeNegative() const { return _kind == CONSTANT_MINUS_SP; }
	inline elm::t::hash hash() const { return elm::Hasher() << _kind << _val; }

	Constant& operator=(const Constant& c);
	Constant& operator=(int32 val);
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
	friend Output& operator<<(Output& out, const Constant& c) { return c.print(out); }

private:
	int32 _val;
	char _kind; // saving space with 1 bye. will be casted to constant_kind_t
	// bool _sign;

	Output& print(Output& out) const;
};

extern const Constant SP; // SP + 0

class ConstantHash {
public:
	static inline elm::t::hash hash(const Constant& c) { return c.hash(); }
	static inline bool equals(const Constant& c1, const Constant& c2) { return c1 == c2; }
};

#endif
