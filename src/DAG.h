#ifndef DAG_H
#define DAG_H

#include <elm/util/array.h>
#include <elm/types.h>
#include <elm/genstruct/HashTable.h>
#include <elm/genstruct/quicksort.h>
#include <elm/io/Output.h>
#include <otawa/program.h>
#include "predicate.h"

// using namespace elm;
using namespace otawa;

// Comment / un-comment this to enable sum canonicalization (sometimes maybe time expensive)
// #define	DAG_SUM

class DAG {

	class Key {
	public:
		inline Key(char operation = -1, const Operand *argument1 = 0, const Operand *argument2 = 0)
			: op(operation), arg1(argument1), arg2(argument2) { }
		inline char operation(void) const { return op; }
		inline const Operand *argument1(void) const { return arg1; }
		inline const Operand *argument2(void) const { return arg2; }

 		inline bool operator==(const Key& k) const { return op == k.op && arg1 == k.arg1 && arg2 == k.arg2; }
 		inline bool operator!=(const Key& k) const { return !operator==(k); }
 		inline t::hash hash(void) const { return Hasher() << op << arg1 << arg2; }
	private:
		char op; // save space
		const Operand *arg1, *arg2;
	};

	class Pred {
	public:
		Pred(void): op(CONDOPR_NE), arg1(0), arg2(0) { }
		Pred(condoperator_t operation, const Operand *argument1, const Operand *argument2)
			: op(operation), arg1(argument1), arg2(argument2) { }
		inline condoperator_t operation(void) const { return condoperator_t(op); }
		inline const Operand *argument1(void) const { return arg1; }
		inline const Operand *argument2(void) const { return arg2; }

		inline bool operator==(const Pred& k) const { return op == k.op && arg1 == k.arg1 && arg2 == k.arg2; }
		inline bool operator!=(const Pred& k) const { return !operator==(k); }
		inline t::hash hash(void) const { return Hasher() << op << arg1 << arg2; }

	private:
		char op; // save space
		const Operand *arg1, *arg2;
	};

	enum
	{
		ARITHOPR_MEM = ARITHOPR_CMP + 1, // Key.argument1 = OperandConst*
		ARITHOPR_TOP = ARITHOPR_MEM + 1, // Key.argument2 = OperandTop*
	};

	class Compare {
	public:
		static int compare(const Pair<bool, const Operand *>& o1, const Pair<bool, const Operand *>& o2) {
			if(o1.fst < o2.fst) // j
				return -1;
			else if(o1.fst > o2.fst)
				return +1;
			// same sign
			else if(*o1.snd == *o2.snd)
				return 0;
			else if(*o1.snd < *o2.snd)
				return -1;
			else // *o1.snd > *o2.snd
				return +1;
		}
	};
 
 	typedef genstruct::HashTable<Key,      Operand *,   SelfHashKey<Key> >  op_map_t;
 	typedef genstruct::HashTable<Pred,     Predicate *, SelfHashKey<Pred> > pred_map_t;
 	typedef genstruct::HashTable<Constant, Operand *,   ConstantHash>       cst_map_t;

#	ifdef DAG_SUM
	class Sum: public OperandVisitor {
	public:
		Sum(DAG& dag): _dag(dag), _neg(false) { }
		virtual bool visit(const OperandConst& o) 	{ args.add(pair(_neg, static_cast<const Operand *>(&o))); return true; }
		virtual bool visit(const OperandVar& o) 	{ args.add(pair(_neg, static_cast<const Operand *>(&o))); return true; }
		virtual bool visit(const OperandMem& o)		{ args.add(pair(_neg, static_cast<const Operand *>(&o))); return true; }
		virtual bool visit(const OperandTop& o)		{ args.add(pair(_neg, static_cast<const Operand *>(&o))); return true; }

		virtual bool visit(const OperandArith& o) {
			bool save = _neg;
			if(o.opr() == ARITHOPR_ADD) {
				o.leftOperand().accept(*this);
				_neg = save;
				o.rightOperand().accept(*this);
				_neg = save;
			}
			else if(o.opr() == ARITHOPR_SUB) {
				o.leftOperand().accept(*this);
				_neg = !save;
				o.rightOperand().accept(*this);
				_neg = save;
			}
			else if(o.opr() == ARITHOPR_NEG) {
				_neg = !_neg;
				o.leftOperand().accept(*this);
				_neg = save;
			}
			else
				args.add(pair(_neg, static_cast<const Operand *>(&o)));
			return true;
		}

		const Operand *make(arithoperator_t op, const Operand *arg1, const Operand *arg2 = NULL) {
			// only for ADD/SUB/NEG
			// record and sort the expressions of the sum
			switch(op) {
			case ARITHOPR_NEG:
				_neg = true;
				arg1->accept(*this);
				break;
			case ARITHOPR_ADD:
				_neg = false;
				arg1->accept(*this);
				arg2->accept(*this);
				break;
			case ARITHOPR_SUB:
				_neg = false;
				arg1->accept(*this);
				_neg = true;
				arg2->accept(*this);
				break;
			default:
				ASSERT(false);
				break;
			}
			genstruct::quicksort<Pair<bool, const Operand *>, genstruct::Vector, Compare>(args);

			// rebuild the expression
			int i = args.count() - 2;
			bool neg = args[i + 1].fst;
			const Operand *res = args[i + 1].snd;
			while(i >= 0) {
				/*if(args[i].fst == neg)
					res = _dag.add(args[i].snd, res);
				else
					res = _dag.sub(args[i].snd, res);*/ // infinite loop, this calls make
				res = _dag.op(args[i].fst == neg ? ARITHOPR_ADD : ARITHOPR_SUB, args[i].snd, res);
				neg = args[i].fst;
				i--;
			}
			if(neg)
				res = _dag.op(ARITHOPR_NEG, res);
			args.clear();
			return res;
		}

	private:
		DAG& _dag;
		bool _neg;
		genstruct::Vector<Pair<bool, const Operand *> > args;
	}; // Sum class
#	endif

public:
	DAG(int max_tempvars, int max_registers) : tmp_cnt(max_tempvars), var_cnt(max_registers)
#		ifdef DAG_SUM
			, sum(*this)
#		endif
	{
		vars = new Operand *[tmp_cnt + var_cnt];
		array::set(vars, tmp_cnt + var_cnt, null<Operand>());
	}

	~DAG(void) {
		for(int i = 0; i < tmp_cnt + var_cnt; i++)
			delete vars[i];
		delete [] vars;
		for(cst_map_t::Iterator cst(cst_map); cst; cst++)
			delete *cst;
		for(Vector<Operand*>::Iter atop(tops); atop; atop++)
			delete *atop;
		for(op_map_t::Iterator op(op_map); op; op++)
			delete *op;
		for(pred_map_t::Iterator pred(pred_map); pred; pred++)
			delete *pred;
	}

	const Operand *var(int v) {
		ASSERT(0 <= v + tmp_cnt && v + tmp_cnt < tmp_cnt + var_cnt);
		if(!vars[v + tmp_cnt])
			vars[v + tmp_cnt] = new OperandVar(v);
		return vars[v + tmp_cnt];
	}
	inline const Operand *var(const OperandVar& opdv)
		{ return var(opdv.addr()); }

	const Operand *cst(const Constant& cst) {
		Operand *r = cst_map.get(cst, 0);
		if(!r) {
			r = new OperandConst(cst);
			cst_map.put(cst, r);
		}
		return r;
	}

	const Operand *mem(const OperandConst *addr) {
ASSERT(addr);
		Key k(ARITHOPR_MEM, addr);
		Operand *r = op_map.get(k, 0);
		if(!r) {
			r = new OperandMem(*addr);
			op_map.put(k, r);
		}
		return r;
	}
	inline const Operand *mem(const Constant& cst)
		{ return mem(static_cast<const OperandConst*>(this->cst(cst))); }

	inline const Operand *mem(const OperandMem& opd_mem)
		{ return mem(opd_mem.addr().value()); }

	// creates a new top
	const Operand* new_top(void) {
		Operand* r = new OperandTop(tops.length());
		tops.push(r);
		return r;
	}
	/*const Operand* new_top(void) {
		Operand* r = new OperandTop();
		Key k(ARITHOPR_TOP, r);
		op_map.put(k, r); // it can't possibly already exist in the tab
		return r;
	}*/

private:
	const Operand *op(arithoperator_t op, const Operand *arg) {
		Key k(op, arg);
		Operand *r = op_map.get(k, 0);
		if(!r) {
			r = new OperandArith(op, arg);
			op_map.put(k, r);
		}
		return r;
	}

	const Operand *op(arithoperator_t op, const Operand *arg1, const Operand *arg2) {
ASSERTP(arg1 && arg2, arg1 << arg2);
		Key k(op, arg1, arg2);
		Operand *r = op_map.get(
			k, 0);
		if(!r) {
			// DBG(color::IBlu() << "k=" << *k.argument1() << (arithoperator_t)k.operation() << *k.argument2() << " not in " << *this)
			r = new OperandArith(op, arg1, arg2);
			op_map.put(k, r);
		}
		return r;
	}

	// unused
	const Operand *assocOp(arithoperator_t _op, const Operand *arg1, const Operand *arg2) {
		return op(_op, arg1, arg2);
	}

	const Operand *comOp(arithoperator_t _op, const Operand *arg1, const Operand *arg2) {
		if(*arg2 < *arg1)
			return op(_op, arg2, arg1);
		else
			return op(_op, arg1, arg2);
	}

public:
	// inline const Operand *neg (const Operand *arg) { return op(ARITHOPR_NEG, arg); }
#	ifdef DAG_SUM
		inline const Operand *neg (const Operand *arg) 						 { return sum.make(ARITHOPR_NEG, arg); }
		inline const Operand *add (const Operand *arg1, const Operand *arg2) { return sum.make(ARITHOPR_ADD, arg1, arg2); }
		inline const Operand *sub (const Operand *arg1, const Operand *arg2) { return sum.make(ARITHOPR_SUB, arg1, arg2); }
#	else
		inline const Operand *neg (const Operand *arg) 						 { return op(ARITHOPR_NEG, arg); }
		inline const Operand *add (const Operand *arg1, const Operand *arg2) { return comOp(ARITHOPR_ADD, arg1, arg2); }
		inline const Operand *sub (const Operand *arg1, const Operand *arg2) { return op(ARITHOPR_SUB, arg1, arg2); }
#	endif
	inline const Operand *mul (const Operand *arg1, const Operand *arg2) { return comOp(ARITHOPR_MUL, arg1, arg2); }
	inline const Operand *mulh(const Operand *arg1, const Operand *arg2) { return comOp(ARITHOPR_MULH, arg1, arg2); }
	inline const Operand *div (const Operand *arg1, const Operand *arg2) { return op(ARITHOPR_DIV, arg1, arg2); }
	inline const Operand *mod (const Operand *arg1, const Operand *arg2) { return op(ARITHOPR_MOD, arg1, arg2); }
	inline const Operand *cmp (const Operand *arg1, const Operand *arg2) { return op(ARITHOPR_CMP, arg1, arg2); }

	const Operand *autoOp(arithoperator_t op, const Operand *arg1, const Operand *arg2) {
		// ASSERTP(arg1 && arg2, arg1 << arg2);
		switch(op) {
			case ARITHOPR_ADD: return add(arg1, arg2);
			case ARITHOPR_SUB: return sub(arg1, arg2);
			case ARITHOPR_MUL: return mul(arg1, arg2);
			case ARITHOPR_MULH: return mulh(arg1, arg2);
			case ARITHOPR_DIV: return div(arg1, arg2);
			case ARITHOPR_MOD: return mod(arg1, arg2);
			case ARITHOPR_CMP: return cmp(arg1, arg2);
			case ARITHOPR_NEG: return neg(arg1);
			default: elm::crash();
		}
	}

	const Operand *autoOp(arithoperator_t op, const Operand *arg1) {
		switch(op) {
			case ARITHOPR_NEG: return neg(arg1);
			default: elm::crash();
		}
	}

	inline Predicate *pred(condoperator_t _op, const Operand *left, const Operand *right) {
		Pred k(_op, left, right);
		Predicate *p = pred_map.get(k, 0);
		if(!p) {
			p = new Predicate(_op, left, right);
			pred_map.put(k, p);
		}
		return p;
	}

	inline Predicate *comPred(condoperator_t _op, const Operand *left, const Operand *right) {
		if(left > right)
			return pred(_op, right, left);
		else
			return pred(_op, left, right);
	}

	inline Predicate *eq(const Operand *left, const Operand *right) { return comPred(CONDOPR_EQ, left, right); }
	inline Predicate *ne(const Operand *left, const Operand *right) { return comPred(CONDOPR_NE, left, right); }
	inline Predicate *lt(const Operand *left, const Operand *right) { return pred(CONDOPR_LT, left, right); }
	inline Predicate *le(const Operand *left, const Operand *right) { return pred(CONDOPR_LE, left, right); }
	inline Predicate *gt(const Operand *left, const Operand *right) { return pred(CONDOPR_LT, right, left); }
	inline Predicate *ge(const Operand *left, const Operand *right) { return pred(CONDOPR_LE, right, left); }

	friend io::Output& operator<<(io::Output& out, const DAG& dag) { return dag.print(out); }
private:
	io::Output& print(io::Output& out) const {
		bool first = true;
		out << "DAG\n\t-> cst:  ";
		for(cst_map_t::PairIterator i(cst_map); i; i++) {
			out << (first?"":",  ") << (*i).fst;
			first = false;
			// for(int i = 0; i < int(sizeof(Constant)); i++)
			// 	out << io::hex(((unsigned char *)&c)[i]).pad('0').right().width(2) << " ";
		}
		first = true;
		out << "\n\t-> op:   ";
		for(op_map_t::PairIterator i(op_map); i; i++) {
			out << (first?"":",  ") << *(*i).snd;
			first = false;
			// out << "\t[op]  " << *(*i).fst.argument1() << (arithoperator_t)(*i).fst.operation() << ((*i).fst.argument2()?*(*i).fst.argument2():*(*i).fst.argument1()) << ", " << *(*i).snd << endl;
		}
		first = true;
		out << "\n\t-> pred: ";
		for(pred_map_t::PairIterator i(pred_map); i; i++) {
			out << (first?"":",  ") << *(*i).snd;
			first = false;
		}
		return out;
	}

	int tmp_cnt, var_cnt;
	Operand **vars;
	Vector<Operand *> tops;
 	cst_map_t 	cst_map;
 	op_map_t 	op_map;
 	pred_map_t 	pred_map;
#	ifdef DAG_SUM
		Sum sum;
#	endif
}; // DAG class

#endif // DAG_H