#include "constant_variables_simplified.h"

ConstantVariablesSimplified::ConstantVariablesSimplified(unsigned int max_tempvars, unsigned int max_registers)
	: _max_tempvars(max_tempvars), _max_registers(max_registers)
{
	tempvars = new Option<t::int32>[max_tempvars];
	registers = new Option<t::int32>[max_registers];
	for(unsigned int i = 0; i < max_tempvars; i++)
		tempvars[i] = none;
	for(unsigned int i = 0; i < max_registers; i++)
		registers[i] = none;
}

Option<t::int32>& ConstantVariablesSimplified::getCell(t::int32 var_id) const
{
	if(var_id < 0)
		return tempvars[-var_id-1]; // tempvars id start at 1 and are negative
	return registers[var_id]; // registers ids start at 0 and are positive
}

bool ConstantVariablesSimplified::isConstant(t::int32 var_id) const
{
	return getCell(var_id).isOne();
}

t::int32 ConstantVariablesSimplified::getValue(t::int32 var_id) const
{
	return getCell(var_id).value();
}

io::Output& ConstantVariablesSimplified::print(io::Output& out) const
{
	out << "[" << endl;
	for(unsigned int i = 0; i < _max_tempvars; i++)
	{
		if(tempvars[i])
			out << "\tt" << i+1 << " = " << *(tempvars[i]) << endl;
	}
	for(unsigned int i = 0; i < _max_registers; i++)
	{
		if(registers[i])
			out << "\t?" << i << " = " << *(registers[i]) << endl;
	}
	return (out << "]");
}
