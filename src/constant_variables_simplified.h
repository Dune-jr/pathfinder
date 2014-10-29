/*
 * TODO: give this class some appropriate name (maybe ConstantVariables and rename the other one as LabelledConstantVariables?)
**/

#ifndef _CONSTANT_VARIABLES_SIMPLIFIED_H
#define _CONSTANT_VARIABLES_SIMPLIFIED_H

#include <otawa/cfg/Edge.h>
#include <elm/genstruct/SLList.h>
#include <elm/util/Option.h>
#include "constant.h"

using namespace elm;
using namespace elm::io;

class ConstantVariablesSimplified
{
private:
	Option<Constant>* tempvars;
	Option<Constant>* registers;
	unsigned int _max_tempvars;
	unsigned int _max_registers;

	Option<Constant>& getCell(t::int32 var_id) const;
	io::Output& print(io::Output& out) const;

public:
	ConstantVariablesSimplified(unsigned int max_tempvars, unsigned int max_registers);
	~ConstantVariablesSimplified();
	inline void setTempVar(int id, Option<Constant> val) { tempvars[id] = val; }
	inline void setRegister(int id, Option<Constant> val) { registers[id] = val; }
	bool isConstant(t::int32 var_id) const;
	Constant getValue(t::int32 var_id) const;
	inline Constant operator[](t::int32 var_id) const { return getValue(var_id); }
	friend io::Output& operator<<(io::Output& out, const ConstantVariablesSimplified& cvs) { return cvs.print(out); }
};

#endif