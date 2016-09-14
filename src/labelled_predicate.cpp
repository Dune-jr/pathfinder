#include "labelled_predicate.h"
#include "pretty_printing.h"

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
 * @brief Update a Predicate by replacing all occurrences of an operand by another one
 * @param opd Operand to replace
 * @param expr Operand to replace with
 */

io::Output& LabelledPredicate::print(io::Output &out) const
{
	if(_labels.isEmpty())
		return (out << _pred);
	else
		return out << "(" << _pred << " | " << pathToString(_labels) << ")";
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