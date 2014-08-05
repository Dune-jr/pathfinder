#ifndef _LABELLED_PREDICATE_H
#define _LABELLED_PREDICATE_H

#include <otawa/cfg/Edge.h>
#include <elm/genstruct/SLList.h>
#include "predicate.h"

using namespace otawa;
using namespace elm::genstruct; 

// LabelledPredicate class
class LabelledPredicate {
private:
	Predicate _pred;
	SLList<const Edge*> _labels;
	io::Output& print(io::Output& out) const;

public:
	LabelledPredicate(const Predicate& pred, const SLList<const Edge*>& labels);
	LabelledPredicate(const LabelledPredicate& lp);
	inline const Predicate& pred() const { return _pred; };
	inline const SLList<const Edge*>& labels() const { return _labels; };
	friend io::Output& operator<<(io::Output& out, const LabelledPredicate& lv) { return lv.print(out); }
};

#endif
