#include "analysis.h"

Analysis::Analysis(CFG *cfg)
{
	initializeAnalysis();
	processCFG(cfg);
}

Analysis::LabelledPredicate::LabelledPredicate(Predicate pred, Edge* label)
	: _pred(pred), _label(label) { }
	
io::Output& Analysis::LabelledPredicate::print(io::Output &out) const
{
	out << "(" << _pred << " | " << _label << ")";
	return out;
}
