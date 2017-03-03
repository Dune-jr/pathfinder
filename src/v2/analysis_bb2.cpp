/*
 * V2 version of the BasicBlock top-down analysis
 */

#include <otawa/sem/PathIter.h>
#include <otawa/sem/inst.h>
#include "../analysis_state.h"
#include "../arith.h"
#include "../debug.h"
#include "../struct/DAG.h"
#include "../struct/operand.h"

using namespace otawa::sem;

/**
 * @brief      v2/v3 of abstract interpretation
 *
 * @param seminsts  	 The semantic instruction to parse
 * @param last_condition If we are in a conditional segment, the sem inst corresponding to the conditional instruction, NOP otherwise
 * @return 	0 by default, 1 if memory needs to be wiped
 */
int Analysis::State::processSemInst2(SemanticParser& semp)
{
	const t::int16 &a = semp.inst().a(), &b = semp.inst().b(), &d = semp.inst().d();
	const t::int32 &cst = semp.inst().cst();
	const t::int16 &reg = semp.inst().reg(), &addr = semp.inst().addr();
	const t::uint16 op = semp.inst().op;
	const bool in_conditional_segment = (semp.lastCond().op != NOP);

	// parse conservatively conditional segments of the BB: set top to everything that is written there
	if(in_conditional_segment && (op != IF && op != CONT && op != BRANCH))
	{	// Instruction is within the conditional segment of BB 
		if(affectsRegister(op)) // affects the register d
			semp.scratch(d);
		if(affectsMemory(op)) // affects memory at addr
			return store(OperandVar(addr), semp.new_top());
		return 0;
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
		updateLabels(semp.inst());
	
	switch(op)
	{
		case NOP:
			break;
		case BRANCH:
			break;
		case IF:
		{
			const OperandVar sr = OperandVar(semp.lastCond().sr());
			const Operand& opd = lvars[sr] ? *lvars[sr] : sr;
			if(opd.kind() == ARITH && opd.toArith().opr() == ARITHOPR_CMP)
			{
				Path labels; // empty // TODO
				Predicate p = getConditionalPredicate(semp.lastCond().cond(), opd.toArith().left(), opd.toArith().right(), true);
				generated_preds += LabelledPredicate(p, labels);
				DBG(color::IPur() << DBG_SEPARATOR << color::IGre() << " + " << p)
			}
			break;
		}
		case CONT:
		{
			const OperandVar sr = OperandVar(semp.lastCond().sr());
			const Operand& opd = lvars[sr] ? *lvars[sr] : sr;
			if(opd.kind() == ARITH && opd.toArith().opr() == ARITHOPR_CMP)
			{
				Path labels; // empty // TODO
				Predicate p = getConditionalPredicate(semp.lastCond().cond(), opd.toArith().left(), opd.toArith().right(), false); // false because we need to invert condition (else branch)
				generated_preds += LabelledPredicate(p, labels);
				DBG(color::IPur() << DBG_SEPARATOR << color::IGre() << " + " << p)
			}
			break; // we cannot generate a predicate otherwise
		}
		case LOAD: // reg <- MEM_type(addr)
			// tip: addr is likely to be t1
			// TODO!! warning for LOAD, assert false for STORE for unaligned accesses
			if(lvars.isConst(addr))
			{
				Constant c = lvars(addr).toConstant();
				if(Option<Constant> v = getConstantValueOfReadOnlyMemCell(OperandMem(c), semp.inst().type()))
				{
					DBG(color::IBlu() << "  R-O memory cell " << OperandMem(c) << " simplified to " << *v)
					set(reg, dag->cst(*v));
				}
				else
				{
					DBG(color::IBlu() << "  Reading from " << OperandMem(c))
					if(mem.exists(c))
						set(reg, mem[c]); // TODO! labels
					else
						set(reg, dag->mem(c));	
				}
			}
			else
			{
				DBG(color::IYel() << "  Load address is unknown: " << OperandVar(addr) << " = " << lvars(addr))
				semp.scratch(reg);
			}
			break;
		case STORE:	// MEM_type(addr) <- reg
			return store(OperandVar(addr), getPtr(reg)); // TODO! labels
		case SET: // d <- a
			set(d, getPtr(a));
			break;
		case SETI: // d <- cst
			set(d, dag->cst(cst));
			break;
		case SETP:
			semp.scratch(d);
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
				semp.scratch(d);
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
				semp.scratch(d);
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
				set(d, dag->cst(op == OR ? (*av | *bv) : (*av ^ *bv)));
			else
				semp.scratch(d);
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
					semp.scratch(d);
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
						semp.scratch(d);
				}
			}
			else
				semp.scratch(d);
			break;
		}
		case MULU:
			UNTESTED_CODE("unsigned variant")
		case MUL:
			set(d, Arith::mul(dag, getPtr(a), getPtr(b)));
			break;
		case MULH:
			semp.scratch(d); // TODO! do better, like generating a predicate
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
			semp.scratch(d);
			break;
	}
	if(dbg_&0x8)
		ASSERTP(lvars(context->sp).kind() != CST || lvars(context->sp).toConstant().isRelativePositive(), "SP lost: " << lvars(context->sp))
	return 0;
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

void Analysis::State::SemanticParser::scratch(const OperandVar& var)
{
	s.set(var, new_top(), false);
	s.lvars.clearLabels(var);
}

const Operand* Analysis::State::getPtr(t::int32 var_id) const
{
	return lvars[var_id] ? lvars[var_id] : dag->var(var_id);
}

/**
 * @brief      Empties the memory (first step of an access to Top)
 * @warning    This must always be followed by a call to setMemoryInitPoint
 */
void Analysis::State::wipeMemory(VarMaker& vm)
{
	DBG(color::IRed() << "  Wiping the memory (" << mem.count() << " items)")
	mem.clear();

	// collect all OperandMems
	avl::Map<const Operand*, const Operand*> topmap; // match a mem with a top
	const Operand *opdm, *opdtop;
	for(LocalVariables::Iter i(lvars); i; i++)
		if(lvars[i])
			while((opdm = lvars[i]->involvesMemory()))
			{
				opdtop = topmap.get(opdm, NULL);
				if(! opdtop)
				{
					opdtop = vm.new_top();
					topmap.put(opdm, opdtop);
				}
				if(Option<const Operand*> mb_newopd = lvars[i]->update(*dag, opdm, opdtop))
					lvars[i] = *mb_newopd;
			}

	for(MutablePredIterator piter(*this); piter; piter++)
	{
		while((opdm = piter->pred().involvesMemory()))
		{
			opdtop = topmap.get(opdm, NULL);
			if(! opdtop)
			{
				opdtop = vm.new_top();
				topmap.put(opdm, opdtop);
			}
			piter.item().updatePred(*dag, opdm, opdtop);
		}		
	}
}

/**
 * @brief      Sets the memory initial point (what the right operands will refer to).
 *
 * @param[in]  b   The block
 * @param[in]  id  The ID of the instruction
 */
void Analysis::State::setMemoryInitPoint(const otawa::Block* b, short id)
{
	memid.b = b;
	memid.id = id;
}

/**
 * @brief      Perform a store at a given address, with a given value
 *
 * @param      addr  The address
 * @param      opd   The operand
 *
 * @return     0 if success, 1 if we had to wipe the memory (access to Top)
 */
int Analysis::State::store(OperandVar addr, const Operand* opd)
{
	Constant c;
	if(lvars.isConst(addr) && (c = lvars(addr).toConstant(), c.isValidAddress()))
	{
		setMem(c, opd);
		return 0;
	}
	else
	{
		DBG(color::IYel() << "  Store address is unknown: " << addr << " = " << lvars(addr))
		return 1; // Memory will be wiped
	}
}

void Analysis::State::updateLabels(const sem::inst& seminst)
{
	switch(seminst.op)
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
			DBG(color::Cya() << "  /" << OperandVar(seminst.a()) << "=" << lvars(seminst.a()) << "/")
			lvars.label(OperandVar(seminst.d()), lvars.labels(OperandVar(seminst.a())));
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
			DBG(color::Cya() << "  /" << OperandVar(tmp=seminst.a()) << "=" << (lvars(tmp=seminst.a()))
							 << ", "  << OperandVar(tmp=seminst.b()) << "=" << (lvars(tmp=seminst.b())) << "/")
			lvars.label(OperandVar(seminst.d()), lvars.labels(OperandVar(seminst.a())));
			lvars.label(OperandVar(seminst.d()), lvars.labels(OperandVar(seminst.b())));
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
