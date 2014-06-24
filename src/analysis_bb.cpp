#include <otawa/cfg/BasicBlock.h>
#include <otawa/sem/inst.h>
#include <otawa/sem/PathIter.h>
#include <elm/string/String.h>
#include "predicate.h"
#include "operand.h"
#include "debug.h"
#include "analysis.h"

using namespace otawa::sem;

void Analysis::analyzeBB(const BasicBlock *bb)
{
	SLList<Predicate> generated_preds_before_condition;
	sem::inst condition;
	generated_preds.clear();
	generated_preds_taken.clear();
	
	// parse assembly instructions
	for(BasicBlock::InstIterator insts(bb); insts; insts++)
	{
		DBG(COLOR_Pur << *insts)
		Block block;
		insts->semInsts(block);
		
		PathIter seminsts;
		// parse semantical instructions with PathIter
		for(seminsts.start(*insts); seminsts; seminsts++)
		{
			DBG(COLOR_IPur << *seminsts)
			
			if(seminsts.isCond()) // IF
			{	// backup the list of generated predicates before entering the condition
				generated_preds_before_condition = generated_preds; // side effect: reverses the order of the list
				DBG(COLOR_IBlu << "(Parsing taken path)")
			}
			if(seminsts.pathEnd()) // CONT
			{ 	// dumping the current generated_preds into the previous_paths_preds list
				invalidateTempVars(); // do the end-of-instruction tempvar invalidation first
				DBG(COLOR_IBlu << "(Parsing not taken path)")
				generated_preds_taken = generated_preds;
				generated_preds = generated_preds_before_condition;
				//previous_paths_preds += generated_preds;
			}
			
			Operand *opd1 = NULL, *opd2 = NULL;
			condoperator_t opr = CONDOPR_EQ; // Default is =
			bool make_pred = false;
			
			// some shortcuts (the seminsts.F functions are not called at this point)
			const t::int16& a = seminsts.a(), b = seminsts.b(), d = seminsts.d();
			const t::int32& cst = seminsts.cst();
			const t::int16& reg = seminsts.reg(), addr = seminsts.addr();
			
			// TODO: test unsigned MUL DIV MOD CMP
			switch(seminsts.op())
			{
				case NOP:
					assert(!UNTESTED_CRITICAL);
					DBG(COLOR_BIRed << "Untested operand NOP running!")
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
					make_pred = true;
					opd1 = new OperandVar(reg);
					opd2 = new OperandMem(OperandVar(addr));
					break;
				case STORE:	// MEM_type(addr) <- reg
					invalidateMem(addr);
					make_pred = true;
					opd1 = new OperandMem(OperandVar(addr));
					opd2 = new OperandVar(reg);
					break;
				case SCRATCH:
					invalidateVar(d);
					break;
				case SET:
					invalidateVar(d);
					make_pred = true;
					opd1 = new OperandVar(d);
					opd2 = new OperandVar(a);
					break;
				case SETI:
					make_pred = true;
					invalidateVar(d);
					opd1 = new OperandVar(d);
					opd2 = new OperandConst(cst);
					break;
				case SETP:
					assert(!UNTESTED_CRITICAL);
					DBG(COLOR_BIRed << "Untested operand SETP running!")
					break;
				case CMP:
				case CMPU:
					make_pred = true;
					opd1 = new OperandVar(d);
					{
						invalidateVar(d);
						Operand *opd21 = new OperandVar(a);
						Operand *opd22 = new OperandVar(b);
						opd2 = new OperandArithExpr(ARITHOPR_CMP, *opd21, *opd22);
					}
					break;
				case ADD:
					{
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
							}
							else // d <- d+b
							{	// [d-b / d]
								update(OperandVar(d), OperandArithExpr(ARITHOPR_SUB, OperandVar(d), OperandVar(b)));
							}
						}
						else 
						{
							if(d == b) // d <- d+a
							{	// [d-a / d]
								update(OperandVar(d), OperandArithExpr(ARITHOPR_SUB, OperandVar(d), OperandVar(a)));
							}
							else
							{
								// d is not related to a or b, invalidate predicates that involve d
								invalidateVar(d);
								opd1 = new OperandVar(d);
								Operand *opd21 = new OperandVar(a);
								Operand *opd22 = new OperandVar(b);
								opd2 = new OperandArithExpr(ARITHOPR_ADD, *opd21, *opd22);
								make_pred = true; // d = a + b
							}
						}
					}
					break;
				case SUB:
					opd1 = new OperandVar(d);
					{
						bool d_eq_a = d == a, d_eq_b = d == b;
						if(d_eq_a)
						{
							if(d_eq_b) // d <- d-d
							{	// [0 / d], d is set to 0!
								invalidateVar(d);
								opd1 = new OperandVar(d);
								opd2 = new OperandConst(0);
							}
							else // d <- d-b
							{	// [d+b / d]
								update(OperandVar(d), OperandArithExpr(ARITHOPR_ADD, OperandVar(d), OperandVar(b)));
							}
						}
						else
						{
							if(d_eq_b) // d <- a-d
							{	// [a-d / d], this function f has a f°f=Id property
								update(OperandVar(d), OperandArithExpr(ARITHOPR_SUB, OperandVar(a), OperandVar(d)));
							}
							else // d <- a-b
							{
								invalidateVar(d);
								make_pred = true;
								opd1 = new OperandVar(d);
								Operand *opd21 = new OperandVar(a);
								Operand *opd22 = new OperandVar(b);
								opd2 = new OperandArithExpr(ARITHOPR_SUB, *opd21, *opd22);
							}
						}
					}
					break;
				case SHL: // (*) TODO! don't we need to check for cases d <- d << 2 ???
					opd1 = new OperandVar(d);
					{
						Operand *opd21 = new OperandVar(a);
						t::int32 val;
						// b is usually a tempvar that has been previously set to a constant value
						if(!findConstantValueOfTempVar(b, val))
						{
							DBG(COLOR_Blu << "  [t1 could not be identified as a constant value]")
							invalidateVar(d); // only invalidate now (otherwise we can't find t1 for shl t1, ?0, t1)
						}
						else
						{
							DBG(COLOR_Blu << "  [t1 identified as 0x" << io::hex(val) << "]")
							invalidateVar(d);
							make_pred = true;
							Operand *opd22 = new OperandConst(1 << val); // 2^val
							opd2 = new OperandArithExpr(ARITHOPR_MUL, *opd21, *opd22);
						}
					}
					break;
				case ASR: // TODO test: is this really legit?
					assert(!UNTESTED_CRITICAL);
					DBG(COLOR_BIRed << "Untested operator running!")
				case SHR: // TODO: see previously (*)
					opd1 = new OperandVar(d);
					{
						Operand *opd21 = new OperandVar(a);
						t::int32 val;
						// b is usually a tempvar that has been previously set to a constant value
						if(!findConstantValueOfTempVar(b, val))
						{
							DBG(COLOR_Blu << "  [" << OperandVar(b) << " could not be identified as a constant value]")
							break;
						}
						else
							DBG(COLOR_Blu << "  [" << OperandVar(b) << " identified as 0x" << io::hex(val) << "]")
						
						// only invalidate now (otherwise we can't find t1 for shl t1, ?0, t1)
						invalidateVar(d);
						make_pred = true;
						Operand *opd22 = new OperandConst(1 << val); // 2^val
						opd2 = new OperandArithExpr(ARITHOPR_DIV, *opd21, *opd22);
					}
					break;
				case NEG: // TODO test
					assert(!UNTESTED_CRITICAL);
					DBG(COLOR_BIRed << "Untested operator running!")
					{
						if(a == d) // d <- -d
						{	// [-1 * d / d]
							// update(OperandVar(d), OperandArithExpr(ARITHOPR_MUL, OperandConst(-1), OperandVar(d)));
							update(OperandVar(d), OperandArithExpr(ARITHOPR_NEG, OperandVar(d))); // TODO test
						}
						else
						{	
							invalidateVar(d);
							make_pred = true;
							opd1 = new OperandVar(d);
							Operand *opd21 = new OperandConst(-1);
							Operand *opd22 = new OperandVar(a);
							opd2 = new OperandArithExpr(ARITHOPR_MUL, *opd21, *opd22);
						}
					}
					break;
				case NOT:		// d <- ~a
				case AND:		// d <- a & b
				case OR:		// d <- a | b
				case XOR:		// d <- a ^ b
					assert(!UNTESTED_CRITICAL);
					DBG(COLOR_BIRed << "Unimplemented operator running!")
					invalidateVar(d);
					break;
				case MULU:
					assert(!UNTESTED_CRITICAL);
					DBG(COLOR_BIRed << "Untested unsigned variant running!")
				case MUL:
					make_pred = true;
					opd1 = new OperandVar(d);
					{
						if(d == a)
						{
							if(d == b) // d <- d*d
							{
								assert(!UNTESTED_CRITICAL);
								DBG(COLOR_BIRed << "Untested case of operator MUL running!")
								invalidateVar(d); // we have no way to do [sqrt(d) / d], so just invalidate
								opr = CONDOPR_LE; // and add a "0 <= d" predicate
								opd1 = new OperandConst(0);
								opd2 = new OperandVar(d);
							}
							else // d <- d*b
							{	// [d/b / d] // we will have to assume that 0/0 is scratch!
								assert(!UNTESTED_CRITICAL);
								DBG(COLOR_BIRed << "Untested case of operator MUL running!")
								update(OperandVar(d), OperandArithExpr(ARITHOPR_DIV, OperandVar(d), OperandVar(b)));
								
								// we also add a predicate to say that d is now a multiple of b
								{
									Operand *opd11 = new OperandVar(d);
									Operand *opd12 = new OperandVar(b);
									opd1 = new OperandArithExpr(ARITHOPR_MOD, *opd11, *opd12); // d % b (%0 is to consider!)
								}
								opd2 = new OperandConst(0);
							}
						}
						else
						{
							if(d == b) // d <- a*d
							{	// [d/a / d] // we will have to assume that 0/0 is scratch!
								assert(!UNTESTED_CRITICAL);
								DBG(COLOR_BIRed << "Untested case of operator MUL running!")
								update(OperandVar(d), OperandArithExpr(ARITHOPR_DIV, OperandVar(d), OperandVar(a)));
								
								// we also add a predicate to say that d is now a multiple of a
								{
									Operand *opd11 = new OperandVar(d);
									Operand *opd12 = new OperandVar(a);
									opd1 = new OperandArithExpr(ARITHOPR_MOD, *opd11, *opd12); // d % a (%0 is to consider!)
								}
								opd2 = new OperandConst(0);
							}
							else // d <- a*b
							{
								opd1 = new OperandVar(d);
								invalidateVar(d);
								{
									Operand *opd21 = new OperandVar(a);
									Operand *opd22 = new OperandVar(b);
									opd2 = new OperandArithExpr(ARITHOPR_MUL, *opd21, *opd22);
								}
							}
						}
						break;
					}
					break;
				case DIVU:
					assert(!UNTESTED_CRITICAL);
					DBG(COLOR_BIRed << "Untested unsigned variant running!")
				case DIV:
					assert(!UNTESTED_CRITICAL);
					DBG(COLOR_BIRed << "Untested operator running!")
					if(d == a)
					{
						if(d == b) // d <- d / d
						{	// [1 / d]
							// TODO: this should be okay to assume d != 0, right? otherwise the program is faulty?
							// or should we use a SMT solver mode that supports / 0?
							make_pred = true;
							opd1 = new OperandVar(d);
							opd2 = new OperandConst(1);
							invalidateVar(d);
						}
						else // d <- d / b
						{
							invalidateVar(d); // we cannot just write [d*a / d] because we lost info
						}
					}
					else
					{
						if(d == b) // d <- a / d
						{
							invalidateVar(d);
						}
						else // d <- a / b
						{	// + (d = a/b)
							make_pred = true;
							opd1 = new OperandVar(d);
							invalidateVar(d);
							{
								Operand *opd21 = new OperandVar(a);
								Operand *opd22 = new OperandVar(b);
								opd2 = new OperandArithExpr(ARITHOPR_DIV, *opd21, *opd22);
							}
						}
					}
					break;
				case MODU:
				case MOD:
					invalidateVar(d);
					if(d == a || d == b) // TODO: it's right to not handle these weird cases, right?
						break;
					make_pred = true;
					opd1 = new OperandVar(d);
					{
						Operand *opd21 = new OperandVar(a);
						Operand *opd22 = new OperandVar(b);
						opd2 = new OperandArithExpr(ARITHOPR_MOD, *opd21, *opd22);
					}
					break;
				case SPEC: // special instruction (d: code, cst: sub-code)
					invalidateVar(d);
					break;
				default:
					make_pred = false;
			}
			if(make_pred)
			{
				assert(opd1);
				assert(opd2);
				DBG(COLOR_IPur DBG_SEPARATOR COLOR_IGre " + " << Predicate(opr, *opd1, *opd2))
				generated_preds += Predicate(opr, *opd1, *opd2);
			}
		}
		// all tempvars are freed at the end of an assembly instruction, so invalidate them
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
bool Analysis::invalidateVar(const OperandVar& var)
{
	bool rtn = false;
	
	// Invalidate predicates that have already been labelled
	SLList<LabelledPredicate> top_list = getTopList();
	for(SLList<LabelledPredicate>::Iterator iter(top_list); iter; )
	{
		if((*iter).pred().involvesVariable(var))
		{
			DBG(COLOR_IPur DBG_SEPARATOR COLOR_IYel " - " << *iter)
			top_list.remove(iter);
			rtn = true;
			continue;
		}
		iter++;
	}
	setTopList(top_list);
	
	// Also invalidate predicates of the current BB
	for(SLList<Predicate>::Iterator iter(generated_preds); iter; )
	{
		if((*iter).involvesVariable(var))
		{
			DBG(COLOR_IPur DBG_SEPARATOR COLOR_IYel " - " << *iter)
			generated_preds.remove(iter);
			rtn = true;
			continue;
		}
		iter++;
	}	
	return rtn;
}

bool Analysis::invalidateMem(const OperandVar& var)
{
	Option<OperandMem> addr = findAddressOfVar(var);
	if(!addr)
	{
		DBG(COLOR_IPur " " COLOR_IYel " Identification of " << var << " failed, invalidating whole memory")
		invalidateAllMemory();
		return true;
	}

	bool rtn = false;
	// Invalidate predicates that have already been labelled
	SLList<LabelledPredicate> top_list = getTopList();
	for(SLList<LabelledPredicate>::Iterator iter(top_list); iter; )
	{
		if((*iter).pred().involvesMemoryCell(addr))
		{
			DBG(COLOR_IPur DBG_SEPARATOR COLOR_IYel " - " << *iter)
			top_list.remove(iter);
			rtn = true;
			continue;
		}
		iter++;
	}
	setTopList(top_list);
	
	// Also invalidate predicates of the current BB
	for(SLList<Predicate>::Iterator iter(generated_preds); iter; )
	{
		if((*iter).involvesMemoryCell(addr))
		{
			DBG(COLOR_IPur DBG_SEPARATOR COLOR_IYel " - " << *iter)
			generated_preds.remove(iter);
			rtn = true;
			continue;
		}
		iter++;
	}	
	return rtn;
}

// This function will try to keep the information contained in the tempvars 
// by replacing them by their values in other predicates before removing them
bool Analysis::invalidateTempVars()
{
	bool loop, rtn = false;
	// First step: try and replace everything we can
	do {
		loop = false;
		for(SLList<Predicate>::Iterator iter(generated_preds); iter; iter++)
		{
			if((*iter).countTempVars())
			{
				OperandVar temp_var(0);
				Operand* expr = NULL;
				if((*iter).getIsolatedTempVar(temp_var, expr)) // returns false if several tempvars
				{
					rtn |= replaceTempVar(temp_var, *expr);
					
					// remove the predicate
					DBG(COLOR_IYel << "- " << *iter)
					generated_preds.remove(iter);
					loop = true;
					break;
				}
			}
		}
	} while(loop);
	
	// Second step: remove everything that is left and still contains a tempvar
	for(SLList<Predicate>::Iterator iter(generated_preds); iter; )
	{
		if((*iter).countTempVars())
		{	// remove the predicate
			DBG(COLOR_IYel << "- " << *iter)
			generated_preds.remove(iter);
			continue;
		}
		iter++;
	}	
	return rtn;
}

bool Analysis::replaceTempVar(const OperandVar& temp_var, const Operand& expr)
{
	// if(!start) return false; // already at the end of the list				
	bool rtn = false;
	for(SLList<Predicate>::Iterator iter(generated_preds); iter; ) // begin one position ahead of iterator 'start'
	{
		if((*iter).involvesVariable(temp_var))
		{
			Predicate p = *iter;
			String prev_str = _ << p;
			
			operand_state_t state = p.updateVar(temp_var, expr);
			assert(state != OPERANDSTATE_UNCHANGED); // otherwise means involvesVariable returned a false positive
			if(state == OPERANDSTATE_INVALID)
			{
				assert(!UNTESTED_CRITICAL);
				DBG(COLOR_BIRed << "Untested case of invalidation!")
				generated_preds.remove(iter);
			}
			if(p.isIdent()) // if we just transformed t1 = X into X = X
			{
				iter++;
				continue;
			}
				
			if(rtn == false) // first time
			{
				DBG(COLOR_IBlu << "[" << expr << " / " << temp_var << "]")
				rtn = true;
			}
				
			DBG(COLOR_IBlu DBG_SEPARATOR " " COLOR_Cya "- " << prev_str)
			DBG(COLOR_IBlu DBG_SEPARATOR " " COLOR_ICya "+ " << p)
			generated_preds.set(iter, p);
		}
		iter++;
	}
	return rtn;
}

// returns true if a variable has been updated
// second operand is usually an OperandArithExpr
bool Analysis::update(const OperandVar& opd_to_update, const Operand& opd_modifier)
{
	DBG(COLOR_IPur DBG_SEPARATOR COLOR_Blu " [" << opd_modifier << " / " << opd_to_update << "]")
	bool rtn = false;
	
	// Update predicates that have already been labelled
	SLList<LabelledPredicate> top_list = getTopList();
	for(SLList<LabelledPredicate>::Iterator iter(top_list); iter; )
	{
		if((*iter).pred().involvesVariable(opd_to_update))
		{
			DBG(COLOR_Cya << "  - " << *iter)
			
			// updating the predicate
			Predicate p = (*iter).pred();
			operand_state_t state = p.updateVar(opd_to_update, opd_modifier);
			assert(state != OPERANDSTATE_UNCHANGED);
			if(state == OPERANDSTATE_UPDATED) // making sure something is updated
			{
				LabelledPredicate lp = LabelledPredicate(p, (*iter).label());
				top_list.set(iter, lp);
				rtn = true;
				DBG(COLOR_ICya << "  + " << lp)
			}
			else if(state == OPERANDSTATE_INVALID)
			{
				DBG(COLOR_IPur DBG_SEPARATOR COLOR_Blu " " DBG_SEPARATOR COLOR_Cya "(new indirection is too complex)")
				top_list.remove(iter);
				continue;
			}
		}
		iter++;
	}
	setTopList(top_list);
	
	// Also update predicates of the current BB
	for(SLList<Predicate>::Iterator iter(generated_preds); iter; )
	{
		if((*iter).involvesVariable(opd_to_update))
		{			
			// updating the predicate
			Predicate p = *iter;
			DBG(COLOR_IPur DBG_SEPARATOR COLOR_Blu " " DBG_SEPARATOR COLOR_Cya " - " << *iter)
			operand_state_t state = p.updateVar(opd_to_update, opd_modifier);
			assert(state != OPERANDSTATE_UNCHANGED);
			if(state == OPERANDSTATE_UPDATED) // making sure something is updated
			{
				generated_preds.set(iter, p); 
				rtn = true;
				DBG(COLOR_IPur DBG_SEPARATOR COLOR_Blu " " DBG_SEPARATOR COLOR_ICya " + " << *iter)
			}
			else if(state == OPERANDSTATE_INVALID)
			{
				DBG(COLOR_IPur DBG_SEPARATOR COLOR_Blu " " DBG_SEPARATOR COLOR_Cya "(new indirection is too complex)")
				generated_preds.remove(iter);
				continue;
			}
		}
		iter++;
	}	
	return rtn;
}

/**
 * @fn bool Analysis::findConstantValueOfTempVar(const OperandVar& var, OperandConst& val);
 * For a temporary variable tX, try to find a predicate tX=cst, and return cst when it finds it.
 * @return true if a value was found
 */
bool Analysis::findConstantValueOfTempVar(const OperandVar& var, t::int32& val)
{
	for(SLList<Predicate>::Iterator iter(generated_preds); iter; iter++)
	{
		if((*iter).opr() == CONDOPR_EQ) // operator is =
		{
			if((*iter).leftOperand() == var) // left operand matches our tempvar
				if((*iter).rightOperand().kind() == OPERAND_CONST) // other operand is const (maybe we could eval arithoprs too)
				{
					val = ((OperandConst&)(*iter).rightOperand()).value();
					return true;
				}
				
			if((*iter).rightOperand() == var) // right operand matches our tempvar
				if((*iter).leftOperand().kind() == OPERAND_CONST) // other operand is const
				{
					val = ((OperandConst&)(*iter).leftOperand()).value();
					return true;
				}
		}
	}
	return false; // No matches found
}
/**
 * @fn bool Analysis::findValueOfCompVar(const OperandVar& var, Operand*& opd_left, Operand*& opd_right);
 * For a register variable ?X, try to find a predicate ?X = (opd_left ~ opd_right), and return false if it cannot find any
 * @return true if a value was found
 */
bool Analysis::findValueOfCompVar(const OperandVar& var, Operand*& opd_left, Operand*& opd_right)
{
	// We only explore the local generated_preds. This seems reasonable since we should be able to find this predicate in the local BB
	for(SLList<Predicate>::Iterator iter(generated_preds); iter; iter++)
	{
		if((*iter).opr() == CONDOPR_EQ) // operator is =
		{
			if((*iter).leftOperand() == var) // left operand matches our register
			{
				if((*iter).rightOperand().kind() == OPERAND_ARITHEXPR)
				{
					const OperandArithExpr& opd = (OperandArithExpr&)(*iter).rightOperand();
					if(opd.opr() == ARITHOPR_CMP) // other operand has to be ??? ~ ???
					{
						opd_left = opd.leftOperand().copy();
						opd_right = opd.rightOperand().copy();
						return true;
					}
				}
			}
				
			if((*iter).rightOperand() == var) // right operand matches our register
			{
				if((*iter).leftOperand().kind() == OPERAND_ARITHEXPR)
				{
					const OperandArithExpr& opd = (OperandArithExpr&)(*iter).leftOperand();
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

Option<OperandMem> Analysis::findAddressOfVar(const OperandVar& var)
{
	return none; // TODO!!!
}

void Analysis::invalidateAllMemory()
{
	SLList<LabelledPredicate> top_list = getTopList();
	for(SLList<LabelledPredicate>::Iterator iter(top_list); iter; )
	{
		if((*iter).pred().involvesMemory())
		{
			DBG(COLOR_IPur DBG_SEPARATOR COLOR_IYel " - " << *iter)
			top_list.remove(iter);
			continue;
		}
		iter++;
	}
	setTopList(top_list);
	
	for(SLList<Predicate>::Iterator iter(generated_preds); iter; )
	{
		if((*iter).involvesMemory())
		{
			DBG(COLOR_IPur DBG_SEPARATOR COLOR_IYel " - " << *iter)
			generated_preds.remove(iter);
			continue;
		}
		iter++;
	}
}

Predicate* Analysis::getPredicateGeneratedByCondition(sem::inst condition, bool taken)
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
	return new Predicate(opr, reverse ? *opd_right : *opd_left, reverse ? *opd_left : *opd_right);
}
