/*
 * DAG.cpp
 *
 *  Created on: 15 avr. 2016
 *      Author: casse
 */

#ifndef DAG_CPP_
#define DAG_CPP_

#include <elm/util/array.h>
#include <elm/types.h>
#include <elm/genstruct/HashTable.h>
#include <elm/genstruct/quicksort.h>
#include <elm/io/Output.h>
#include <otawa/program.h>
#include "predicate.h"
#include "debug.h"

using namespace elm;
using namespace elm::color;
using namespace otawa;

// Comment / un-comment this to enable sum canonicalization (sometimes maybe time expensive)
#define	DAG_SUM

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

	const static int
		ARITHOPR_MEM = ARITHOPR_CMP + 1;

	class Compare {
	public:
		static int compare(const Pair<bool, const Operand *>& o1, const Pair<bool, const Operand *>& o2) {
			if(o1.fst < o2.fst) // j
				return -1;
			else if(o1.fst > o2.fst)
				return +1;
			// same sign
			else if(o1.snd < o2.snd)
				return -1;
			else if(o1.snd == o2.snd)
				return 0;
			else
				return +1;
		}
	};

 	class ConstantHash {
 	public:
 		static inline t::hash hash(const Constant& c) { return c.hash(); }
 		static inline bool equals(const Constant& c1, const Constant& c2) { return c1 == c2; }
 	};
 
 	typedef genstruct::HashTable<Key,      Operand *,   SelfHashKey<Key> >  op_map_t;
 	typedef genstruct::HashTable<Pred,     Predicate *, SelfHashKey<Pred> > pred_map_t;
 	typedef genstruct::HashTable<Constant, Operand *,   ConstantHash>       cst_map_t;

#	ifdef DAG_SUM
	class Sum: public OperandVisitor {
	public:
		Sum(DAG& dag): _dag(dag), _neg(false) { }
		virtual bool visit(const OperandConst& o) 		{ args.add(pair(_neg, static_cast<const Operand *>(&o))); return true; }
		virtual bool visit(const OperandVar& o)			{ args.add(pair(_neg, static_cast<const Operand *>(&o))); return true; }
		virtual bool visit(const OperandMem& o)			{ args.add(pair(_neg, static_cast<const Operand *>(&o))); return true; }

		virtual bool visit(const OperandArithExpr& o) {
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
			if(vars[i])
				delete vars[i];
		delete [] vars;
		for(cst_map_t::Iterator cst(cst_map); cst; cst++)
			delete *cst;
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

	const Operand *cst(const Constant& cst) {
		Operand *r = cst_map.get(cst, 0);
		if(!r) {
			r = new OperandConst(cst);
			cst_map.put(cst, r);
		}
		return r;
	}

    const Operand *mem(const OperandConst *addr) {
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

	const Operand *mem(const OperandMem *addr) {
		Key k(ARITHOPR_MEM, addr);
		Operand *r = op_map.get(k, 0);
		if(!r) {
			r = new OperandMem(*addr);
			op_map.put(k, r);
		}
		return r;
	}

private:
	const Operand *op(arithoperator_t op, const Operand *arg) {
		Key k(op, arg);
		Operand *r = op_map.get(k, 0);
		if(!r) {
			r = new OperandArithExpr(op, *arg);
			op_map.put(k, r);
		}
		return r;
	}

	const Operand *op(arithoperator_t op, const Operand *arg1, const Operand *arg2) {
		Key k(op, arg1, arg2);
		Operand *r = op_map.get(k, 0);
		if(!r) {
			// DBG(color::IBlu() << "k=" << *k.argument1() << (arithoperator_t)k.operation() << *k.argument2() << " not in " << *this)
			r = new OperandArithExpr(op, *arg1, *arg2);
			op_map.put(k, r);
		}
		return r;
	}

	// unused
	const Operand *assocOp(arithoperator_t _op, const Operand *arg1, const Operand *arg2) {
		return op(_op, arg1, arg2);
	}

	const Operand *comOp(arithoperator_t _op, const Operand *arg1, const Operand *arg2) {
		if(arg1 > arg2)
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

	inline Predicate *pred(condoperator_t _op, const Operand *left, const Operand *right) {
		Pred k(_op, left, right);
		Predicate *p = pred_map.get(k, 0);
		if(!p) {
			p = new Predicate(_op, *left, *right);
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
		out << "DAG\n\t-> cst:  ";
		bool first = true;
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
 	cst_map_t 	cst_map;
 	op_map_t 	op_map;
 	pred_map_t 	pred_map;
#	ifdef DAG_SUM
		Sum sum;
#	endif
}; // DAG class

void testSimplify(DAG& d)
{
	const Operand* zero  = d.cst(0);
	const Operand* one   = d.cst(1);
	const Operand* two   = d.cst(2);
	const Operand* three = d.cst(3);
	DBG("zero: "  << *zero)
	DBG("one: "   << *one)
	DBG("two: "   << *two)
	DBG("three: " << *three)

	const Operand* four =  d.cst(4);four=four;
	const Operand* t1 = d.var(-1);

	DBG("making neg(t1)")
	const Operand* e11 = d.neg(t1);
	DBG("neg(t1): " << *e11)
	const Operand* e1 = d.add(t1, e11);
	const Operand* f = d.sub(two, three);f=f;
	const Operand* fv2 = d.sub(two, three);fv2=fv2;

	DBG("e1: "    << *e1)
	/*Operand* o1 = e1->copy();
	if(Option<Operand*> o2 = o1->simplify()) {
		DBG("e1 simplified: " << **o2)
		delete *o2;
	}
	else DBG("e1 easy-simplified: " << *o1)*/
	DBG("f=fv2: " << DBG_TEST(f==fv2, true) << "  (" << *f << "==" << *fv2 << ")")
	// DBG("one+two: " << *d.add(one, two))
	// DBG("one+two simplified: " << **d.op(ARITHOPR_ADD, one, two)->simplify())
	// DBG("one+two simplified equal three: " << DBG_TEST(three == *d.op(ARITHOPR_ADD, one, two)->simplify(), true))
}

void testPredicates(DAG& d)
{
	const Operand* oprconst = d.cst(2);
	const Operand* oprvar = d.mem(0x4000);
	const Operand* e1 = d.mul(oprconst, oprvar); // e1 := 2 * @0x4000
	const Predicate* p1 = d.pred(CONDOPR_EQ, oprvar, e1); // p1 := @0x4000 = e1
	const Predicate* p1bis = d.eq(oprvar, e1); // p1 := @0x4000 = e1
	const Predicate* p2 = d.pred(CONDOPR_LE, oprconst, oprvar); // p2 := 2 <= @0x4000
	
	DBG("--- Equality over predicates ---")
	DBG("p1 = p1':\t"<< DBG_TEST(p1 == p1bis, true))
	DBG("p2 = p2:\t" << DBG_TEST(p2 == p2, true))
	DBG("p1 = p2:\t" << DBG_TEST(p1 == p2, false) << io::endl)
}

void testOperands(DAG& d)
{
	DBG("--- Equality over Operands  ---")
	const Operand* o1 = d.cst(12);
	const Operand* o1bis = d.cst(12);
	const Operand* o2 = d.cst(16);
	const Operand* o3 = d.mem(0x1004);
	const Operand* o4 = d.mem(0x1008);
	DBG("diag: " << d)
	DBG("o1 = o1:\t" << DBG_TEST(o1 == o1, true) 	<< " \t" << *o1 << "==" << *o1)
	DBG("o1 = o1':\t"<< DBG_TEST(o1 == o1bis, true) << " \t" << *o1 << "==" << *o1bis)
	DBG("o1 = o2:\t" << DBG_TEST(o1 == o2, false) 	<< " \t" << *o1 << "==" << *o2)
	DBG("o1 = o3:\t" << DBG_TEST(o1 == o3, false) 	<< " \t" << *o1 << "==" << *o3)
	DBG("o3 = o1:\t" << DBG_TEST(o3 == o1, false) 	<< " \t" << *o3 << "==" << *o1)
	DBG("o3 = o3:\t" << DBG_TEST(o3 == o3, true) 	<< " \t" << *o3 << "==" << *o3)
	DBG("o3 = o4:\t" << DBG_TEST(o3 == o4, false) 	<< " \t" << *o3 << "==" << *o4 << io::endl)
	
	const Operand* oae = d.mul(o1, o2);
	const Operand* oaebis = d.mul(o1, o2);
	DBG("o1*o2:\t" << *oae)
	const Operand* oae2 = d.mul(o2, o2);
	const Operand* reverse_oae = d.mul(o2, o1);
	const Operand* oae4 = d.add(o1, o2);
	DBG("oae = oae:\t"  << DBG_TEST(oae == oae, true))
	DBG("oae = oae':\t"  << DBG_TEST(oae == oaebis, true))
	DBG("oae = oae2:\t" << DBG_TEST(oae == oae2, false))
	DBG("oae = roae:\t" << DBG_TEST(oae == reverse_oae, true))
	DBG("oae = oae4:\t" << DBG_TEST(oae == oae4, false))
}

int _main(void)
{
	elm::log::Debug::setSourceInfoFlag(false);
	DAG dag(16,16);
	testOperands(dag);
	testPredicates(dag);
	testSimplify(dag);
	DBG("diag: " << dag)
	DBG("==================================\n")
	Predicate *p = dag.eq(dag.sub(dag.neg(dag.add(dag.cst(2),dag.cst(2))), dag.sub(dag.mul(dag.cst(2),dag.cst(3)), dag.add(dag.mul(dag.cst(2),dag.cst(47)),dag.mul(dag.cst(2),dag.cst(13))))),
		   dag.add(dag.cst(13),dag.cst(47)));
	DBG("diag: " << dag)
	DBG("Original predicate: -(2+2) - (2*3 - (2*47 + 2*13)) = 13+47")
	DBG("Predicate: " << *p)
	return 0;
}

#endif /* DAG_CPP_ */
