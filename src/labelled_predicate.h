#ifndef _LABELLED_PREDICATE_H
#define _LABELLED_PREDICATE_H

#include <elm/avl/Set.h>
#include <otawa/cfg/Edge.h>
#include "predicate.h"

using namespace otawa;
using namespace elm::avl;
// using namespace elm::genstruct;

// LabelledPredicate class
class LabelledPredicate {
public:
	LabelledPredicate(const Predicate& pred, const Set<Edge*>& labels);
	LabelledPredicate(const LabelledPredicate& lp);
	inline const Predicate& pred() const { return _pred; }
	inline const Set<Edge*>& labels() const { return _labels; }
	inline bool updatePred(const Operand& opd, const Operand& expr) { return _pred.update(opd, expr); }
	inline void addLabel(Edge* label) { _labels += label; }
	inline void addLabels(const Set<Edge*>& labels) { _labels.addAll(labels); }

	LabelledPredicate& operator=(const LabelledPredicate& lp);
	bool operator==(const LabelledPredicate& lp) const;
	friend io::Output& operator<<(io::Output& out, const LabelledPredicate& lv) { return lv.print(out); }
	inline const LabelledPredicate* operator->(void) const { return this; }
private:
	io::Output& print(io::Output& out) const;

	Predicate _pred;
	Set<Edge*> _labels; // Analysis::Path
};

#endif
