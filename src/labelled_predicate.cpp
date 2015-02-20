#include "labelled_predicate.h"

LabelledPredicate::LabelledPredicate(const Predicate& pred, const Set<Edge*>& labels)
	: _pred(pred), _labels(labels) { }

LabelledPredicate::LabelledPredicate(const LabelledPredicate& lp)
	: _pred(lp._pred), _labels(lp._labels) { }
	
io::Output& LabelledPredicate::print(io::Output &out) const
{
	if(_labels.isEmpty())
		return (out << _pred);

	out << "(" << _pred << " | ";
	bool first_time = true;
	for(Set<Edge*>::Iterator iter(_labels); iter; iter++)
	{
		if(first_time)
			first_time = false;
		else
			out << ", ";
		out << (*iter)->source()->number() << "->" << (*iter)->target()->number();
	}
	return (out << ")");
}

LabelledPredicate& LabelledPredicate::operator=(const LabelledPredicate& lp)
{
	_pred = lp._pred;
	_labels = lp._labels; // should do proper copy
	return *this;
}

bool LabelledPredicate::operator==(const LabelledPredicate& lp) const
{
	return _pred == lp._pred && _labels == lp._labels; // strict equality: also testing labels
}