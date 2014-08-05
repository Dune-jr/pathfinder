/*
 * TODO: give this class some appropriate name (maybe ConstantVariables and rename the other one as LabelledConstantVariables?)
**/

#ifndef _CONSTANT_VARIABLES_SIMPLIFIED_H
#define _CONSTANT_VARIABLES_SIMPLIFIED_H

#include <otawa/cfg/Edge.h>
#include <elm/genstruct/SLList.h>
#include <elm/util/Option.h>

using namespace elm;

class ConstantVariablesSimplified
{
private:
	Option<t::int32>* tempvars;
	Option<t::int32>* registers;
	unsigned int _max_tempvars;
	unsigned int _max_registers;

	Option<t::int32>& getCell(t::int32 var_id) const;

public:
	ConstantVariablesSimplified(unsigned int max_tempvars, unsigned int max_registers);
	inline void setTempVar(int id, Option<t::int32> val) { tempvars[id] = val; }
	inline void setRegister(int id, Option<t::int32> val) { tempvars[id] = val; }
	bool isConstant(t::int32 var_id) const;
	t::int32 getValue(t::int32 var_id) const;
	inline t::int32 operator[](t::int32 var_id) const { return getValue(var_id); }
};

#endif