#include "analysis.h"
#include <otawa/cfg/Edge.h>

void addIndents(io::Output& out, int n) { for(int i=0; i<n; i++) out << "\t"; }

Analysis::Analysis(CFG *cfg, int sp_id, unsigned int max_tempvars, unsigned int max_registers)
	: sp(sp_id), constants(max_tempvars, max_registers)
{
	DBG("Stack pointer identified to " << sp)
	initializeAnalysis();
	processCFG(cfg);
}

// LabelledPredicate
Analysis::LabelledPredicate::LabelledPredicate(const Predicate& pred, SLList<const Edge*> labels)
	: _pred(pred), _labels(labels) { }
	
io::Output& Analysis::LabelledPredicate::print(io::Output &out) const
{
	out << "(" << _pred << " | ";
	bool first_time = true;
	for(SLList<const Edge*>::Iterator iter(_labels); iter; iter++)
	{
		if(first_time)
			first_time = false;
		else
			out << ", ";
		out << (*iter)->source()->number() << "->" << (*iter)->target()->number();
	}
	return (out << ")");
}

// ConstantVariables
Analysis::ConstantVariables::ConstantVariables(unsigned int max_tempvars, unsigned int max_registers)
	: _max_tempvars(max_tempvars), _max_registers(max_registers)
{
	tempvars = new Option<t::int32>[max_tempvars];
	registers = new Option<t::int32>[max_registers];
}

Analysis::ConstantVariables::ConstantVariables(const ConstantVariables& cv)
	: _max_tempvars(cv.maxTempVars()), _max_registers(cv.maxRegisters())
{
	tempvars = new Option<t::int32>[cv.maxTempVars()];
	registers = new Option<t::int32>[cv.maxRegisters()];
	*this = cv;
}

Analysis::ConstantVariables& Analysis::ConstantVariables::operator=(const ConstantVariables& cv)
{
	// the two ConstantVariables must have the same size!
	assert(_max_tempvars == cv.maxTempVars());
	assert(_max_registers == cv.maxRegisters());
	for(int i = 0; i < _max_tempvars; i++)
		tempvars[i] = cv.tempvars[i];
	for(int i = 0; i < _max_registers; i++)
		registers[i] = cv.registers[i];
	return *this;
}

Option<t::int32>& Analysis::ConstantVariables::getCell(const OperandVar& opdv) const
{	
	if(opdv.isTempVar())
		return tempvars[-1-opdv.addr()]; // tempvars id start at 1 and are negative
	return registers[opdv.addr()]; // registers ids start at 0 and are positive
}

bool Analysis::ConstantVariables::isConstant(const OperandVar& opdv) const
{
	return getCell(opdv).isOne();
}

t::int32 Analysis::ConstantVariables::getValue(const OperandVar& opdv) const
{
	Option<t::int32>& k = getCell(opdv);
	assert(k.isOne());
	return k.value();
}

void Analysis::ConstantVariables::set(const OperandVar& opdv, t::int32 val)
{
	Option<t::int32>& k = getCell(opdv);
	k = some(val);
	DBG(COLOR_IGre << *this << COLOR_BIGre " +" << opdv << "=" << val)
}

void Analysis::ConstantVariables::invalidate(const OperandVar& opdv)
{
	Option<t::int32>& k = getCell(opdv);
	k = none;
	DBG(COLOR_IRed << *this << COLOR_BIRed " -" << opdv)
}

void Analysis::ConstantVariables::invalidateTempVars()
{
	for(int i = 0; i < _max_tempvars; i++)
		tempvars[i] = none;
	DBG(COLOR_IRed << *this << COLOR_BIRed " -tempvars")
}

io::Output& Analysis::ConstantVariables::print(io::Output& out) const
{
	out << "[" << endl;
	for(int i = 0; i < _max_tempvars; i++)
	{
		if(!tempvars[i])
			continue; // do not print variables we know nothing about
		out << "\tt" << i+1 << " = " << *(tempvars[i]) << endl;
	}
	for(int i = 0; i < _max_registers; i++)
	{
		if(!registers[i])
			continue;
		out << "\t?" << i << " = " << *(registers[i]) << endl;
	}
	out << "]";
}

// topList shortcuts
void Analysis::addElemToTopList(const SLList<LabelledPredicate>& lps)
{	
	SLList<LabelledPredicate> topList = getTopList(); // get topList
	topList.addAll(lps);							  // append to topList
	setTopList(topList); 							  // set topList as new first item of attribute labelled_preds
}

SLList<Analysis::LabelledPredicate> Analysis::getTopList()
{
	return labelled_preds.first();
}

void Analysis::setTopList(const SLList<LabelledPredicate>& lps)
{
	labelled_preds.removeFirst();
	labelled_preds.addFirst(lps);
}
