/*
 * V2 version of the BasicBlock top-down analysis
 */

#include <otawa/sem/PathIter.h>
#include <otawa/sem/inst.h>
#include "../analysis_state.h"
#include "../arith.h"
#include "../DAG.h"
#include "../debug.h"
#include "../operand.h"

using namespace otawa::sem;

/**
 * @brief      v2/v3 of abstract interpretation
 *
 * @param seminsts  	 The semantic instruction to parse
 * @param last_condition If we are in a conditional segment, the sem inst corresponding to the conditional instruction, NOP otherwise
 */
void Analysis::State::processSemInst2(const PathIter& seminsts, const sem::inst& last_condition)
{
	const t::int16 &a = seminsts.a(), &b = seminsts.b(), &d = seminsts.d();
	const t::int32 &cst = seminsts.cst();
	const t::int16 &reg = seminsts.reg(), &addr = seminsts.addr();
	const t::uint16 op = seminsts.op();
	const bool in_conditional_segment = last_condition.op != NOP;

	// parse conservatively conditional segments of the BB: set top to everything that is written there
	if(in_conditional_segment && (op != IF && op != CONT && op != BRANCH))
	{	// Instruction is within the conditional segment of BB 
		if(affectsRegister(op)) // affects the register d
			scratch(d);
		if(affectsMemory(op)) // affects memory at addr
			store(OperandVar(addr), vm->new_top());
		return;
	}
	// maintain predicates
	if(affectsRegister(op))
		invalidateVar(d, false);
	if(affectsMemory(op))
	{
		Constant c;
		if(lvars.isConst(addr) && (c = lvars(addr).toConstant(), c.isValidAddress()))
			invalidateMem(OperandMem(c));
		else
			invalidateAllMemory();
	}
	// set proper labels (will be removed by scratch if necessary)
	if(! in_conditional_segment) // if we are in sequential segment of BB
		updateLabels(seminsts);
	
	switch(op)
	{
		case NOP:
			break;
		case BRANCH:
			break;
		case IF:
		{
			const OperandVar sr = OperandVar(last_condition.sr());
			const Operand& opd = lvars[sr] ? *lvars[sr] : sr;
			if(opd.kind() == ARITH && opd.toArith().opr() == ARITHOPR_CMP)
			{
				Path labels; // empty // TODO
				Predicate p = getConditionalPredicate(last_condition.cond(), opd.toArith().left(), opd.toArith().right(), true);
				generated_preds += LabelledPredicate(p, labels);
				DBG(color::IPur() << DBG_SEPARATOR << color::IGre() << " + " << p)
			}
		}
			break;
		case CONT:
		{
			const OperandVar sr = OperandVar(last_condition.sr());
			const Operand& opd = lvars[sr] ? *lvars[sr] : sr;
			if(opd.kind() == ARITH && opd.toArith().opr() == ARITHOPR_CMP)
			{
				Path labels; // empty // TODO
				Predicate p = getConditionalPredicate(last_condition.cond(), opd.toArith().left(), opd.toArith().right(), false); // false because we need to invert condition (else branch)
				generated_preds += LabelledPredicate(p, labels);
				DBG(color::IPur() << DBG_SEPARATOR << color::IGre() << " + " << p)
			}
		}
			break; // we cannot generate a predicate otherwise
		case LOAD: // reg <- MEM_type(addr)
			// tip: addr is likely to be t1
			// TODO!! warning for LOAD, assert false for STORE for unaligned accesses
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
					set(reg, mem[c]); // TODO! labels
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
			store(OperandVar(addr), getPtr(reg)); // TODO! labels
			break;
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
			UNTESTED_CODE("unsigned variant")
		case CMP:
			set(d, dag->cmp(getPtr(a), getPtr(b)));
			break;
		case ADD:
			set(d, Arith::add(dag, getPtr(a), getPtr(b)));
			break;
		case SUB:
			set(d, Arith::sub(dag, getPtr(a), getPtr(b)));
			break;
		case SHL: // d <- a << b
			if(lvars.isConst(b))
				set(d, Arith::mul(dag, getPtr(a), Constant(1) << lvars(b).toConstant()));
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
					set(d, Arith::div(dag, getPtr(a), dag->cst(Constant(1) << lvars(b).toConstant())));
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
			UNTESTED_CODE("unsigned variant")
		case MUL:
			set(d, Arith::mul(dag, getPtr(a), getPtr(b)));
			break;
		case MULH:
			scratch(d); // TODO! do better, like generating a predicate
			break;
		case DIVU:
			UNTESTED_CODE("unsigned variant")
		case DIV:
			if(Option<Constant> av = getPtr(a)->evalConstantOperand())
				if(Option<Constant> bv = getPtr(b)->evalConstantOperand())
				{
					set(d, dag->cst(*av / *bv));
					break;
				}
			set(d, Arith::div(dag, getPtr(a), getPtr(b)));
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
			ASSERT(!UNTESTED_CRITICAL)
		case SCRATCH:
			scratch(d);
			break;
	}
}

void Analysis::State::set(const OperandVar& x, const Operand* expr, bool set_updated)
{
	DBG(color::IGre() << " * " << x << " = " << *expr << color::Gre() << " {" << lvars(x) << "}")
	lvars[x] = expr;
	if(set_updated)
		lvars.markAsUpdated(x);
}

void Analysis::State::setMem(Constant addr, const Operand* expr)
{
	DBG(color::IGre() << " * " << OperandMem(addr) << " = " << *expr
		<< color::Gre() << " {" << (mem.exists(addr) ? **mem[addr] : (const Operand&)OperandMem(addr)) << "}")
	mem[addr] = expr;
}

const Operand* Analysis::State::getPtr(t::int32 var_id) const
{
	return lvars[var_id] ? lvars[var_id] : dag->var(var_id);
}

void Analysis::State::scratch(const OperandVar& var)
{
	set(var, vm->new_top(), false);
	lvars.clearLabels(var);
}

void Analysis::State::scratchAllMemory()
{
	DBG(color::IRed() << "  Access to Top, invalidating all memory (" << mem.count() << " items)")
	for(mem_t::MutableIter i(mem); i; i++)
		i.item() = vm->new_top(); // TODO! that looks costly... maybe we should rework the way Tops work in DAG.
}

void Analysis::State::store(OperandVar addr, const Operand* opd)
{
	Constant c;
	if(lvars.isConst(addr) && (c = lvars(addr).toConstant(), c.isValidAddress()))
		setMem(c, opd);
	else
	{
		DBG(color::IYel() << "  Store address is unknown: " << addr << " = " << lvars(addr))
		scratchAllMemory(); // access to Top
	}
}

void Analysis::State::updateLabels(const PathIter& seminsts)
{
	switch(seminsts.op())
	{
		// nothing to do
		case NOP:
		case ASSUME:
		case BRANCH:
		case TRAP:
		case CONT:
		case IF:
		case SCRATCH:
		case SETI:
		case SETP:
		case SPEC:
		// [addr] <- f(reg)
		case STORE:
		// reg <- f([addr])
		case LOAD:
			break;
		// d <- f(a)
		case SET:
		case NEG:
		case NOT:
			DBG(color::Cya() << "  /" << OperandVar(seminsts.a()) << "=" << lvars(seminsts.a()) << "/")
			lvars.label(OperandVar(seminsts.d()), lvars.labels(OperandVar(seminsts.a())));
			break;
		// d <- f(a, b)	
		case CMP:
		case CMPU:
		case ADD:
		case SUB:
		case SHL:
		case SHR:
		case ASR:
		case AND:
		case OR:
		case XOR:
		case MUL:
		case MULU:
		case DIV:
		case DIVU:
		case MOD:
		case MODU:
		case MULH:
		{
			OperandVar tmp;
			DBG(color::Cya() << "  /" << OperandVar(tmp=seminsts.a()) << "=" << (lvars(tmp=seminsts.a()))
							 << ", "  << OperandVar(tmp=seminsts.b()) << "=" << (lvars(tmp=seminsts.b())) << "/")
			lvars.label(OperandVar(seminsts.d()), lvars.labels(OperandVar(seminsts.a())));
			lvars.label(OperandVar(seminsts.d()), lvars.labels(OperandVar(seminsts.b())));
			break;
		}
		// case FORK:
		default:
			crash();
	}
}

/**
 * @brief      Determines the register written by the semantic instruction, if it exists
 *
 * @param[in]  op    The opcode of the semantic instruction
 *
 * @return     The register id, if it exists
 */
bool Analysis::State::affectsRegister(t::uint16 op)
{
	switch(op)
	{
		// instructions that do not write anything
		case NOP:
		case ASSUME:
		case BRANCH:
		case TRAP:
		case CONT:
		case IF:
		// instructions that write in memory
		case STORE:
			return false;
		// instructions that write in register d
		case LOAD:
		case SCRATCH:
		case SET:
		case SETI:
		case SETP:
		case CMP:
		case CMPU:
		case ADD:
		case SUB:
		case SHL:
		case SHR:
		case ASR:
		case NEG:
		case NOT:
		case AND:
		case OR:
		case XOR:
		case MUL:
		case MULU:
		case DIV:
		case DIVU:
		case MOD:
		case MODU:
		case SPEC:
		case MULH:
			return true;
		// case FORK:
		default:
			crash();
	}
}

bool Analysis::State::affectsMemory(t::uint16 op)
{
	return op == STORE; // the only op that can write in memory is STORE
}
