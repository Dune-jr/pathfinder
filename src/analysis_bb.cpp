#include <otawa/cfg/BasicBlock.h>
#include <otawa/sem/inst.h>
#include <otawa/sem/PathIter.h>
#include <elm/string/String.h>
#include "predicate.h"
#include "operand.h"
#include "debug.h"
#include "analysis.h"

using namespace otawa::sem;

void Analysis::State::processBB(const BasicBlock *bb)
{
	SLList<LabelledPredicate> generated_preds_before_condition;
	sem::inst condition;
	generated_preds.clear();
	generated_preds_taken.clear();
	
	// parse assembly instructions
	for(BasicBlock::InstIterator insts(bb); insts; insts++)
	{
		DBG(color::Pur() << *insts)
		Block block;
		insts->semInsts(block);
		
		PathIter seminsts;
		// parse semantical instructions with PathIter
		for(seminsts.start(*insts); seminsts; seminsts++)
		{
			DBG(color::IPur() << *seminsts)
			
			if(seminsts.isCond()) // IF
			{	// backup the list of generated predicates before entering the condition
				generated_preds_before_condition = generated_preds; // side effect: reverses the order of the list
				DBG(color::IBlu() << "(Parsing taken path)")
			}
			if(seminsts.pathEnd()) // CONT
			{ 	// dumping the current generated_preds into the previous_paths_preds list
				invalidateTempVars(); // do the end-of-instruction tempvar invalidation first
				DBG(color::IBlu() << "(Parsing not taken path)")
				generated_preds_taken = generated_preds;
				generated_preds = generated_preds_before_condition;
				//previous_paths_preds += generated_preds;
			}
			
			Operand *opd1 = NULL, *opd2 = NULL;
			condoperator_t opr = CONDOPR_EQ; // Default is =
			Path labels; // empty by default
			bool make_pred = false;
			
			// some shortcuts (the seminsts.F functions are not called at this point)
			const t::int16& a = seminsts.a(), b = seminsts.b(), d = seminsts.d();
			const t::int32& cst = seminsts.cst();
			const t::int16& reg = seminsts.reg(), addr = seminsts.addr();
			
			switch(seminsts.op())
			{
				case NOP:
					ASSERT(!UNTESTED_CRITICAL);
					DBG(color::BIRed() << "Untested operand NOP running!")
					break;
				case BRANCH:
					break;
				case CONT:
					{
						Predicate *p;
						if(!(p = getPredicateGeneratedByCondition(condition, false)))
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
						if(!(p = getPredicateGeneratedByCondition(condition, true)))
							break; // in view of this, we cannot generate a predicate 
						make_pred = true;
						opr = p->opr();
						opd1 = p->leftOperand().copy();
						opd2 = p->rightOperand().copy();
						delete p;
					}
					break;
				case LOAD: // reg <- MEM_type(addr)
					invalidateVar(reg);
					if(Option<OperandMem> addr_mem = getOperandMem(addr, labels))
					{
						// if it's a constant address of some read-only data
						if(Option<OperandConst> addr_const_value = getConstantDataValue(*addr_mem, (*seminsts).type()))
						{
							DBG(color::IPur() << DBG_SEPARATOR " " << color::IBlu() << "Memory data " << *addr_mem << " simplified to " << *addr_const_value)
							constants.set(d, (*addr_const_value).value());
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
				case SCRATCH:
					invalidateVar(d);
					break;
				case SET:
					invalidateVar(d);
					opd1 = new OperandVar(d);
					opd2 = new OperandVar(a);
					if(isConstant(a)) // if a is already identified as a constant
						constants.set(d, constants[a]); // then constants[d] = constants[a]
					else // no use generating this predicate if it is a constant, because the ConstantVariables object handles that
						make_pred = true; // d = already
					break;
				case SETI:
					invalidateVar(d);
					// everything should already be handled by the ConstantVariables object
					constants.set(d, cst); // constants[d] = cst
					break;
				case SETP:
					ASSERT(!UNTESTED_CRITICAL);
					DBG(color::BIRed() << "Unimplemented operand SETP running!")
					invalidateVar(d);
					break;
				case CMP:
				case CMPU:
					make_pred = true;
					opd1 = new OperandVar(d);
					{
						Operand *opd21 = new OperandVar(a);
						Operand *opd22 = new OperandVar(b);
						opd2 = new OperandArithExpr(ARITHOPR_CMP, *opd21, *opd22);
					}
					invalidateVar(d);
					break;
				case ADD:
					if(d == a)
					{
						if(d == b) // d <- d+d
						{	// [d/2 / d]
							update(OperandVar(d), OperandArithExpr(ARITHOPR_DIV, OperandVar(d), OperandConst(2)));
							Operand *opd11 = new OperandVar(d);
							Operand *opd12 = new OperandVar(2);
							opd1 = new OperandArithExpr(ARITHOPR_MOD, *opd11, *opd12);
							opd2 = new OperandConst(0);
							make_pred = true; // d % 2 = 0
							if(isConstant(d))
								constants.set(d, constants[d]*2);
						}
						else // d <- d+b
						{	// [d-b / d]
							update(OperandVar(d), OperandArithExpr(ARITHOPR_SUB, OperandVar(d), OperandVar(b)));
							if(isConstant(d))
							{
								if(isConstant(b))
									constants.set(d, constants[d]+constants[b]);
								else
								{
									// d = constants[d] + b
									opd1 = new OperandVar(d);
									Operand *opd21 = new OperandConst(constants[d]);
									Operand *opd22 = new OperandVar(b);
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
							update(OperandVar(d), OperandArithExpr(ARITHOPR_SUB, OperandVar(d), OperandVar(a)));
							if(isConstant(d))
							{
								if(isConstant(a))
									constants.set(d, constants[d]+constants[a]);
								else
								{
									// d = a + constants[d]
									opd1 = new OperandVar(d);
									Operand *opd21 = new OperandVar(a);
									Operand *opd22 = new OperandConst(constants[d]);
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
							Operand *opd21 = new OperandVar(a);
							Operand *opd22 = new OperandVar(b);
							opd2 = new OperandArithExpr(ARITHOPR_ADD, *opd21, *opd22);
							if(isConstant(a) && isConstant(b))
								constants.set(d, constants[a]+constants[b]);
							else make_pred = true; // d = a+b
							// the invalidation in constants is already handled by invalidateVar
						}
					}
					break;
				case SUB:
					opd1 = new OperandVar(d);
					{
						if(d == a)
						{
							if(d == b) // d <- d-d
							{	// [0 / d], d is set to 0!
								invalidateVar(d);
								constants.set(d, 0);
							}
							else // d <- d-b
							{	// [d+b / d]
								update(OperandVar(d), OperandArithExpr(ARITHOPR_ADD, OperandVar(d), OperandVar(b)));
								if(isConstant(d))
								{
									if(isConstant(b))
										constants.set(d, constants[d]-constants[b]);
									else
									{
										// d = constants[d] - b
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
								update(OperandVar(d), OperandArithExpr(ARITHOPR_SUB, OperandVar(a), OperandVar(d)));
								if(isConstant(d))
								{
									if(isConstant(a)) // everything is const, update const value of d
										constants.set(d, constants[a]-constants[d]);
									else
									{
										// d = a - constants[d]
										// opd1 = new OperandVar(d);
										Operand *opd21 = new OperandVar(a);
										Operand *opd22 = new OperandConst(constants[d]);
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
								Operand *opd21 = new OperandVar(a);
								Operand *opd22 = new OperandVar(b);
								opd2 = new OperandArithExpr(ARITHOPR_SUB, *opd21, *opd22);
								if(isConstant(a) && isConstant(b))
									constants.set(d, constants[a]-constants[b]);
								else make_pred = true; // d = a-b
								// the invalidation in constants is already handled by invalidateVar
							}
						}
					}
					break;
				case SHL:
					opd1 = new OperandVar(d);
					{
						/*
						if(d == b) // safety test, shouldn't happen unless assembly language allows variable bit shifting
						{	// well actually ldr r3, [r3, r2, lsl #2] makes it happen in ARM...
							invalidateVar(d);
							break;
						}
						*/

						// b is usually a tempvar that has been previously set to a constant value
						if(!isConstant(b))
						{
							DBG(color::Blu() << "  [" << OperandVar(b) << " could not be identified to a constant value]")
							invalidateVar(d); // only invalidate now (otherwise we can't find t1 for shl t1, ?0, t1)
							break;
						}
						t::int32 b_val = constants[b].val();
						DBG(color::Blu() << "  [" << OperandVar(b) << " identified as 0x" << io::hex(b_val) << "]")
						if(d == a) // d <- d << b
						{
							ASSERT(!UNTESTED_CRITICAL);
							DBG(color::BIRed() << "Untested case of operator SHL running!")
							update(OperandVar(d), OperandArithExpr(ARITHOPR_DIV, OperandVar(d), OperandConst(1 << b_val)));
							// we also add a predicate to say that d is now a multiple of 2^b
							Operand *opd11 = new OperandVar(d);
							Operand *opd12 = new OperandConst(1 << b_val);
							opd1 = new OperandArithExpr(ARITHOPR_MOD, *opd11, *opd12);
							opd2 = new OperandConst(0);
							if(isConstant(d)) // we must update constant value of d
								constants.set(d, constants[d] << b_val);
							else make_pred = true;
						}
						else // d <- a << b
						{
							invalidateVar(d);
							if(isConstant(a))
								constants.set(d, constants[a] << b_val);
							else
							{
								Operand *opd21 = new OperandVar(a);
								Operand *opd22 = new OperandConst(1 << b_val); // 2^b_val
								opd2 = new OperandArithExpr(ARITHOPR_MUL, *opd21, *opd22);
								make_pred = true;
							}
						}
					}
					break;
				case ASR: // TODO test: is this really legit?
					ASSERT(!UNTESTED_CRITICAL);
					DBG(color::BIRed() << "Untested operator running!")
				case SHR:
					opd1 = new OperandVar(d);
					{
						// b is usually a tempvar that has been previously set to a constant value
						if(!isConstant(b))
						{
							DBG(color::Blu() << "  [" << OperandVar(b) << " could not be identified as a constant value]")
							invalidateVar(d);
							break;
						}
						t::int32 b_val = constants[b].val();
						DBG(color::Blu() << "  [" << OperandVar(b) << " identified as 0x" << io::hex(b_val) << "]")
						invalidateVar(d);
						if(d == a) // d <- d >> b
							break; // not much we can do because we lost info (cf (*))
						// d <- a >> b
						Operand *opd21 = new OperandVar(a);
						Operand *opd22 = new OperandConst(1 << b_val); // 2^b_val
						opd2 = new OperandArithExpr(ARITHOPR_DIV, *opd21, *opd22);
						if(isConstant(a))
							constants.set(d, constants[a] >> constants[b]);
						else make_pred = true;
					}
					break;
				case NEG: // TODO test
					ASSERT(!UNTESTED_CRITICAL);
					DBG(color::BIRed() << "Untested operator running!")
					{
						if(a == d) // d <- -d
						{	// [-1 * d / d]
							// update(OperandVar(d), OperandArithExpr(ARITHOPR_MUL, OperandConst(-1), OperandVar(d)));
							update(OperandVar(d), OperandArithExpr(ARITHOPR_NEG, OperandVar(d))); // TODO test
							if(isConstant(d))
								constants.set(d, -constants[d]);
						}
						else
						{	
							invalidateVar(d);
							opd1 = new OperandVar(d);
							Operand *opd21 = new OperandConst(-1);
							Operand *opd22 = new OperandVar(a);
							opd2 = new OperandArithExpr(ARITHOPR_MUL, *opd21, *opd22);
							if(isConstant(a))
								constants.set(d, -constants[a]);
							else make_pred = true;
						}
					}
					break;
				case NOT:		// d <- ~a
				case AND:		// d <- a & b
				case OR:		// d <- a | b
				case XOR:		// d <- a ^ b
					ASSERT(!UNTESTED_CRITICAL);
					DBG(color::BIRed() << "Unimplemented operator NOT/AND/OR/XOR running!")
					dumpPredicates();
					invalidateVar(d);
					break;
				case MULU:
					ASSERT(!UNTESTED_CRITICAL);
					DBG(color::BIRed() << "Untested unsigned variant running!")
				case MUL:
					make_pred = true;
					opd1 = new OperandVar(d);
					{
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
								if(isConstant(d))
									constants.set(d, constants[d]*constants[d]);
								else
									constants.invalidate(d);
							}
							else // d <- d*b
							{	// [d/b / d] // we will have to assume that 0/0 is scratch!
								ASSERT(!UNTESTED_CRITICAL);
								DBG(color::BIRed() << "Untested case of operator MUL running!")
								update(OperandVar(d), OperandArithExpr(ARITHOPR_DIV, OperandVar(d), OperandVar(b)));
								if(isConstant(d))
								{
									if(isConstant(b))
										constants.set(d, constants[d]*constants[b]);
									else
										constants.invalidate(d);
								}
								
								// we also add a predicate to say that d is now a multiple of b
								Operand *opd11 = new OperandVar(d);
								Operand *opd12 = new OperandVar(b);
								opd1 = new OperandArithExpr(ARITHOPR_MOD, *opd11, *opd12); // d % b (%0 is to consider!)
								opd2 = new OperandConst(0);
							}
						}
						else
						{
							if(d == b) // d <- a*d
							{	// [d/a / d] // we will have to assume that 0/0 is scratch!
								ASSERT(!UNTESTED_CRITICAL);
								DBG(color::BIRed() << "Untested case of operator MUL running!")
								update(OperandVar(d), OperandArithExpr(ARITHOPR_DIV, OperandVar(d), OperandVar(a)));
								if(isConstant(d))
								{
									if(isConstant(a))
										constants.set(d, constants[d]*constants[a]);
									else
										constants.invalidate(d);
								}
								
								// we also add a predicate to say that d is now a multiple of a
								Operand *opd11 = new OperandVar(d);
								Operand *opd12 = new OperandVar(a);
								opd1 = new OperandArithExpr(ARITHOPR_MOD, *opd11, *opd12); // d % a (%0 is to consider!)
								opd2 = new OperandConst(0);
							}
							else // d <- a*b
							{
								opd1 = new OperandVar(d);
								invalidateVar(d);
								Operand *opd21 = new OperandVar(a);
								Operand *opd22 = new OperandVar(b);
								opd2 = new OperandArithExpr(ARITHOPR_MUL, *opd21, *opd22);
								if(isConstant(a) && isConstant(b))
									constants.set(d, constants[a]*constants[b]);
							}
						}
						break;
					}
					break;
				case DIVU:
					ASSERT(!UNTESTED_CRITICAL);
					DBG(color::BIRed() << "Untested unsigned variant running!")
				case DIV:
					ASSERT(!UNTESTED_CRITICAL);
					DBG(color::BIRed() << "Untested operator running!")
					if(d == a)
					{
						if(d == b) // d <- d / d
						{	// [1 / d]
							// TODO: this should be okay to assume d != 0, right? otherwise the program is faulty?
							// or should we use a SMT solver mode that supports / 0?
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
									constants.set(d, constants[d]/constants[b]);
								else constants.invalidate(d);
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
									constants.set(d, constants[a]/constants[d]);
								else constants.invalidate(d);
							}
						}
						else // d <- a / b
						{
							invalidateVar(d);
							opd1 = new OperandVar(d);
							Operand *opd21 = new OperandVar(a);
							Operand *opd22 = new OperandVar(b);
							opd2 = new OperandArithExpr(ARITHOPR_DIV, *opd21, *opd22);
							if(isConstant(a) && isConstant(b))
								constants.set(d, constants[a]/constants[b]);
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
						Operand *opd21 = new OperandVar(a);
						Operand *opd22 = new OperandVar(b);
						opd2 = new OperandArithExpr(ARITHOPR_MOD, *opd21, *opd22);
					}
					if(isConstant(a) && isConstant(b))
						constants.set(d, constants[a]%constants[b]);
					else make_pred = true;
					break;
				case SPEC: // special instruction (d: code, cst: sub-code)
					invalidateVar(d);
					break;
				default:
					make_pred = false;
			}
			if(make_pred)
			{
				ASSERT(opd1);
				ASSERT(opd2);
				// If we have predicates such as ?16 = ?4 ~ t1, make sure none of these are identified as constants in the constantVariables table!
				if(Option<Operand*> maybe_opd1 = opd1->replaceConstants(constants.toSimplified()))
				{
					delete opd1;
					opd1 = *maybe_opd1;
				}
				if(Option<Operand*> maybe_opd2 = opd2->replaceConstants(constants.toSimplified()))
				{
					delete opd2;
					opd2 = *maybe_opd2;
				}
				DBG(color::IPur() << DBG_SEPARATOR << color::IGre() << " + " << Predicate(opr, *opd1, *opd2))
				generated_preds += LabelledPredicate(Predicate(opr, *opd1, *opd2), labels);
			}
			if(opd1) delete opd1;
			if(opd2) delete opd2;
		}
		// all temporary variables are freed at the end of any assembly instruction, so invalidate them
		invalidateTempVars();
	}
	
	if(generated_preds_taken)
	{
		DBG("Predicates generated: ")
		DBG("|-> taken path: " << generated_preds_taken)
		DBG("|-> not taken path: " << generated_preds)
	}
	else
		DBG("Predicates generated: " << generated_preds)
}

// returns true if a variable has been invalidated
bool Analysis::State::invalidateVar(const OperandVar& var, bool invalidate_constant_info)
{
	bool rtn = false;

	// try and identify a value for ?3 (look for a ?3 = X predicate)
	for(PredIterator piter(*this); piter; piter++)
	{
		if(piter.pred().opr() == CONDOPR_EQ && piter.pred().involvesVariable(var) == 1)
		{
			// this requires generated_preds to be LabelledPredicates!
			if(piter.pred().leftOperand().involvesVariable(var) == 1) // var is in left operand
			{
				if(piter.pred().leftOperand() == var) // left operand is exactly var (that is, predicate is "var = ???")
				{
					Operand* expr = piter.pred().rightOperand().copy(); // backup the "???" expr
					removePredicate(piter); // remove current predicate
					replaceVar(var, *expr); // [??? / var]
					delete expr;
					break; // stop and move on
				}
			}
			else // var is in right operand
				if(piter.pred().rightOperand() == var) // right operand is exactly var (that is, predicate is "??? = var")
				{
					Operand* expr = piter.pred().leftOperand().copy(); // backup the "???" expr
					removePredicate(piter); // remove current predicate
					replaceVar(var, *expr); // [??? / var]
					delete expr;
					break; // stop and move on
				}
		}
	}

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

	if(invalidate_constant_info)
		constants.invalidate(var);
	return rtn;
}

/*
	This tries to find a matching OperandMem for the provided OperandVar, then calls invalidateMem(OperandMem)
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

/*
	Replace every occurence of the OperandMem parameter by a constant value if possible, removes the occurences from the predicate lists otherwise
*/
bool Analysis::State::invalidateMem(const OperandMem& addr)
{
	Path labels;
	if(Option<Constant> maybe_val = findConstantValueOfMemCell(addr, labels))
		return replaceMem(addr, OperandConst(*maybe_val), labels); // try to keep the info
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
			if((*iter).pred().countTempVars())
			{
				OperandVar temp_var(0);
				Operand* expr = NULL;
				if((*iter).pred().getIsolatedTempVar(temp_var, expr)) // returns false if several tempvars
				{
					// try to keep the info
					rtn |= replaceTempVar(temp_var, *expr);
					// then remove the predicate
					DBG(color::IYel() << "- " << (*iter).pred())
					generated_preds.remove(iter);
					loop = true;
					break;
				}
			}
		}
	} while(loop);
	
	// Second step: remove everything that is left and still contains a tempvar
	for(SLList<LabelledPredicate>::Iterator iter(generated_preds); iter; )
	{
		if((*iter).pred().countTempVars())
		{	// remove the predicate
			DBG(color::IYel() << "- " << (*iter).pred())
			generated_preds.remove(iter);
			continue;
		}
		iter++;
	}
	return rtn;
}

// this mindlessly replaces all occurences of var by expr
bool Analysis::State::replaceVar(const OperandVar& var, const Operand& expr)
{
	bool rtn = false;
	for(PredIterator piter(*this); piter; )
	{
		if(piter.pred().involvesVariable(var))
		{
			Predicate p(piter.pred());
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

bool Analysis::State::replaceTempVar(const OperandVar& temp_var, const Operand& expr)
{
	bool rtn = false;
	for(SLList<LabelledPredicate>::Iterator iter(generated_preds); iter; )
	{
		if((*iter).pred().involvesVariable(temp_var))
		{
			Predicate p = (*iter).pred();
			Path l = (*iter).labels(); // just keep the labels
			String prev_str = _ << p;
			
			p.update(temp_var, expr);
			if(p.isIdent()) // if we just transformed t1 = X into X = X
			{
				iter++;
				continue;
			}
			if(rtn == false) // first time
			{
				DBG(color::IBlu() << "[" << expr << " / " << temp_var << "]")
				rtn = true;
			}
			DBG(color::IBlu() << DBG_SEPARATOR " " << color::Cya()  << "- " << prev_str)
			DBG(color::IBlu() << DBG_SEPARATOR " " << color::ICya() << "+ " << p)
			generated_preds.set(iter, LabelledPredicate(p, l));
		}
		iter++;
	}
	return rtn;
}

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
			DBG(color::IBlu() << DBG_SEPARATOR " " << color::Cya()  << "- " << prev_str)
			if(iter.item().pred().isIdent())
				DBG(color::IBlu() << DBG_SEPARATOR " " << color::ICya() << "  (not adding identity)")
			else
			{
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
bool Analysis::State::update(const OperandVar& opd_to_update, const Operand& opd_modifier)
{
	Operand* opd_modifier_prenew = opd_modifier.copy();
	Option<Operand*> maybe_opd_modifier_new = opd_modifier_prenew->replaceConstants(constants.toSimplified()); // replace constants
	// for example instead of doing [?13+t1/?13], do [?13+4/?13]
	Operand* opd_modifier_new = maybe_opd_modifier_new ? *maybe_opd_modifier_new : opd_modifier_prenew;

	// amongst other things this can simplify constant arithopr such as 8+(4-2)
	if(Option<Operand*> opd_modifier_new_simplified = opd_modifier_new->simplify())
	{
		DBG(color::Pur() << DBG_SEPARATOR << color::Blu() << " Simplified " << *opd_modifier_new << " to " << **opd_modifier_new_simplified)
		delete opd_modifier_new;
		opd_modifier_new = *opd_modifier_new_simplified;
	}

	DBG(color::IPur() << DBG_SEPARATOR << color::Blu() << " [" << *opd_modifier_new << " / " << opd_to_update << "]")
	bool rtn = false;
	
	for(PredIterator piter(*this); piter; piter++)
	{
		if(piter.pred().involvesVariable(opd_to_update))
		{
			DBG(color::IPur() << DBG_SEPARATOR << color::Blu() << " " DBG_SEPARATOR << color::Cya() << " - " << *piter)
			
			// updating the predicate
			Predicate p = piter.pred();
			bool update_state = p.update(opd_to_update, *opd_modifier_new);
			ASSERTP(update_state, "nothing was updated");

			LabelledPredicate lp = LabelledPredicate(p, piter.labels());
			setPredicate(piter, lp);
			DBG(color::IPur() << DBG_SEPARATOR << color::Blu() << " " DBG_SEPARATOR << color::ICya() << " + " << *piter)
			movePredicateToGenerated(piter); // cf issue "ANALYSIS SOUNDNESS: predicates may lack labels"
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
Option<Constant> Analysis::State::findConstantValueOfVar(const OperandVar& var)
{
	if(!isConstant(var))
		return none;
	return some(constants[var]);
}

// bool Analysis::State::findConstantValueOfVar_old(const OperandVar& var, t::int32& val)
// {
	/* we should separate kinds:
	*  OperandConst or OperandMem : nothing to do
	*  OperandArith or OperandVar : recursive call to findConstantValueOfVar with an exclusion list to avoid infinite loops
	*  complexity issues! :(
	*  split these in two functions : let this one as is and create a findConstantValueOfVarRecursively
	*  if this (simpler) first function fails, call the second one
	*  that does not improve worst-case scenario complexity but avoids worst case scanrios a lot
	*/
	/*
	for(SLList<Predicate>::Iterator iter(generated_preds); iter; iter++)
	{
		if((*iter).opr() == CONDOPR_EQ) // operator is =
		{
			if((*iter).leftOperand() == var) // left operand matches our var
				if(Option<OperandConst> maybe_val = (*iter).rightOperand().evalConstantOperand()) // other operand can be evald to const
				{
					// val = ((OperandConst&)(*iter).rightOperand()).value();
					val = (*maybe_val).value();
					return true;
				}
				
			if((*iter).rightOperand() == var) // right operand matches our var
				if(Option<OperandConst> maybe_val = (*iter).leftOperand().evalConstantOperand()) // other operand can be evald to const
				{
					// val = ((OperandConst&)(*iter).leftOperand()).value();
					val = (*maybe_val).value();
					return true;
				}
		}
	}
	if(!var.isTempVar()) // we need to parse previously generated preds as well
	{
		SLList<LabelledPredicate> top_list = labelled_preds.first(); // getTopList();
		for(SLList<LabelledPredicate>::Iterator iter(top_list); iter; iter++)
		{
			if((*iter).pred().opr() == CONDOPR_EQ) // operator is =
			{
				if((*iter).pred().leftOperand() == var) // left operand matches our var
					if(Option<OperandConst> maybe_val = (*iter).pred().rightOperand().evalConstantOperand()) // other operand can be evald to const
					{
						// val = ((OperandConst&)(*iter).rightOperand()).value();
						val = (*maybe_val).value();
						return true;
					}
					
				if((*iter).pred().rightOperand() == var) // right operand matches our var
					if(Option<OperandConst> maybe_val = (*iter).pred().leftOperand().evalConstantOperand()) // other operand can be evald to const
					{
						// val = ((OperandConst&)(*iter).leftOperand()).value();
						val = (*maybe_val).value();
						return true;
					}
			}
		}
	}
	return false; // No matches found
}*/

// TODO: improve to handle affine equations ("[SP-12] -1 = 0" etc...)
Option<Constant> Analysis::State::findConstantValueOfMemCell(const OperandMem& mem, Path &labels)
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
				return (*maybe_val).value();
			}
		}
		else if(p.rightOperand() == mem)
		{
			if(Option<OperandConst> maybe_val = p.leftOperand().evalConstantOperand())
			{
				labels += piter.labels();
				return (*maybe_val).value();
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
		if(piter.pred().isAffine(var, sp)) // try and look for an affine case ((.. + ..)-..) = (..-..)
		{
			AffineEquationState eqstate(sp);
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
		/*
			Operand* opd_left  = piter.pred().leftOperand().copy();
			Operand* opd_right = piter.pred().rightOperand().copy();
			Operand* opd_result = NULL;
			Operand* new_opd = NULL;
			// write this clearer maybe
			int found_var = 0; // 0 = not found, 1 = found in opdleft, 2 = found in opdright
			int found_sp = 0; // 0 = not found, 1 = found in opdleft, 2 = found in opdright
			int delta = 0;
			pop_result_t res;
			/// left operand ///-
			do
			{
				res = opd_left->doAffinePop(opd_result, new_opd);
				if(res == POPRESULT_DONE)
				{
					if(opd_result->kind() == OPERAND_CONST)
						delta += ((OperandConst*)opd_result)->value();
					if(opd_result->kind() == OPERAND_VAR)
					{
						if(*opd_result == var && !found_var)
							found_var = 1; // we found it on the left side of the equation
						else if(*opd_result == sp && !found_sp)
							found_sp = 1; // we found it on the left side of the equation
						else
						{
							// FAIL! we found something that is neither var nor sp, or we had already found them
							res = POPRESULT_FAIL;
							break;
						}
					}
				}
				if(res == POPRESULT_CONTINUE)
				{
					if(opd_result->kind() == OPERAND_CONST)
					{
						delta += ((OperandConst*)opd_result)->value();
						opd_left = new_opd;
					}
					if(opd_result->kind() == OPERAND_VAR)
					{
						if(*opd_result == var && !found_var)
						{
							found_var = 1; // we found it on the left side of the equation
							opd_left = new_opd;
						}								
						else if(*opd_result == sp && !found_sp)
						{
							found_sp = 1; // we found it on the left side of the equation
							opd_left = new_opd;
						}
						else
						{
							// FAIL! we found something that is neither var nor sp, or we had already found them
							res = POPRESULT_FAIL;
							break;
						}
					}
				}
			} while(res == POPRESULT_CONTINUE);
			if(res == POPRESULT_FAIL)
			{
				continue; // this affine analysis failed
			}
			do
			{
				res = opd_right->doAffinePop(opd_result, new_opd);
				if(res == POPRESULT_DONE)
				{
					if(opd_result->kind() == OPERAND_CONST)
						delta -= ((OperandConst*)opd_result)->value();
					if(opd_result->kind() == OPERAND_VAR)
					{
						if(*opd_result == var && !found_var)
							found_var = 2; // we found it on the left side of the equation
						else if(*opd_result == sp && !found_sp)
							found_sp = 2; // we found it on the left side of the equation
						else
						{
							// FAIL! we found something that is neither var nor sp, or we had already found them
							res = POPRESULT_FAIL;
							break;
						}
					}
				}
				if(res == POPRESULT_CONTINUE)
				{
					if(opd_result->kind() == OPERAND_CONST)
					{
						delta -= ((OperandConst*)opd_result)->value();
						opd_right = new_opd;
					}
					if(opd_result->kind() == OPERAND_VAR)
					{
						if(*opd_result == var && !found_var)
						{
							found_var = 2; // we found it on the left side of the equation
							opd_right = new_opd;
						}								
						else if(*opd_result == sp && !found_sp)
						{
							found_sp = 2; // we found it on the left side of the equation
							opd_right = new_opd;
						}
						else
						{
							// FAIL! we found something that is neither var nor sp, or we had already found them
							res = POPRESULT_FAIL;
							break;
						}
					}
				}
			} while(res == POPRESULT_CONTINUE);
			if(res == POPRESULT_FAIL)
			{
				continue; // this affine analysis failed
			}
			if(found_var && found_sp && found_sp != found_var) // we want them to be on opposite sides, otherwise we have ?var = -sp + K !
			{
				if(found_var == 1)
					delta = -delta;
				return delta;
			}
		}
		*/
	}
	DBG(color::IRed() << "findStackRelativeValueOfVar failed!")
	return none; // no matches found
}
/**
 * @fn bool Analysis::State::findValueOfCompVar(const OperandVar& var, Operand*& opd_left, Operand*& opd_right);
 * For a register variable ?X, try to find a predicate ?X = (opd_left ~ opd_right), and return false if it cannot find any
 * @return true if a value was found
 */
bool Analysis::State::findValueOfCompVar(const OperandVar& var, Operand*& opd_left, Operand*& opd_right)
{
	// We only explore the local generated_preds. This seems reasonable since we should be able to find this predicate in the local BB
	for(SLList<LabelledPredicate>::Iterator iter(generated_preds); iter; iter++)
	{
		const Predicate& p = (*iter).pred();
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
	if(Option<Constant> val = findConstantValueOfVar(var))
		return some(OperandMem(OperandConst(*val)));
	if(Option<t::int32> val = findStackRelativeValueOfVar(var, labels))
		return some(OperandMem(OperandConst(SP+*val)));
		// return some(OperandMem(OperandConst(*val), true));
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

Predicate* Analysis::State::getPredicateGeneratedByCondition(sem::inst condition, bool taken)
{
	cond_t kind = condition.cond();
	t::int16 sr = condition.sr();
	Operand *opd_left = NULL, *opd_right = NULL;
	if(!findValueOfCompVar(OperandVar(sr), opd_left, opd_right))
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
Option<OperandConst> Analysis::State::getConstantDataValue(const OperandMem& addr_mem, otawa::sem::type_t type)
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

/*
void Analysis::State::processBB(BasicBlock *bb)
{
	analyzeBB(bb);
	// ASSERT(labelled_preds);
	
	// these will be overwritten by further analysis, so back them up
	SLList<LabelledPredicate> generated_preds_backup	   = generated_preds;
	SLList<LabelledPredicate> generated_preds_taken_backup = generated_preds_taken;
	SLList<LabelledPredicate> top_list_backup			   = labelled_preds.first();
	ConstantVariables constants_backup(constants);

	int edgeId = 0;
	for(BasicBlock::OutIterator outs(bb); outs; outs++)
	{
		if(outs->kind() == Edge::TAKEN
		|| outs->kind() == Edge::NOT_TAKEN
		|| outs->kind() == Edge::VIRTUAL
		|| outs->kind() == Edge::VIRTUAL_RETURN) // Filter out irrelevant edges (calls...)
		{
			const SLList<LabelledPredicate> &relevant_preds = (outs->kind() == Edge::TAKEN) ?
				generated_preds_taken_backup :
				generated_preds_backup;
			
			if(edgeId++) // if this is not the first valid edge
			{
				labelled_preds += top_list_backup; // copy the predicates we have generated until this node into a new list
				constants = constants_backup; // also reset the constants
			}			

			// label our list of predicates with the current edge then append it
			SLList<LabelledPredicate> labelled_analysis_result = labelPredicateList(relevant_preds, *outs);
			// label the constants as well
			constants.label(*outs);
			
			// Add result to topList
			SLList<LabelledPredicate> topList = labelled_preds.first();
			topList += labelled_analysis_result;
			labelled_preds.removeFirst();
			labelled_preds.addFirst(topList);
		}
	}
}
*/