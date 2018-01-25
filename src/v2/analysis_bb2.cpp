/*
 *	
 *
 *	This file is part of OTAWA
 *	Copyright (c) 2006-2018, IRIT UPS.
 * 
 *	OTAWA is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	OTAWA is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with OTAWA; if not, write to the Free Software 
 *	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
/*
 * V2 version of the BasicBlock top-down analysis
 */

#include <otawa/sem/PathIter.h>
#include <otawa/sem/inst.h>
#include "../analysis_state.h"
#include "../debug.h"
#include "../struct/arith.h"
#include "../struct/DAG.h"
#include "../struct/operand.h"
#include "analysis_sem2.h"

using namespace otawa::sem;

/**
 * @brief      v2/v3 of abstract interpretation
 *
 * @param inst  The semantic instruction to parse
 * @return 	    0 by default, 1 if memory needs to be wiped
 */
int Analysis::State::SemanticParser::process(const sem::PathIter& inst)
{
	const t::int16 &a = inst.a(), &b = inst.b(), &d = inst.d();
	const t::int32 &cst = inst.cst();
	const t::int16 &reg = inst.reg(), &addr = inst.addr();
	const t::uint16 op = inst.op();
	const bool in_conditional_segment = (lastCond().op != NOP);

	if(inst.isCond())
		last_condition = inst;

	// parse conservatively conditional segments of the BB: set top to everything that is written there
	if(in_conditional_segment && (op != IF && op != CONT && op != BRANCH))
	{	// Instruction is within the conditional segment of BB 
		if(affectsRegister(op)) // affects the register d
			scratch(d);
		if(affectsMemory(op)) // affects memory at addr
			return store(OperandVar(addr), new_top());
		return 0;
	}
	// maintain predicates
	if(affectsRegister(op))
		s.invalidateVar(d, false);
	if(affectsMemory(op))
	{
		Constant c;
		if(lvars.isConst(addr) && (c = lvars(addr).toConstant(), c.isValidAddress()))
			s.invalidateMem(OperandMem(c));
		else
			s.invalidateAllMemory();
	}
	// set proper labels (will be removed by scratch if necessary)
	if(! in_conditional_segment) // if we are in sequential segment of BB
		s.updateLabels(inst);
	
	switch(op)
	{
		case NOP:
			break;
		case BRANCH:
			break;
		case IF:
		case CONT: // TODO!!! this doesn't work when there is a sequence of if in the program!
		{
			const OperandVar sr = OperandVar(lastCond().sr());
			const Operand& opd = lvars[sr] ? *lvars[sr] : sr;
			if(opd.kind() == ARITH && opd.toArith().opr() == ARITHOPR_CMP)
			{
				Path labels; // empty // TODO
				Predicate p = s.getConditionalPredicate(lastCond().cond(), opd.toArith().left(), opd.toArith().right(), op == IF); // false inverts the condition (else branch)
				s.generated_preds += LabelledPredicate(p, labels);
				DBG(color::IPur() << DBG_SEPARATOR << color::IGre() << " + " << p)
			}
			break;
		}
		case LOAD: // reg <- MEM_type(addr)
			// tip: addr is likely to be t1
			// TODO!! warning for LOAD, assert false for STORE for unaligned accesses
			if(lvars.isConst(addr))
			{
				Constant c = lvars(addr).toConstant();
				if(Option<Constant> v = s.getConstantValueOfReadOnlyMemCell(OperandMem(c), (*inst).type()))
				{
					DBG(color::IBlu() << "  R-O memory cell " << OperandMem(c) << " simplified to " << *v)
					set(reg, dag.cst(*v));
				}
				else
				{
					DBG(color::IBlu() << "  Reading from " << OperandMem(c))
					if(s.mem.exists(c))
						set(reg, s.mem[c]); // TODO! labels
					else
						set(reg, dag.mem(c));	
				}
			}
			else
			{
				DBG(color::IYel() << "  Load address is unknown: " << OperandVar(addr) << " = " << lvars(addr))
				scratch(reg);
			}
			break;
		case STORE:	// MEM_type(addr) <- reg
			return store(OperandVar(addr), getPtr(reg)); // TODO! labels
		case SET: // d <- a
			set(d, getPtr(a));
			break;
		case SETI: // d <- cst
			set(d, dag.cst(cst));
			break;
		case SETP:
			scratch(d);
			break;
		case CMPU:
			UNTESTED_CODE("unsigned variant")
		case CMP:
			set(d, dag.cmp(getPtr(a), getPtr(b)));
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
					set(d, dag.cst(getPtr(a)->toConstant() / Constant(1) << lvars(b).toConstant()));
				else
					set(d, Arith::div(dag, getPtr(a), dag.cst(Constant(1) << lvars(b).toConstant())));
			}
			else
			{
				DBG(color::IYel() << "  SHL with unknown factor!")
				scratch(d);
			}
			break;
		case NEG:
			set(d, Arith::mul(dag, getPtr(b), Constant(-1)));
			break;
		case NOT: // d <- ~a
		    if(lvars.isConst(a))
		    	set(d, dag.cst(~ lvars(a).toConstant()));
		    else
		    	scratch(d);
			break;
		case OR:  // d <- a | b
		case XOR: // d <- a ^ b
		{
			if(lvars.isConst(a) && lvars.isConst(b))
				set(d, dag.cst(op == OR 
					? (lvars(a).toConstant() | lvars(b).toConstant())
					: (lvars(a).toConstant() ^ lvars(b).toConstant())));
			else
				scratch(d);
			break;
		}
		case AND: // d <- a & b
		{
			if(lvars.isConst(a) && lvars.isConst(b))
				set(d, dag.cst(lvars(a).toConstant() & lvars(b).toConstant()));
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
						set(d, dag.mod(o, dag.cst(mod_factor)));
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
			scratch(d); // TODO! do better, like generating a predicate?
			break;
		case DIVU:
			UNTESTED_CODE("unsigned variant")
		case DIV:
			set(d, Arith::div(dag, getPtr(a), getPtr(b)));
			break;
		case MODU:
		case MOD:
			if(Option<Constant> av = getPtr(a)->evalConstantOperand())
				if(Option<Constant> bv = getPtr(b)->evalConstantOperand())
				{
					set(d, dag.cst(*av % *bv));
					break;
				}
			set(d, dag.mod(getPtr(a), getPtr(b)));
			break;
		case SPEC: // special instruction (d: code, cst: sub-code)
		default:
			DBG(color::BIRed() << "Unknown seminst running!")
			ASSERT(!UNTESTED_CRITICAL)
		case SCRATCH:
			scratch(d);
			break;
	}
	if(dbg_&0x8)
		ASSERTP(lvars(s.context->sp).kind() != CST || lvars(s.context->sp).toConstant().isRelativePositive(), "SP lost: " << lvars(s.context->sp))
	return 0;
}

/**
 * @brief      Assigns expr to the variable x
 *
 * @param      x            The variable to set
 * @param      expr         The expression
 * @param      set_updated  Enable to mark x as updated
 */
void Analysis::State::set(const OperandVar& x, const Operand* expr, bool set_updated)
{
	DBG(color::IGre() << " * " << x << " = " << *expr << color::Gre() << " {" << lvars(x) << "}")
	lvars[x] = expr;
	if(set_updated)
		lvars.markAsUpdated(x);
}

/**
 * @brief      Assigns expr to the memory address addr
 *
 * @param      addr  The address
 * @param      expr  The expression
 */
void Analysis::State::setMem(Constant addr, const Operand* expr)
{
	DBG(color::IGre() << " * " << OperandMem(addr) << " = " << *expr
		<< color::Gre() << " {" << (mem.exists(addr) ? **mem[addr] : (const Operand&)OperandMem(addr)) << "}")
	mem[addr] = expr;
}

/**
 * @brief      Perform a store at a given address, with a given value
 *
 * @param      addr  The address
 * @param      opd   The operand
 *
 * @return     0 if success, 1 if we had to wipe the memory (access to Top)
 */
int Analysis::State::SemanticParser::store(OperandVar addr, const Operand* opd)
{
	Constant c;
	if(s.lvars.isConst(addr) && (c = s.lvars(addr).toConstant(), c.isValidAddress()))
	{
		DBG(color::IBlu() << "  Storing at " << OperandMem(c))
		setMem(c, opd);
		return 0;
	}
	else
	{
		DBG(color::IYel() << "  Store address is unknown: " << addr << " = " << s.lvars(addr))
		return 1; // Memory will be wiped
	}
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
					set(i, *mb_newopd);
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

void Analysis::State::clampPredicates(VarMaker &vm)
{
	for(LocalVariables::Iter i(lvars); i; i++)
		if(lvars[i] && lvars[i]->count() >= 12)
			lvars[i] = vm.new_top();

	Vector<Constant> toscratch;
	for(mem_t::PairIterator iter(mem); iter; iter++)
		if((*iter).snd->count() >= 12)
			toscratch.push((*iter).fst);
	for(Vector<Constant>::Iter i(toscratch); i; i++)
		mem[i] = vm.new_top();
	// for(MutablePredIterator piter(*this); piter; piter++)
}

void Analysis::State::updateLabels(const sem::inst& seminst)
{
	switch(seminst.op)
	{
		case NOP: case ASSUME: case BRANCH: case TRAP: case CONT: case IF: case SCRATCH: case SETI: case SETP: case SPEC: // nothing to do
		case STORE: // [addr] <- f(reg)
		case LOAD: // reg <- f([addr])
			// DBG(color::Cya() << "  /" << OperandVar(seminst.reg()) << "=" << lvars(seminst.reg()) << "/")
			break;
		case SET: case NEG: case NOT: // d <- f(a)
			DBG(color::Cya() << "  /" << OperandVar(seminst.a()) << "=" << lvars(seminst.a()) << "/")
			lvars.label(OperandVar(seminst.d()), lvars.labels(OperandVar(seminst.a())));
			break;
		// d <- f(a, b)	
		case CMP: case CMPU: case ADD: case SUB: case SHL: case SHR: case ASR: case AND: case OR:
		case XOR: case MUL: case MULU: case DIV: case DIVU: case MOD: case MODU: case MULH:
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
 * @param      op    The opcode of the semantic instruction
 * @return     The register id, if it exists
 */
bool Analysis::State::SemanticParser::affectsRegister(t::uint16 op)
{
	switch(op)
	{
		// instructions that do not write anything
		case NOP: case ASSUME: case BRANCH: case TRAP: case CONT: case IF:
		// instructions that write in memory
		case STORE:
			return false;
		// instructions that write in register d
		case LOAD: case SCRATCH: case SET: case SETI: case SETP: case CMP: case CMPU: case ADD: case SUB: case SHL: case SHR: case ASR:
		case NEG: case NOT: case AND: case OR: case XOR: case MUL: case MULU: case DIV: case DIVU: case MOD: case MODU: case SPEC: case MULH:
			return true;
		// case FORK:
		default:
			crash();
	}
}

bool Analysis::State::SemanticParser::affectsMemory(t::uint16 op)
{
	return op == STORE; // the only op that can write in memory is STORE
}
