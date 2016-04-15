#include "labelled_predicate.h"

/**
 * @class LabelledPredicate
 * @brief Predicate labelled with a list of CFG Edges
 */
LabelledPredicate::LabelledPredicate(const Predicate& pred, const Set<Edge*>& labels)
	: _pred(pred), _labels(labels) { }

LabelledPredicate::LabelledPredicate(const LabelledPredicate& lp)
	: _pred(lp._pred), _labels(lp._labels) { }

/**
 * @fn inline void LabelledPredicate::addLabel(Edge* label);
 * @brief Add a label
 * @param label label to add
 */

/**
 * @fn inline void LabelledPredicate::addLabels(const Set<Edge*>& labels);
 * @brief Add a set of labels
 * @param labels set of labels to add
 */

/**
 * @fn inline bool LabelledPredicate::updatePred(const Operand& opd, const Operand& expr);
 * @brief Update a Predicate by replacing all occurences of an operand by another one
 * @param opd Operand to replace
 * @param expr Operand to replace with
 */

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
#ifdef v1
		out << (*iter)->source()->number() << "->" << (*iter)->target()->number(); // v1
#else
		out << (*iter)->source()->index() << "->" << (*iter)->target()->index(); // v2
#endif
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