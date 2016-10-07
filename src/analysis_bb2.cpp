/*
 * V2 version of the BasicBlock top-down analysis
 */

#include <otawa/sem/PathIter.h>
#include "operand.h"
#include "DAG.h"
#include "debug.h"
#include "analysis_state.h"

using namespace otawa::sem;

void Analysis::State::processSemInst2(const PathIter& seminsts, sem::inst& last_condition)
{
	const t::int16 &a = seminsts.a(), &b = seminsts.b(), &d = seminsts.d();
	const t::int32 &cst = seminsts.cst();
	const t::int16 &reg = seminsts.reg(), &addr = seminsts.addr();
	switch(seminsts.op())
	{
		case NOP:
			break;
		case BRANCH:
			// TODO
			break;
		case CONT:
			// TODO
			break;
		case IF:
			// TODO
			break;
		case LOAD: // reg <- MEM_type(addr)
			// tip: addr is likely to be t1
			if(lvars.isConst(addr))
			{
				Constant c = lvars(addr).toConstant();
				if(Option<Constant> v = getConstantValueOfReadOnlyMemCell(OperandMem(c), (*seminsts).type()))
				{
					DBG(color::IBlu() << "  R-O memory cell " << OperandMem(c) << " simplified to " << *v)
					set(reg, dag->cst(*v));
				}
				else if(mem.exists(c))
				{
					DBG(color::IBlu() << "  Reading from " << OperandMem(c))
					set(reg, mem[c]);
				}
				else
				{
					DBG(color::IYel() << "  Loading Top from " << OperandMem(c))
					scratch(reg); 
				}
			}
			else
			{
				DBG(color::IYel() << "  Load address is unknown: " << OperandVar(addr) << " = " << lvars(addr))
				scratch(reg);
			}			
			break;
		
		case STORE:	// MEM_type(addr) <- reg
		{
			Constant c;
			if(lvars.isConst(addr) && (c = lvars(addr).toConstant(), c.isValidAddress()))
				setMem(c, getPtr(reg));
			else
			{
				DBG(color::IYel() << "  Store address is unknown: " << OperandVar(addr) << " = " << lvars(addr))
				scratchAllMemory(); // access to Top
			}
			break;
		}
		case SET: // d <- a
			set(d, getPtr(a));
			break;
		case SETI: // d <- cst
			set(d, dag->cst(cst));
			break;
		case SETP:
			scratch(d);
			break;
		case CMPU:
			ASSERT(!UNTESTED_CRITICAL);
			DBG(color::BIRed() << "Untested unsigned variant running!")
		case CMP:
			set(d, dag->cmp(getPtr(a), getPtr(b)));
			break;
		case ADD:
			set(d, smart_add(getPtr(a), getPtr(b)));
			break;
		case SUB:
			set(d, smart_sub(getPtr(a), getPtr(b)));
			break;
		case SHL: // d <- a << b
			if(lvars.isConst(b))
				set(d, smart_mul(getPtr(a), Constant(1) << lvars(b).toConstant()));
			else
			{
				DBG(color::IYel() << "  SHL with unknown factor!")
				scratch(d);
			}
			break;
		case ASR:
		case SHR:
			if(lvars.isConst(b))
			{
				if(lvars.isConst(a))
					set(d, dag->cst(getPtr(a)->toConstant() / Constant(1) << lvars(b).toConstant()));
				else
					set(d, dag->div(getPtr(a), dag->cst(Constant(1) << lvars(b).toConstant())));
			}
			else
			{
				DBG(color::IYel() << "  SHL with unknown factor!")
				scratch(d);
			}
			break;
		case NEG:
			break;
		case NOT: // d <- ~a
			break;
		case OR:  // d <- a | b
		case XOR: // d <- a ^ b
		{
			Option<Constant> av, bv;
			if(lvars.isConst(a) && lvars.isConst(b)
				&& (av = lvars(a).toConst().evalConstantOperand())
				&& (bv = lvars(b).toConst().evalConstantOperand()) )
				set(d, dag->cst(seminsts.op() == OR ? (*av | *bv) : (*av ^ *bv)));
			else
				scratch(d);
			break;
		}
		case AND: // d <- a & b
		{
			Option<Constant> av, bv;
			if(lvars.isConst(a) && lvars.isConst(b)
				&& (av = lvars(a).toConst().evalConstantOperand())
				&& (bv = lvars(b).toConst().evalConstantOperand()) )
				set(d, dag->cst(*av & *bv));
			else if(lvars.isConst(a) || lvars.isConst(b)) // test MODULO 2^x
			{
				const t::int32 cstv = lvars(lvars.isConst(a) ? a : b).toConstant().val();
				const Operand* o = lvars.isConst(a) ? getPtr(b) : getPtr(a);
				if(cstv < 0)
				{
					DBG(color::IYel() << "  [Right operand of AND affects MSB]") // Most Significant Bit
					scratch(d);
				}
				else
				{
					int x = cstv, i = 0;
					while(x&1)
					{
						x >>= 1;
						i++;
					}
					if(x == 0) // cstv was 00...000111..111, and i is the count of 1s
					{
						int mod_factor = cstv+1; // if cstv is 111, mod factor is 8 = 1000 f.e.
						DBG(color::Blu() << "  [Detected a modulus " << mod_factor << " operation]")
						set(d, dag->mod(o, dag->cst(mod_factor)));
					}
					else
						scratch(d);
				}
			}
			else
				scratch(d);
			break;
		}
		case MULU:
			ASSERT(!UNTESTED_CRITICAL);
			DBG(color::BIRed() << "Untested unsigned variant running!")
		case MUL:
			set(d, smart_mul(getPtr(a), getPtr(b)));
			break;
		case MULH:
			scratch(d); // TODO! do better
			break;
		case DIVU:
			DBG(color::BIRed() << "Untested unsigned variant running!")
			ASSERT(!UNTESTED_CRITICAL);
		case DIV:
			if(Option<Constant> av = getPtr(a)->evalConstantOperand())
				if(Option<Constant> bv = getPtr(b)->evalConstantOperand())
				{
					set(d, dag->cst(*av / *bv));
					break;
				}
			set(d, dag->div(getPtr(a), getPtr(b)));
			break;
		case MODU:
		case MOD:
			if(Option<Constant> av = getPtr(a)->evalConstantOperand())
				if(Option<Constant> bv = getPtr(b)->evalConstantOperand())
				{
					set(d, dag->cst(*av % *bv));
					break;
				}
			set(d, dag->mod(getPtr(a), getPtr(b)));
			break;
		case SPEC: // special instruction (d: code, cst: sub-code)
		default:
			DBG(color::BIRed() << "Unknown seminst running!")
			ASSERT(!UNTESTED_CRITICAL);
		case SCRATCH:
			scratch(d);
			break;
	}
}

void Analysis::State::set(const OperandVar& x, const Operand* expr)
{
	if(dbg_verbose == DBG_VERBOSE_ALL)
	{
		elm::String output = _ << color::Cya() << " {" << lvars(x) << "}";
		lvars[x] = expr;
		DBG(color::ICya() << " * " << x << " = " << lvars(x) << output)
	}
	else
		lvars[x] = expr;
}

void Analysis::State::setMem(Constant addr, const Operand* expr)
{
	DBG(color::ICya() << " * " << OperandMem(addr) << " = " << *expr << (mem.exists(addr) ? _ << color::Cya() << " {" << *mem[addr] << "}" : elm::String()))
	mem[addr] = expr;
}

const Operand* Analysis::State::getPtr(t::int32 var_id) const
{
	return lvars[var_id] ? lvars[var_id] : dag->var(var_id);
}

void Analysis::State::scratchAllMemory()
{
	DBG(color::IRed() << "  Access to Top, invalidating all memory (" << mem.count() << " items)")
	for(mem_t::MutableIter i(mem); i; i++)
		i.item() = dag->new_top(); // TODO! that looks costly... maybe we should rework the way Tops work in DAG.
}

// this is minimal and a bit unoptimized
const Operand* Analysis::State::smart_add(const Operand* a, const Operand* b)
{
	Option<Constant> av = a->evalConstantOperand(), bv = b->evalConstantOperand();
	if(av && bv)
		return dag->cst(*av+*bv);
	else if(!av && !bv)
		return dag->add(a, b);
	// only one is constant
	Constant x = av ? *av : *bv;
	const Operand* y = av ? b : a;
	if(x == 0)
		return y;
	if(y->kind() == ARITH)
	{
		const OperandArith& z = y->toArith();
		av = z.leftOperand().evalConstantOperand();
		bv = z.rightOperand().evalConstantOperand();
		if(z.opr() == ARITHOPR_ADD)
		{
			if(av) // x + (av + z2)
				return dag->add(z.right(), dag->cst(x + *av));
			if(bv) // x + (z1 + bv)
				return dag->add(z.left(), dag->cst(x + *bv));
		}
		if(z.opr() == ARITHOPR_SUB)
		{
			if(av) // x + (av - z2)
				return dag->sub(dag->cst(x + *av), z.right());
			if(bv) // x + (z1 - bv)
				return dag->add(z.left(), dag->cst(x - *bv));
		}
	}
	return dag->add(a, b);
}

// TODO! implement this further (just like smart_add)
const Operand* Analysis::State::smart_sub(const Operand* a, const Operand* b)
{
	Option<Constant> av = a->evalConstantOperand(), bv = b->evalConstantOperand();
	if(av && bv)
		return dag->cst(*av-*bv);
	else if(!av && !bv)
		return dag->sub(a, b);
	else if(av && *av == 0)
		return b;
	else if(bv && *bv == 0)
		return a;
	else
		return dag->sub(a, b);
}

const Operand* Analysis::State::smart_mul(const Operand* a, const Operand* b)
{
	if(Option<Constant> av = a->evalConstantOperand())
		return smart_mul(b, *av);
	else if(Option<Constant> bv = b->evalConstantOperand())
		return smart_mul(a, *bv);
	else
		return dag->mul(a, b);
}

const Operand* Analysis::State::smart_mul(const Operand* a, Constant c)
{
	if(c == 1)
		return a;
	if(c == 0)
		return dag->cst(0);
	Option<Constant> v;
	if(a->kind() == ARITH) // explore both operands and try to reduce (k*x)*c to [k*c]*x (where a=k*x)
	{
		const OperandArith& z = a->toArith();
		if(v = z.left()->evalConstantOperand())
			return dag->mul(z.right(), dag->cst(*v * c));
		else if(v = z.rightOperand().evalConstantOperand())
			return dag->mul(z.left(), dag->cst(*v * c));
		else
			return dag->mul(a, dag->cst(c));
	}
	else if(a->kind() == CST)
		return dag->cst(a->toConstant() * c);
	else
		return dag->mul(a, dag->cst(c));
}
