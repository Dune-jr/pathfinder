/*
 * Micro analysis: defines how we parse a BasicBlock
 */

#include <otawa/cfg/BasicBlock.h>
#include <otawa/sem/inst.h>
#include <otawa/sem/PathIter.h>
#include <elm/string/String.h>
#include "predicate.h"
#include "operand.h"
#include "debug.h"
#include "analysis_state.h"

using namespace otawa::sem;

void Analysis::State::processBB(const BasicBlock *bb)
{
	DBG("Processing " << (otawa::Block*)bb << " (" << bb->address() << ") of path " << getPathString())
	SLList<LabelledPredicate> generated_preds_before_condition;
	sem::inst condition;
	generated_preds.clear();
	generated_preds_taken.clear();
	
	// parse assembly instructions
	for(BasicBlock::InstIter insts(bb); insts; insts++)
	{
		DBG(color::BIPur() << *insts)
		sem::Block block;
		insts->semInsts(block);
		
		PathIter seminsts;
		// parse semantical instructions with PathIter
		for(seminsts.start(*insts); seminsts; seminsts++)
		{
			DBG(color::IPur() << *seminsts)
			
			if(seminsts.isCond()) // IF
			{	// backup the list of generated predicates before entering the condition
				generated_preds_before_condition.addAll(generated_preds); // side effect: reverses the order of the list
				DBG(color::IBlu() << "(Parsing taken path)")
			}
			if(seminsts.pathEnd()) // CONT
			{ 	// dumping the current generated_preds into the previous_paths_preds list
				invalidateTempVars(); // do the end-of-instruction tempvar invalidation first
				DBG(color::IBlu() << "(Parsing not taken path)")
				generated_preds_taken = generated_preds;
				generated_preds = generated_preds_before_condition;
			}
			
			Operand *opd1 = NULL, *opd2 = NULL, *opd11 = NULL, *opd12 = NULL, *opd21 = NULL, *opd22 = NULL;
			condoperator_t opr = CONDOPR_EQ; // default is =
			Path labels; // default is {}
			bool make_pred = false;
			
			// some shortcuts (the seminsts.F functions are not called at this point)
			const t::int16 &a = seminsts.a(), &b = seminsts.b(), &d = seminsts.d();
			const t::int32 &cst = seminsts.cst();
			const t::int16 &reg = seminsts.reg(), &addr = seminsts.addr();
			
			switch(seminsts.op())
			{
				case NOP:
					break;
				case BRANCH:
					break;
				case CONT:
					{
						Predicate *p;
						if(!(p = getPredicateGeneratedByCondition(condition, false, labels)))
							break; // in view of this, we cannot generate a predicate 
						make_pred = true;
						opr = p->opr();
						opd1 = p->leftOperand().copy();
						opd2 = p->rightOperand().copy();
						delete p;
					}
					break;
				case IF:
					condition = *seminsts; // save this for later
					{
						Predicate *p;
						if(!(p = getPredicateGeneratedByCondition(condition, true, labels)))
							break; // in view of this, we cannot generate a predicate 
						make_pred = true;
						opr = p->opr();
						opd1 = p->leftOperand().copy();
						opd2 = p->rightOperand().copy();
						delete p;
					}
					break;
				case LOAD: // reg <- MEM_type(addr)
					invalidateVar(reg); // TODO: shouldn't we invalidate reg later incase we LOAD ?11, ?11
					if(Option<OperandMem> addr_mem = getOperandMem(addr, labels))
					{
						// if it's a constant address of some read-only data
						if(Option<OperandConst> addr_const_value = getConstantValueOfReadOnlyMemCell(*addr_mem, (*seminsts).type()))
						{
							DBG(color::IPur() << DBG_SEPARATOR " " << color::IBlu() << "R-O memory data " << *addr_mem << " simplified to " << *addr_const_value)
							constants.set(d, ConstantVariables::LabelledValue((*addr_const_value).value(), labels, true)); 
						}
						// or maybe we can link it to a constant value from the predicates
						else if(Option<OperandConst> addr_const_value = findConstantValueOfMemCell(*addr_mem, labels))
						{
							DBG(color::IPur() << DBG_SEPARATOR " " << color::IBlu() << "Memory data " << *addr_mem << " simplified to " << *addr_const_value)
							constants.set(d, ConstantVariables::LabelledValue((*addr_const_value).value(), labels, true));
						}
						else
						{							
							make_pred = true;
							opd1 = new OperandVar(reg);
							opd2 = new OperandMem(*addr_mem);
						}
					}
					else
					{
						OperandVar addr_var = OperandVar(addr);
						DBG(color::IPur() << DBG_SEPARATOR " " << color::IYel() << "Could not simplify " << addr_var)
					}
					break;
				case STORE:	// MEM_type(addr) <- reg
					if(Option<OperandMem> addr_mem = getOperandMem(addr, labels))
					{
						invalidateMem(*addr_mem);
						make_pred = true;
						opd1 = new OperandVar(reg);
						opd2 = new OperandMem(*addr_mem);
					}
					else
					{
						OperandVar addr_var = OperandVar(addr);
						DBG(color::IPur() << DBG_SEPARATOR " " << color::IYel() << "Could not simplify " << addr_var << ", invalidating whole memory")
						invalidateAllMemory();
					}
					break;
				case SET:
					invalidateVar(d);
					opd1 = new OperandVar(d);
					opd2 = new OperandVar(a);
					if(isConstant(a)) // if a is already identified as a constant
						constants.set(d, constants[a], getLabels(a)); // then constants[d] = constants[a]
					else // no use generating this predicate if it is a constant, because the ConstantVariables object handles that
						make_pred = true; // d = already
					break;
				case SETI:
					invalidateVar(d);
					// everything should already be handled by the ConstantVariables object
					constants.set(d, cst); // constants[d] = cst
					break;
				case SETP:
					DBG(color::BIRed() << "Unimplemented operator SETP running!")
					ASSERT(!UNTESTED_CRITICAL);
					invalidateVar(d);
					break;
				case CMP:
				case CMPU:
					make_pred = true;
					opd1 = new OperandVar(d);
					opd21 = new OperandVar(a);
					opd22 = new OperandVar(b);
					opd2 = new OperandArithExpr(ARITHOPR_CMP, *opd21, *opd22);
					invalidateVar(d);
					break;
				case ADD:
					if(d == a)
					{
						if(d == b) // d <- d+d
						{	// [d/2 / d]
							update(OperandVar(d), OperandArithExpr(ARITHOPR_DIV, OperandVar(d), OperandConst(2)), labels);
							opd11 = new OperandVar(d);
							opd12 = new OperandVar(2);
							opd1 = new OperandArithExpr(ARITHOPR_MOD, *opd11, *opd12);
							opd2 = new OperandConst(0);
							make_pred = true; // d % 2 = 0
							if(isConstant(d))
								constants.set(d, constants[d]*2, getLabels(d));
						}
						else // d <- d+b
						{	// [d-b / d]
							update(OperandVar(d), OperandArithExpr(ARITHOPR_SUB, OperandVar(d), OperandVar(b)), labels);
							if(isConstant(d))
							{
								if(isConstant(b))
									constants.set(d, constants[d]+constants[b], getLabels(d, b));
								else
								{
									// d = constants[d] + b
									opd1 = new OperandVar(d);
									opd21 = new OperandConst(constants[d]);
									opd22 = new OperandVar(b);
									opd2 = new OperandArithExpr(ARITHOPR_ADD, *opd21, *opd22);
									make_pred = true;

									constants.invalidate(d);
								}
							}
						}
					}
					else 
					{
						if(d == b) // d <- d+a
						{	// [d-a / d]
							update(OperandVar(d), OperandArithExpr(ARITHOPR_SUB, OperandVar(d), OperandVar(a)), labels);
							if(isConstant(d))
							{
								if(isConstant(a))
									constants.set(d, constants[d]+constants[a], getLabels(d, a));
								else
								{
									// d = a + constants[d]
									opd1 = new OperandVar(d);
									opd21 = new OperandVar(a);
									opd22 = new OperandConst(constants[d]);
									opd2 = new OperandArithExpr(ARITHOPR_ADD, *opd21, *opd22);
									make_pred = true;

									constants.invalidate(d);
								}
							}
						}
						else
						{
							// d is not related to a or b, invalidate predicates that involve d
							invalidateVar(d);
							opd1 = new OperandVar(d);
							opd21 = new OperandVar(a);
							opd22 = new OperandVar(b);
							opd2 = new OperandArithExpr(ARITHOPR_ADD, *opd21, *opd22);
							if(isConstant(a) && isConstant(b))
								constants.set(d, constants[a]+constants[b], getLabels(a, b));
							else make_pred = true; // d = a+b
							// the invalidation in constants is already handled by invalidateVar
						}
					}
					break;
				case SUB:
					if(d == a)
					{
						if(d == b) // d <- d-d
						{	// [0 / d], d is set to 0!
							invalidateVar(d);
							constants.set(d, 0);
						}
						else // d <- d-b
						{	// [d+b / d]
							update(OperandVar(d), OperandArithExpr(ARITHOPR_ADD, OperandVar(d), OperandVar(b)), labels);
							if(isConstant(d))
							{
								if(isConstant(b))
									constants.set(d, constants[d]-constants[b], getLabels(d, b));
								else
								{
									// d = constants[d] - b
									opd1 = new OperandVar(d);
									Operand* opd21 = new OperandConst(constants[d]);
									Operand* opd22 = new OperandVar(b);
									opd2 = new OperandArithExpr(ARITHOPR_SUB, *opd21, *opd22);
									make_pred = true;

									constants.invalidate(d);
								}
							}
						}
					}
					else
					{
						if(d == b) // d <- a-d
						{	// [a-d / d], this function f has a f°f=Id property
							update(OperandVar(d), OperandArithExpr(ARITHOPR_SUB, OperandVar(a), OperandVar(d)), labels);
							if(isConstant(d))
							{
								if(isConstant(a)) // everything is const, update const value of d
									constants.set(d, constants[a]-constants[d], getLabels(a, d));
								else
								{
									// d = a - constants[d]
									opd1 = new OperandVar(d);
									opd21 = new OperandVar(a);
									opd22 = new OperandConst(constants[d]);
									opd2 = new OperandArithExpr(ARITHOPR_SUB, *opd21, *opd22);
									make_pred = true;

									constants.invalidate(d);
								}
							}
						}
						else // d <- a-b
						{
							invalidateVar(d);
							opd1 = new OperandVar(d);
							opd21 = new OperandVar(a);
							opd22 = new OperandVar(b);
							opd2 = new OperandArithExpr(ARITHOPR_SUB, *opd21, *opd22);
							if(isConstant(a) && isConstant(b))
								constants.set(d, constants[a]-constants[b], getLabels(a, b));
							else make_pred = true; // d = a-b
							// the invalidation in constants is already handled by invalidateVar
						}
					}
					break;
				case SHL:
					opd1 = new OperandVar(d);
					{
						/*if(d == b) // safety test, shouldn't happen unless assembly language allows variable bit shifting
						{	// well actually ldr r3, [r3, r2, lsl #2] makes it happen in ARM...
							invalidateVar(d);
							break;
						}*/
						// b is usually a tempvar that has been previously set to a constant value
						if(!isConstant(b) || !constants[b].isAbsolute())
						{
							DBG(color::Blu() << "  [" << OperandVar(b) << " could not be identified to a constant value]")
							invalidateVar(d); // only invalidate now (otherwise we can't find t1 for shl t1, ?0, t1)
							break;
						}
						t::int32 b_val = constants[b].val();
						DBG(color::Blu() << "  [" << OperandVar(b) << " identified as 0x" << io::hex(b_val) << "]")
						if(d == a) // d <- d << b
						{
							labels = getLabels(b);
							update(OperandVar(d), OperandArithExpr(ARITHOPR_DIV, OperandVar(d), OperandConst(1 << b_val)), labels);
							// we also add a predicate to say that d is now a multiple of 2^b
							if(isConstant(d)) // we must update constant value of d
							{
								const Path& l = getLabels(d, b);
								constants.set(d, constants[d] << b_val, l);
							}
							else
							{ 
								opd11 = new OperandVar(d);
								opd12 = new OperandConst(1 << b_val);
								opd1 = new OperandArithExpr(ARITHOPR_MOD, *opd11, *opd12);
								opd2 = new OperandConst(0);
								make_pred = true; // will use labels
							}
						}
						else // d <- a << b
						{
							if(isConstant(a))
							{
								const Path& l = getLabels(a, b);
								invalidateVar(d);
								constants.set(d, constants[a] << b_val, l);
							}
							else
							{
								invalidateVar(d);
								opd21 = new OperandVar(a);
								opd22 = new OperandConst(1 << b_val); // 2^b_val
								opd2 = new OperandArithExpr(ARITHOPR_MUL, *opd21, *opd22);
								make_pred = true;
							}
						}
					}
					break;
				case ASR: // TODO test: is this really legit?
				case SHR:
					opd1 = new OperandVar(d);
					{
						// b is usually a tempvar that has been previously set to a constant value
						if(!isConstant(b) || !constants[b].isAbsolute())
						{
							DBG(color::Blu() << "  [" << OperandVar(b) << " could not be identified as a constant value]")
							invalidateVar(d);
							break;
						}
						else if(d == a) // d <- d >> b
						{
							invalidateVar(d);
							break; // not much we can do because we lost info (cf (*))
						}
						else // d <- a >> b
						{
							t::int32 b_val = constants[b].val();
							DBG(color::Blu() << "  [" << OperandVar(b) << " identified as 0x" << io::hex(b_val) << "]")
							if(isConstant(a))
							{
								const Path& l = getLabels(a, b);
								invalidateVar(d);
								constants.set(d, constants[a] >> b_val, l);
							}
							else
							{
								labels = getLabels(b);
								invalidateVar(d);
								opd21 = new OperandVar(a);
								opd22 = new OperandConst(1 << b_val); // 2^b_val
								opd2 = new OperandArithExpr(ARITHOPR_DIV, *opd21, *opd22);
								make_pred = true; // this will use labels
							}
						}
					}
					break;
				case NEG: // TODO test
					ASSERT(!UNTESTED_CRITICAL);
					DBG(color::BIRed() << "Untested operator running!")
					{
						if(a == d) // d <- -d
						{	// [-1 * d / d]
							update(OperandVar(d), OperandArithExpr(ARITHOPR_NEG, OperandVar(d)), labels); // TODO test
							if(isConstant(d))
								constants.set(d, -constants[d], getLabels(d));
						}
						else
						{	
							invalidateVar(d);
							opd1 = new OperandVar(d);
							opd21 = new OperandConst(-1);
							opd22 = new OperandVar(a);
							opd2 = new OperandArithExpr(ARITHOPR_MUL, *opd21, *opd22);
							if(isConstant(a))
								constants.set(d, -constants[a], getLabels(a));
							else make_pred = true;
						}
					}
					break;
				case NOT: // d <- ~a
					if(isConstant(a) && constants[a].isAbsolute()) 
					{
						t::int32 a_val = ~(constants[a].val());
						if(a_val < 0)
							a_val = -a_val; // TODO: handle better with unsigned support
						labels = getLabels(a); // save before in case d==a
						invalidateVar(d);
						constants.set(d, a_val, labels);
					}
					else invalidateVar(d);
					break;
				case AND: // d <- a & b
				case OR:  // d <- a | b
				case XOR: // d <- a ^ b
					if(isConstant(a) && isConstant(b) && constants[a].isAbsolute() && constants[b].isAbsolute())
					{
						t::int32 val;
						if(seminsts.op() == AND)
							val = constants[a].val() & constants[b].val();
						else if(seminsts.op() == XOR)
							val = constants[a].val() ^ constants[b].val();
						else //if(seminsts.op() == OR)
							val = constants[a].val() | constants[b].val();
						labels = getLabels(a, b);
						invalidateVar(d); // don't do this earlier in case d==a or d==b
						constants.set(d, val, labels);
					}
					else
					{
						DBG(color::Blu() << "  [An operand could not be identified as a constant value]")
						invalidateVar(d);
					}
					break;
				case MULU:
					ASSERT(!UNTESTED_CRITICAL);
					DBG(color::BIRed() << "Untested unsigned variant running!")
				case MUL:
					if(d == a)
					{
						if(d == b) // d <- d*d
						{
							ASSERT(!UNTESTED_CRITICAL);
							DBG(color::BIRed() << "Untested case of operator MUL running!")
							invalidateVar(d, KEEP_CONSTANT_INFO); // we have no way to do [sqrt(d) / d], so just invalidate
							opr = CONDOPR_LE; // and add a "0 <= d" predicate
							opd1 = new OperandConst(0);
							opd2 = new OperandVar(d);
							make_pred = true;
							if(isConstant(d))
								constants.set(d, constants[d]*constants[d], getLabels(d));
							else
								constants.invalidate(d);
						}
						else // d <- d*b
						{	// [d/b / d] // we will have to assume that 0/0 is scratch!
							if(isConstant(d))
							{
								invalidateVar(d, KEEP_CONSTANT_INFO); // keep constants[d], we'll need it
								if(isConstant(b))
									constants.set(d, constants[d]*constants[b], getLabels(d, b));
								else // d <- #d * b
								{
									opd1 = new OperandVar(d);
									opd2 = new OperandArithExpr(ARITHOPR_MUL, OperandConst(constants[d]), OperandVar(b));
									make_pred = true;
									constants.invalidate(d);
								}
							}
							else
							{	// we add a predicate to say that d is now a multiple of b
								update(OperandVar(d), OperandArithExpr(ARITHOPR_DIV, OperandVar(d), OperandVar(b)), labels);
								if(isConstant(b))
								{
									opd11 = new OperandVar(d);
									opd12 = new OperandConst(constants[b]);
									opd1 = new OperandArithExpr(ARITHOPR_MOD, *opd11, *opd12); // d % b (%0 is to consider!)
									opd2 = new OperandConst(0);
									make_pred = true;
								} // I don't think it's worth it to add this "b divides d" predicate if b is not constant... maybe I'm wrong
							}
						}
					}
					else
					{
						if(d == b) // d <- a*d
						{	// [d/a / d] // we will have to assume that 0/0 is scratch!
							if(isConstant(d))
							{
								invalidateVar(d, KEEP_CONSTANT_INFO); // keep constants[d], we'll need it
								if(isConstant(a))
									constants.set(d, constants[d]*constants[a], getLabels(d, a));
								else
								{
									opd1 = new OperandVar(d);
									opd2 = new OperandArithExpr(ARITHOPR_MUL, OperandConst(constants[d]), OperandVar(a));
									make_pred = true;
									constants.invalidate(d);
								}
							}
							else
							{	// we add a predicate to say that d is now a multiple of a
								update(OperandVar(d), OperandArithExpr(ARITHOPR_DIV, OperandVar(d), OperandVar(a)), labels);
								if(isConstant(a))
								{
									opd11 = new OperandVar(d);
									opd12 = new OperandConst(constants[a]);
									opd1 = new OperandArithExpr(ARITHOPR_MOD, *opd11, *opd12); // d % a (%0 is to consider!)
									opd2 = new OperandConst(0);
									make_pred = true;
								} // I don't think it's worth it to add this "a divides d" predicate if a is not constant... maybe I'm wrong
							}
						}
						else // d <- a*b
						{
							invalidateVar(d);
							if(isConstant(a) && isConstant(b))
								constants.set(d, constants[a]*constants[b], getLabels(a, b));
							else
							{
								if(isConstant(a))
									opd21 = new OperandConst(constants[a]);
								else
									opd21 = new OperandVar(a);
								if(isConstant(b))
									opd22 = new OperandConst(constants[b]);
								else
									opd22 = new OperandVar(b);
								opd1 = new OperandVar(d);
								opd2 = new OperandArithExpr(ARITHOPR_MUL, *opd21, *opd22);
								make_pred = true;
							}
						}
					}
					break;
				case MULH:
					if(d == a)
					{
						if(d == b) // d <- d*d >>32
						{
							DBG(color::BIRed() << "Untested case of operator MULH running!")
							ASSERT(!UNTESTED_CRITICAL);
							invalidateVar(d, KEEP_CONSTANT_INFO); // we have no way to do [sqrt(d) / d], so just invalidate
							opr = CONDOPR_LE; // and add a "0 <= d" predicate
							opd1 = new OperandConst(0);
							opd2 = new OperandVar(d);
							make_pred = true;
							if(isConstant(d))
								constants.set(d, (t::int64(constants[d].val())*t::int64(constants[d].val()))>>32, getLabels(d));
							else
								constants.invalidate(d);
						}
						else // d <- d*b >>32
						{	// [d/b / d] // we will have to assume that 0/0 is scratch!
							invalidateVar(d, KEEP_CONSTANT_INFO);
							if(isConstant(d))
							{
								if(isConstant(b))
									constants.set(d, (t::int64(constants[d].val())*t::int64(constants[b].val()))>>32, getLabels(d, b));
								else
								{
									DBG(color::BIRed() << "Untested case of operator MULH running!")
									ASSERT(!UNTESTED_CRITICAL);
									opd1 = new OperandVar(d);
									opd2 = new OperandArithExpr(ARITHOPR_MULH, OperandConst(constants[d]), OperandVar(b));
									make_pred = true;
									constants.invalidate(d);
								}
							}
							else
								constants.invalidate(d);
						}
					}
					else
					{
						if(d == b) // d <- a*d >>32
						{	// [d/a / d] // we will have to assume that 0/0 is scratch!
							invalidateVar(d, KEEP_CONSTANT_INFO);
							if(isConstant(d))
							{
								if(isConstant(a))
									constants.set(d, (t::int64(constants[a].val())*t::int64(constants[d].val()))>>32, getLabels(d, a));
								else
								{
									opd1 = new OperandVar(d);
									opd2 = new OperandArithExpr(ARITHOPR_MULH, OperandVar(a), OperandConst(constants[d]));
									make_pred = true;
									constants.invalidate(d);
								}
							}
							else
								constants.invalidate(d);
						}
						else // d <- a*b >>32
						{
							invalidateVar(d);
							if(isConstant(a) && isConstant(b))
								constants.set(d, (t::int64(constants[a].val())*t::int64(constants[b].val()))>>32, getLabels(a, b));
							else
							{
								if(isConstant(a))
									opd21 = new OperandConst(constants[a]);
								else
									opd21 = new OperandVar(a);
								if(isConstant(b))
									opd22 = new OperandConst(constants[b]);
								else
									opd22 = new OperandVar(b);
								opd1 = new OperandVar(d);
								opd2 = new OperandArithExpr(ARITHOPR_MULH, *opd21, *opd22);
								make_pred = true;
							}
						}
					}
					break;
				case DIVU:
					DBG(color::BIRed() << "Untested unsigned variant running!")
					// ASSERT(!UNTESTED_CRITICAL);
				case DIV:
					if(d == a)
					{
						if(d == b) // d <- d / d
						{	// [1 / d]
							// TODO: this should be okay to assume d != 0, right? otherwise the program is faulty?
							// or should we use a SMT solver mode that supports / 0?
							DBG(color::BIRed() << "Untested operator running!")
							ASSERT(!UNTESTED_CRITICAL);
							invalidateVar(d);
							opd1 = new OperandVar(d);
							opd2 = new OperandConst(1);
							make_pred = true; // d = 1
							constants.set(d, 1);
						}
						else // d <- d / b
						{	/* TODO (*): we can actually handle this, the following way:
							* Example: 2y - (x + z) = 5		(problem: if - is *, there is no bijective reverse function...)
							* Instruction : x <- x / 2 		(can be DIV or SHR/ASR!)
							* <=> x + z = 2y - 5
							* <=> x = 2y - 5 - z
							* Now x <- x / 2
							* <=> x = (2y - 5 - z) / 2y 		(instead of trying to update x!)
							* 
							* this looks hard to implement though
							*/
							invalidateVar(d, KEEP_CONSTANT_INFO); // we cannot just write [d*a / d] because we lost info
							if(isConstant(d))
							{
								if(isConstant(b))
									constants.set(d, constants[d]/constants[b], getLabels(d, b));
								else
								{
									DBG(color::BIRed() << "Untested operator running!")
									ASSERT(!UNTESTED_CRITICAL);
									Constant d_val = constants[d]; // remember this value to use it in predicate
									constants.invalidate(d);
									opd1 = new OperandVar(d);
									opd21 = new OperandConst(d_val);
									opd22 = new OperandVar(b);
									opd2 = new OperandArithExpr(ARITHOPR_DIV, *opd21, *opd22);
									make_pred = true;
								}
							}
						}
					}
					else
					{
						if(d == b) // d <- a / d
						{
							invalidateVar(d, KEEP_CONSTANT_INFO);
							if(isConstant(d))
							{
								if(isConstant(a))
									constants.set(d, constants[a]/constants[d], getLabels(a, d));
								else
								{
									Constant d_val = constants[d]; // remember this value to use it in predicate
									constants.invalidate(d);
									opd1 = new OperandVar(d);
									opd21 = new OperandVar(a);
									opd22 = new OperandConst(d_val);
									opd2 = new OperandArithExpr(ARITHOPR_DIV, *opd21, *opd22);
									make_pred = true;
								}
							}
						}
						else // d <- a / b
						{
							invalidateVar(d);
							opd1 = new OperandVar(d);
							opd21 = new OperandVar(a);
							opd22 = new OperandVar(b);
							opd2 = new OperandArithExpr(ARITHOPR_DIV, *opd21, *opd22);
							if(isConstant(a) && isConstant(b))
								constants.set(d, constants[a]/constants[b], getLabels(a, b));
							else make_pred = true; // d = a / b
						}
					}
					break;
				case MODU:
				case MOD:
					invalidateVar(d);
					if(d == a || d == b)
						break;
					opd1 = new OperandVar(d);
					{
						opd21 = new OperandVar(a);
						opd22 = new OperandVar(b);
						opd2 = new OperandArithExpr(ARITHOPR_MOD, *opd21, *opd22);
					}
					if(isConstant(a) && isConstant(b))
						constants.set(d, constants[a]%constants[b], getLabels(a, b));
					else make_pred = true;
					break;
				case SPEC: // special instruction (d: code, cst: sub-code)
					invalidateVar(d);
					break;
				default:
					DBG(color::BIRed() << "Unknown seminst running!")
					ASSERT(!UNTESTED_CRITICAL);
				case SCRATCH:
					invalidateVar(d);
					make_pred = false;
			}
			if(make_pred)
				generated_preds += makeLabelledPredicate(opr, opd1, opd2, labels);
			else
			{
				delete opd1;
				delete opd2;
			}
			delete opd11;
			delete opd12;
			delete opd21;
			delete opd22;
		}
		// all temporary variables are freed at the end of any assembly instruction, so invalidate them
		invalidateTempVars();
	}
	// DBG(dumpEverything()); // TODO!!!
	if(dbg_verbose == DBG_VERBOSE_ALL)
	{
		if(generated_preds_taken)
		{
			DBG("Predicates generated: ")
			DBG("|-> taken path: " << generated_preds_taken)
			DBG("|-> not taken path: " << generated_preds)
			DBG("Constants updated: " << constants.printChanges())
		}
		else
		{
			DBG("Predicates generated: " << generated_preds)
			DBG("Constants updated: " << constants.printChanges()) // TODO! check that a mess doesn't happen with constants being updated by both taken and not taken path
		}
	}
}

// do all the preprocessing and pretty printing job for making a predicate, then return the final labelled predicate ready to be added to the list
// this function handles the deletion of opd1 and opd2
LabelledPredicate Analysis::State::makeLabelledPredicate(condoperator_t opr, Operand* opd1, Operand* opd2, Path& labels) const
{
	ASSERT(opd1 && opd2);
	const ConstantVariablesSimplified& cvs = constants.toSimplified(); // TODO: This is taking ressources because of bad class design
	Vector<OperandVar> replaced_vars;
	// If we have predicates such as ?16 = ?4 ~ t1, make sure none of these are identified as constants in the constantVariables table!
	if(Option<Operand*> maybe_opd1 = opd1->replaceConstants(cvs, replaced_vars))
	{
		delete opd1;
		opd1 = *maybe_opd1;
	}
	if(Option<Operand*> maybe_opd2 = opd2->replaceConstants(cvs, replaced_vars))
	{
		delete opd2;
		opd2 = *maybe_opd2;
	}
	if(dbg_verbose == DBG_VERBOSE_ALL && replaced_vars) // pretty printing of replaceConstants
	{
		int count = 0;
		elm::String str = "";
		for(Vector<OperandVar>::Iterator iter(replaced_vars); iter; iter++)
		{
			if(count++)
				str = _ << str << ", ";
			str = _ << str << *iter << ":" << constants[*iter];
			if(constants.getLabels(*iter))
				str = _ << str << pathToString(constants.getLabels(*iter));	
		}
			DBG(color::IPur() << DBG_SEPARATOR << color::IBlu() << " Replaced constant" << (count>1?"s ":" ") << str)
	}
	updateLabelsWithReplacedConstantsInfo(labels, replaced_vars);
	const Predicate& p = Predicate(opr, *opd1, *opd2);
	DBG(color::IPur() << DBG_SEPARATOR << color::IGre() << " + " << p)
	delete opd1;
	delete opd2;
	return LabelledPredicate(p, labels);
}

// warning: if successful, removes a predicate
bool Analysis::State::tryToKeepVar(const OperandVar& var)//, const Predicate*& removed_predicate)
{
	// removed_predicate = NULL;
	// try and identify a value for ?3 (look for a ?3 = X predicate)
	for(PredIterator piter(*this); piter; piter++)
	{
		const Predicate &p = piter.pred();
		if(p.opr() == CONDOPR_EQ)
		{
			const int left_involves_var = p.leftOperand().involvesVariable(var);
			const int right_involves_var = p.rightOperand().involvesVariable(var);
			if(left_involves_var == 1 && right_involves_var == 0) // var is in left operand
			{
				if(p.leftOperand() == var) // left operand is exactly var (that is, predicate is "var = ???")
				{
					Operand* expr = p.rightOperand().copy(); // backup the "???" expr
					// removed_predicate = &p;
					removePredicate(piter); // remove current predicate
					replaceVar(var, *expr); // [??? / var]
					DBG(color::IPur() << DBG_SEPARATOR << color::IYel() << " - " << Predicate(CONDOPR_EQ, var, *expr))
					delete expr;
					return true;
				}
			}
			else if(left_involves_var == 0 && right_involves_var == 1) // var is in right operand
			{
				if(p.rightOperand() == var) // right operand is exactly var (that is, predicate is "??? = var")
				{
					Operand* expr = p.leftOperand().copy(); // backup the "???" expr
					// removed_predicate = &p;
					removePredicate(piter); // remove current predicate
					replaceVar(var, *expr); // [??? / var]
					DBG(color::IPur() << DBG_SEPARATOR << color::IYel() << " - " << Predicate(CONDOPR_EQ, *expr, var))
					delete expr;
					return true;
				}
			}
		}
	}
	return false;
}

// returns true if a variable has been invalidated
bool Analysis::State::invalidateVar(const OperandVar& var, bool invalidate_constant_info)
{
	// const Predicate* removed_predicate; // throw this away
	bool rtn = tryToKeepVar(var);//, removed_predicate);
	if(!rtn) // no X expression has been found to match ?3 = X, thus we have to remove every occurrence of ?3
	{
		for(PredIterator piter(*this); piter; )
		{
			if(piter.pred().involvesVariable(var))
			{
				DBG(color::IPur() << DBG_SEPARATOR << color::IYel() << " - " << *piter)
				removePredicate(piter);
				rtn = true;
			}
			else piter++;
		}
	}

	if(invalidate_constant_info)
		constants.invalidate(var);
	return rtn;
}

// generic function on works
// TODO: documentation
bool Analysis::State::invalidate(const Operand& var, bool invalidate_constant_info)
{
	bool rtn = false;

	// try and identify a value for ?3 (look for a ?3 = X predicate)
	for(PredIterator piter(*this); piter; piter++)
	{
		const Predicate &p = piter.pred();
		if(p.opr() == CONDOPR_EQ)
		{
			const int left_involves_var = p.leftOperand().involvesOperand(var);
			const int right_involves_var = p.rightOperand().involvesOperand(var);
			if(left_involves_var == 1 && right_involves_var == 0) // var is in left operand
			{
				if(p.leftOperand() == var) // left operand is exactly var (that is, predicate is "var = ???")
				{
					Operand* expr = p.rightOperand().copy(); // backup the "???" expr
					removePredicate(piter); // remove current predicate
					replaceOperand(var, *expr); // [??? / var]
					DBG(color::IPur() << DBG_SEPARATOR << color::IYel() << " - " << Predicate(CONDOPR_EQ, var, *expr))
					delete expr;
					rtn = true;
					break; // stop and move on
				}
			}
			else if(left_involves_var == 0 && right_involves_var == 1) // var is in right operand
			{
				if(p.rightOperand() == var) // right operand is exactly var (that is, predicate is "??? = var")
				{
					Operand* expr = p.leftOperand().copy(); // backup the "???" expr
					removePredicate(piter); // remove current predicate
					replaceOperand(var, *expr); // [??? / var]
					DBG(color::IPur() << DBG_SEPARATOR << color::IYel() << " - " << Predicate(CONDOPR_EQ, *expr, var))
					delete expr;
					rtn = true;
					break; // stop and move on
				}
			}
		}
	}

	if(!rtn) // no X expression has been found to match ?3 = X, thus we have to remove every occurrence of ?3
	{
		for(PredIterator piter(*this); piter; )
		{
			if(piter.pred().involvesOperand(var))
			{
				DBG(color::IPur() << DBG_SEPARATOR << color::IYel() << " - " << *piter)
				removePredicate(piter);
				rtn = true;
			}
			else piter++;
		}
	}

	if(invalidate_constant_info)
		constants.invalidateOperand(var);
	return rtn;
}

/**
 * This tries to find a matching OperandMem for the provided OperandVar, then calls invalidateMem(OperandMem)
 */
bool Analysis::State::invalidateMem(const OperandVar& var)
{
	Path labels_to_throw;
	Option<OperandMem> addr = getOperandMem(var, labels_to_throw);
	if(!addr)
	{
		DBG(color::IPur() << DBG_SEPARATOR " " << color::IYel() << "Could not simplify " << var << ", invalidating whole memory")
		return invalidateAllMemory();
	}
	return invalidateMem(*addr);
}

/**
 * Replace every occurrence of the OperandMem parameter by a constant value if possible, removes the occurrences from the predicate lists otherwise
 */
bool Analysis::State::invalidateMem(const OperandMem& addr)
{
	//return invalidate(addr); // TODO!! why was this here?
	Path labels;
	if(Option<OperandConst> maybe_val = findConstantValueOfMemCell(addr, labels))
		return replaceMem(addr, *maybe_val, labels); // try to keep the info
	bool rtn = false;
	for(PredIterator piter(*this); piter; )
	{
		if(piter.pred().involvesMemoryCell(addr))
		{
			DBG(color::IPur() << DBG_SEPARATOR << color::IYel() << " - " << *piter)
			removePredicate(piter);
			rtn = true;
		}
		else piter++;
	}
	return rtn;
}

// This function will try to keep the information contained in the tempvars 
// by replacing them by their values in other predicates before removing them
bool Analysis::State::invalidateTempVars()
{
	bool loop, rtn = false;

	rtn |= constants.invalidateTempVars();

	// First step: try and replace everything we can
	do {
		loop = false;
		for(SLList<LabelledPredicate>::Iterator iter(generated_preds); iter; iter++)
		{
			if(iter->pred().countTempVars())
			{
				OperandVar temp_var(0);
				Operand* expr = NULL;
				if(iter->pred().getIsolatedTempVar(temp_var, expr)) // returns false if several tempvars
				{
					// try to keep the info
					rtn |= replaceTempVar(temp_var, *expr);
					delete expr;
					// then remove the predicate
					DBG(color::IYel() << "- " << iter->pred())
					generated_preds.remove(iter);
					loop = true;
					break;
				}
				else if(expr)
					delete expr;
			}
		}
	} while(loop);
	
	// Second step: remove everything that is left and still contains a tempvar
	for(SLList<LabelledPredicate>::Iterator iter(generated_preds); iter; )
	{
		if(iter->pred().countTempVars())
		{	// remove the predicate
			DBG(color::IYel() << "- " << iter->pred())
			generated_preds.remove(iter);
			continue;
		}
		iter++;
	}
	return rtn;
}

int Analysis::State::invalidateStackBelow(const Constant& stack_limit)
{
	int count = 0;
	ASSERT(stack_limit.isRelative());
	// do not try to replace everything unlike invalidateTempVars - the point is to get rid of useless obsolete data
	// wait what... no we sometimes have A = B and B >= 5, gotta keep A >= 5 if B is irrelevant
	for(PredIterator piter(*this); piter; piter++) //)
	{
		if(piter.pred().involvesStackBelow(stack_limit))
		// while(const Option<Constant>& addr_involved = piter.pred().involvesStackBelow(stack_limit))
		{
			removePredicate(piter);
			count++;
			continue; }
			/*
			// const Predicate* removed_predicate = NULL;
			bool replaced_predicates = false;
			const OperandMem opd_to_remove(OperandConst(addr_involved));

			// reparse the predicates to look for some way to keep this damned opd_to_remove operand
			for(PredIterator ppiter(*this); ppiter; ppiter++)
			{
				const Predicate &p = ppiter.pred();
				if(p.opr() == CONDOPR_EQ)
				{
					const int left_involves_opd = p.leftOperand().involvesOperand(opd_to_remove);
					const int right_involves_opd = p.rightOperand().involvesOperand(opd_to_remove);
					// This could really be improved with some basic arithmetic
					if(left_involves_opd == 1 && right_involves_opd == 0) // var is in left operand
					{
						if(p.leftOperand() == opd_to_remove) // left operand is exactly var (that is, predicate is "var = ???")
						{
							Operand* expr = p.rightOperand().copy(); // backup the "???" expr
							removePredicate(ppiter); // remove current predicate
							replaceVar(opd_to_remove, *expr); // [??? / var]
							DBG(color::IPur() << DBG_SEPARATOR << color::IYel() << " - " << Predicate(CONDOPR_EQ, var, *expr))
							delete expr;
							replaced_predicates = true;
							break;
						}
					}
					else if(left_involves_opd == 0 && right_involves_opd == 1) // var is in right operand
					{
						if(p.rightOperand() == opd_to_remove) // right operand is exactly var (that is, predicate is "??? = var")
						{
							Operand* expr = p.leftOperand().copy(); // backup the "???" expr
							removePredicate(ppiter); // remove current predicate
							replaceVar(opd_to_remove, *expr); // [??? / var]
							DBG(color::IPur() << DBG_SEPARATOR << color::IYel() << " - " << Predicate(CONDOPR_EQ, *expr, var))
							delete expr;
							replaced_predicates = true;
							break;
						}
					}
				}
			}

			if(replaced_predicates)
			{
				count++;
				qskldjsjklfjkdsfjksdlfs;
				// recursive call... what's the use of the while actually? cause we could have removed the current predicate, fuck
			}
		}
		if(should_iterate)
			piter++;*/
	}
	DBG(color::IYel() << "- " << count << " predicates")
	return count;
}

// this mindlessly replaces all occurrences of var by expr
bool Analysis::State::replaceVar(const OperandVar& var, const Operand& expr)
{
	bool rtn = false;
	for(PredIterator piter(*this); piter; )
	{
		if(piter.pred().involvesVariable(var))
		{
			Predicate p = piter.pred();
			String prev_str = _ << piter.pred();
			if(p.update(var, expr)) // something updated
			{
				if(rtn == false) // first time
				{
					DBG(color::IBlu() << "[" << expr << " / " << var << "]")
					rtn = true;
				}
				DBG(color::IBlu() << DBG_SEPARATOR " " << color::Cya()  << "- " << prev_str)
				DBG(color::IBlu() << DBG_SEPARATOR " " << color::ICya() << "+ " << p)
				setPredicate(piter, LabelledPredicate(p, piter.labels()));
				movePredicateToGenerated(piter); // this does piter++
				continue;
			}
		}
		piter++;
	}
	return rtn;
}

// this mindlessly replaces all occurrences of var by expr
bool Analysis::State::replaceOperand(const Operand& opd, const Operand& expr)
{
	bool rtn = false;
	for(PredIterator piter(*this); piter; )
	{
		if(piter.pred().involvesOperand(opd))
		{
			Predicate p = piter.pred();
			String prev_str = _ << piter.pred();
			if(p.update(opd, expr)) // something updated
			{
				if(rtn == false) // first time
				{
					DBG(color::IBlu() << "[" << expr << " / " << opd << "]")
					rtn = true;
				}
				DBG(color::IBlu() << DBG_SEPARATOR " " << color::Cya()  << "- " << prev_str)
				DBG(color::IBlu() << DBG_SEPARATOR " " << color::ICya() << "+ " << p)
				setPredicate(piter, LabelledPredicate(p, piter.labels()));
				movePredicateToGenerated(piter); // this does piter++
				continue;
			}
		}
		piter++;
	}
	return rtn;
}

bool Analysis::State::replaceTempVar(const OperandVar& temp_var, const Operand& expr)
{
	bool rtn = false;
	for(SLList<LabelledPredicate>::Iterator iter(generated_preds); iter; iter++)
	{
		if(iter->pred().involvesVariable(temp_var))
		{
			Predicate p(iter->pred());
			String prev_str = _ << p;			
			p.update(temp_var, expr);
			if(!p.isIdent()) // if we just transformed t1 = X into X = X, don't add to generated_preds
			{
				if(rtn == false) // first time
				{
					DBG(color::IBlu() << "[" << expr << " / " << temp_var << "]")
					rtn = true;
				}
				DBG(color::IBlu() << DBG_SEPARATOR " " << color::Cya()  << "- " << prev_str)
				DBG(color::IBlu() << DBG_SEPARATOR " " << color::ICya() << "+ " << p)
				generated_preds.set(iter, LabelledPredicate(p, iter->labels()));
			}
		}
	}
	return rtn;
}

// replaces all occurrences of opdm in all predicates by expr + append "labels"
bool Analysis::State::replaceMem(const OperandMem& opdm, const Operand& expr, const Path& labels)
{
	bool rtn = false;
	for(SLList<LabelledPredicate>::MutableIterator iter(generated_preds); iter; )
	{
		const elm::String prev_str = _ << iter.item().pred();
		if(iter.item().updatePred(opdm, expr))
		{
			iter.item().addLabels(labels);
			if(rtn == false) // first time
			{
				DBG(color::IBlu() << "[" << expr << " / " << opdm << "]")
				rtn = true;
			}
			rtn = true;
			DBG(color::IBlu() << DBG_SEPARATOR " " << color::Cya()  << "- " << prev_str)
			if(iter.item().pred().isIdent())
			{
				DBG(color::IBlu() << DBG_SEPARATOR " " << color::ICya() << "  (not adding identity)")				
				generated_preds.remove(iter);
			}
			else 
			{
				DBG(color::IBlu() << DBG_SEPARATOR " " << color::ICya() << "+ " << iter.item())
				iter++;
			}
		}
		else iter++;
	}
	for(SLList<LabelledPredicate>::MutableIterator iter(labelled_preds); iter; )
	{
		const elm::String prev_str = _ << iter.item();
		if(iter.item().updatePred(opdm, expr))
		{
			iter.item().addLabels(labels);
			if(rtn == false) // first time
			{
				DBG(color::IBlu() << "[" << expr << " / " << opdm << "]")
				rtn = true;
			}
			rtn = true;
			if(!iter.item().pred().isIdent())
			{
				DBG(color::IBlu() << DBG_SEPARATOR " " << color::Cya()  << "- " << prev_str)
				DBG(color::IBlu() << DBG_SEPARATOR " " << color::ICya() << "+ " << iter.item())
				generated_preds += iter.item(); // move to generated preds
			}
			labelled_preds.remove(iter);
		}
		else iter++;
	}
	return rtn;
}

// returns true if a variable has been updated
// second operand is usually an OperandArithExpr
bool Analysis::State::update(const OperandVar& opd_to_update, const Operand& opd_modifier, Path& labels)
{
	Operand *opd_modifier_new, *opd_modifier_prenew = opd_modifier.copy();
	// for example instead of doing [?13+t1/?13], do [?13+4/?13]
	Vector<OperandVar> replaced_vars;
	if(Option<Operand*> maybe_opd_modifier_new = opd_modifier_prenew->replaceConstants(constants.toSimplified(), replaced_vars)) // replace constants
	{
		delete opd_modifier_prenew;
		opd_modifier_new = *maybe_opd_modifier_new;
	}
	else
		opd_modifier_new = opd_modifier_prenew;
	updateLabelsWithReplacedConstantsInfo(labels, replaced_vars);

	// amongst other things this can simplify constant arithopr such as 8+(4-2)
	if(Option<Operand*> opd_modifier_new_simplified = opd_modifier_new->simplify())
	{
		DBG(color::Pur() << DBG_SEPARATOR << color::Blu() << " Simplified " << *opd_modifier_new << " to " << **opd_modifier_new_simplified)
		delete opd_modifier_new;
		opd_modifier_new = *opd_modifier_new_simplified;
	}

#	ifdef NO_UTF8
		DBG(color::IPur() << DBG_SEPARATOR << color::Blu() << " [" << opd_to_update << " -> " << *opd_modifier_new << "]")
#	else
		DBG(color::IPur() << DBG_SEPARATOR << color::Blu() << " [" << opd_to_update << " → " << *opd_modifier_new << "]")
#	endif
	bool rtn = false;
	
	for(PredIterator piter(*this); piter; piter++)
	{
		if(piter.pred().involvesVariable(opd_to_update))
		{
			DBG(color::IPur() << DBG_SEPARATOR << color::Blu() << " " DBG_SEPARATOR << color::Cya() << " - " << *piter)
			
			// updating the predicate
			Predicate p = piter.pred();
			bool update_state = p.update(opd_to_update, *opd_modifier_new);
			ASSERTP(update_state, "nothing was updated even though involvesVariable() returned true");

			LabelledPredicate lp = LabelledPredicate(p, piter.labels());
			setPredicate(piter, lp);
			DBG(color::IPur() << DBG_SEPARATOR << color::Blu() << " " DBG_SEPARATOR << color::ICya() << " + " << *piter)
			movePredicateToGenerated(piter);
			rtn = true;
		}
	}
	delete opd_modifier_new;
	return rtn;
}

/**
 * @fn bool Analysis::State::findConstantValueOfVar(const OperandVar& var, OperandConst& val);
 * For a variable tX or ?X, try to find a predicate tX=cst or ?X=cst, and return cst when successful.
 * If the variable is a tempvar (tX), does not browse predicates that have not been generated in the current BB
 * @return true if a value was found
 */
// TODO: add a Path& labels parameter
Option<OperandConst> Analysis::State::findConstantValueOfVar(const OperandVar& var)
{
	if(!isConstant(var))
		return none;
	return some(OperandConst(constants[var]));
}

// TODO: maybe we should improve this to handle affine equations ("[SP-12] -1 = 0" etc...)
Option<OperandConst> Analysis::State::findConstantValueOfMemCell(const OperandMem& mem, Path &labels)
{
	for(PredIterator piter(*this); piter; piter++)
	{
		const Predicate& p = piter.pred();
		if(p.opr() != CONDOPR_EQ)
			continue;
		if(p.leftOperand() == mem)
		{
			if(Option<OperandConst> maybe_val = p.rightOperand().evalConstantOperand())
			{
				labels += piter.labels();
				return *maybe_val;
			}
		}
		else if(p.rightOperand() == mem)
		{
			if(Option<OperandConst> maybe_val = p.leftOperand().evalConstantOperand())
			{
				labels += piter.labels();
				return *maybe_val;
			}
		}
	}
	return elm::none;
}

/**
 * @fn bool Analysis::State::findStackRelativeValueOfVar(const OperandVar& var, OperandConst& val);
 * For a variable tX or ?X, try to find a predicate tX=sp+cst or ?X=sp+cst, and return cst when successful.
 * @return true if a value was found
 */
Option<t::int32> Analysis::State::findStackRelativeValueOfVar(const OperandVar& var, Path &labels)
{
	for(PredIterator piter(*this); piter; piter++)
	{
		if(piter.pred().opr() != CONDOPR_EQ)
			continue;
		// Algorithm 3 (affine): ((?var +- ...) +- ... = (...)
		if(piter.pred().isAffine(var)) // try and look for an affine case ((.. + ..)-..) = (..-..)
		{
			AffineEquationState eqstate;
			piter.pred().leftOperand().parseAffineEquation(eqstate);
			eqstate.reverseSign();
			piter.pred().rightOperand().parseAffineEquation(eqstate);
			if(eqstate.spCounter() == +1 && eqstate.varCounter() == -1) // var = sp + delta
			{
				labels += piter.labels();
				return some(eqstate.delta());
			}
			else if(eqstate.spCounter() == -1 && eqstate.varCounter() == +1) // sp = var + delta
			{
				labels += piter.labels();
				return some(-eqstate.delta()); // var = sp + (-delta)
			}
			// else algorithm failed (for example var = -sp or var = sp+sp)
		}
	}
	DBG(color::IRed() << "findStackRelativeValueOfVar failed! (store/load to T)")
	// DBG(dumpEverything())
	return none; // no matches found
}
/**
 * @fn bool Analysis::State::findValueOfCompVar(const OperandVar& var, Operand*& opd_left, Operand*& opd_right);
 * For a register variable ?X, try to find a predicate ?X = (opd_left ~ opd_right), and return false if it cannot find any
 * @return true if a value was found
 */
bool Analysis::State::findValueOfCompVar(const OperandVar& var, Operand*& opd_left, Operand*& opd_right, Path& labels)
{
	// We only explore the local generated_preds. This seems reasonable since we should be able to find this predicate in the local BB
	// BUT this does not exempt us from looking for labels because these predicates can be themselves associated to predicates of previous BBs
	for(SLList<LabelledPredicate>::Iterator iter(generated_preds); iter; iter++)
	{
		const Predicate& p = iter->pred();
		if(p.opr() == CONDOPR_EQ) // operator is =
		{
			if(p.leftOperand() == var) // left operand matches our register
			{
				if(p.rightOperand().kind() == OPERAND_ARITHEXPR)
				{
					const OperandArithExpr& opd = (const OperandArithExpr&)p.rightOperand();
					if(opd.opr() == ARITHOPR_CMP) // other operand has to be ??? ~ ???
					{
						opd_left = opd.leftOperand().copy();
						opd_right = opd.rightOperand().copy();
						labels += iter->labels();
						return true;
					}
				}
			}
			if(p.rightOperand() == var) // right operand matches our register
			{
				if(p.leftOperand().kind() == OPERAND_ARITHEXPR)
				{
					const OperandArithExpr& opd = (OperandArithExpr&)p.leftOperand();
					if(opd.opr() == ARITHOPR_CMP) // other operand has to be ??? ~ ???
					{
						opd_left = opd.leftOperand().copy();
						opd_right = opd.rightOperand().copy();
						labels += iter->labels();
						return true;
					}
				}
			}
		}
	}
	return false; // No matches found
}

Option<OperandMem> Analysis::State::getOperandMem(const OperandVar& var, Path& labels)
{
	if(Option<OperandConst> val = findConstantValueOfVar(var))
		return some(OperandMem(*val));
	if(Option<t::int32> val = findStackRelativeValueOfVar(var, labels))
		return some(OperandMem(OperandConst(SP+*val)));
		//return some(OperandMem(OperandConst(*val), true));
	return none;
}

bool Analysis::State::invalidateAllMemory()
{
	bool rtn = false;
	for(PredIterator piter(*this); piter; )
	{
		if(piter.pred().involvesMemory())
		{
			DBG(color::IPur() << DBG_SEPARATOR << color::IYel() << " - " << *piter)
			removePredicate(piter);
			rtn = true;
			continue;
		}
		piter++;
	}
	return rtn;
}

/*
 * Operand::replaceConstants returns a Vector of replaced constant variables,
 * then we have to process ConstantVariables constants to find the matching labels and add them to the labels list
 */
void Analysis::State::updateLabelsWithReplacedConstantsInfo(Path& labels, const Vector<OperandVar>& replaced_vars) const
{
	for(Vector<OperandVar>::Iterator iter(replaced_vars); iter; iter++)
	// {
		labels += constants.getLabels(*iter);
		// DBG(color::IRed() << "replaced_vars:" << *iter << ", labels+=" << pathToString(constants.getLabels(*iter)))
	// }
}

Predicate* Analysis::State::getPredicateGeneratedByCondition(sem::inst condition, bool taken, Path& labels)
{
	cond_t kind = condition.cond();
	t::int16 sr = condition.sr(); // ARM's r16
	Operand *opd_left = NULL, *opd_right = NULL;
	if(!findValueOfCompVar(OperandVar(sr), opd_left, opd_right, labels))
		return NULL;
	
	condoperator_t opr;
	bool reverse = false;
	if(!taken)
		kind = invert(kind); // kind = !kind
	switch(kind) // TODO: handle unsigned better
	{
		case EQ:
			opr = CONDOPR_EQ;
			break;
		case ULT:
		case LT:
			opr = CONDOPR_LT;
			break;
		case ULE:
		case LE:
			opr = CONDOPR_LE;
			break;
		case UGE:
		case GE:
			opr = CONDOPR_LE;
			reverse = true;
			break;
		case UGT:
		case GT:
			opr = CONDOPR_LT;
			reverse = true;
			break;
		case NE:
			opr = CONDOPR_NE;
			break;
		default:
			return NULL; // invalid condition, exit
	}
	Predicate *rtn = new Predicate(opr, reverse ? *opd_right : *opd_left, reverse ? *opd_left : *opd_right);
	delete opd_left;
	delete opd_right;
	return rtn;
}

/*
	Check if addr_mem is the constant address of some read-only data, if so returns the constant data value
*/
Option<OperandConst> Analysis::State::getConstantValueOfReadOnlyMemCell(const OperandMem& addr_mem, otawa::sem::type_t type)
{
	const Constant& addr = addr_mem.getConst().value();
	if(!addr.isAbsolute())
		return none;
	if(!dfa_state->isInitialized(addr.val()))
		return none;
	switch(type)
	{
		case INT8:
		case INT16:
		case INT32:
		case INT64: // TODO: can't we just do the same as UINT and (int) convert it?
		return none;
		case UINT8:
		{
			t::uint8 v;
			dfa_state->get(addr.val(), v);
			return elm::some(OperandConst(v));
		}
		case UINT16:
		{
			t::uint16 v;
			dfa_state->get(addr.val(), v);
			return elm::some(OperandConst(v));
		}
		case UINT32:
		{
			t::uint32 v;
			dfa_state->get(addr.val(), v);
			return elm::some(OperandConst(v));
		}
		case UINT64:
		{
			t::uint64 v;
			dfa_state->get(addr.val(), v);
			return elm::some(OperandConst(v));
		}
		case FLOAT32:
		{
			float v;
			dfa_state->get(addr.val(), v);
			return elm::some(OperandConst(v));
		}
		case FLOAT64:
		{
			double v;
			dfa_state->get(addr.val(), v);
			return elm::some(OperandConst(v));
		}
		case MAX_TYPE:
		case NO_TYPE:
		return none;
	}
	return none;
}
