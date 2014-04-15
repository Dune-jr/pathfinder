#include <otawa/cfg/BasicBlock.h>
#include <otawa/sem/inst.h>
#include <elm/string/String.h>
#include "predicate.h"
#include "operand.h"
#include "debug.h"
#include "analysis.h"

using namespace otawa::sem;

void Analysis::analyzeBB(const BasicBlock *bb)
{
	generated_preds.clear();
	
	// Parse assembly instructions
	for(BasicBlock::InstIterator insts(bb); insts; insts++)
	{
		DBG(COLOR_Pur << *insts << COLOR_RCol)
		
		Block block;
		insts->semInsts(block);
		// TODO: update temp_vars_max_id
		
		// Parse semantical instructions
		for(Block::InstIter seminsts(block); seminsts; seminsts++)
		{
			DBG(COLOR_IPur << *seminsts << COLOR_RCol)
			
			Operand *opd1 = NULL;
			Operand *opd2 = NULL;
			condoperator_t opr = CONDOPR_EQ; // Default is =
			
			// some shortcuts
			const t::int16& a = seminsts.a();
			const t::int16& b = seminsts.b();
			const t::int16& d = seminsts.d();
			const t::int32& cst = seminsts.cst();
			const t::int16& reg = seminsts.reg();
			// const t::int16& addr = seminsts.addr();
			
			bool make_pred = true;
			// TODO: test unsigned MUL DIV MOD CMP
			// TODO: do not generate t1 = t1 + t2 !!! update preds instead
			//       and only generate if we have sth like t1 = t3 + t2
			switch(seminsts.op())
			{
				// case BRANCH:	// perform a branch on content of register a
					// TODO
				// case IF:		// continue if condition cond is meet in register sr, else skip "jump" instructions
					// TODO
				case LOAD:	// reg <- MEM_type(addr)t
					invalidateVar(reg);
					// waiting for LOAD/STORE support to make more out of it
					make_pred = false;
					break;
				// case STORE:	// MEM_type(addr) <- reg
					// waiting for LOAD/STORE support
				case SCRATCH:
					invalidateVar(d);
					make_pred = false;
					break;
				case SET:
					invalidateVar(d);
					opd1 = new OperandVar(d);
					opd2 = new OperandVar(a);
					break;
				case SETI:
					invalidateVar(d);
					opd1 = new OperandVar(d);
					opd2 = new OperandConst(cst);
					break;
				// case SETP:
					// nothing to do until we support LOAD/STORE
				case CMP:
				case CMPU:
					opd1 = new OperandVar(d);
					{
						invalidateVar(d);
						Operand *opd21 = new OperandVar(a);
						Operand *opd22 = new OperandVar(b);
						opd2 = new OperandArithExpr(ARITHOPR_CMP, *opd21, *opd22);
					}
					break;
				case ADD:
					opd1 = new OperandVar(d);
					{
						if(d == a) // d <- d+b
						{	// [d-b / d]
							update(OperandVar(d), OperandArithExpr(ARITHOPR_SUB, OperandVar(d), OperandVar(b)));
							make_pred = false;
						}
						else if(d == b) // d <- d+a
						{	// [d-a / d]
							update(OperandVar(d), OperandArithExpr(ARITHOPR_SUB, OperandVar(d), OperandVar(a)));
							make_pred = false;
						}
						else
						{
							// d is not related to a or b, invalidate predicates that involve d
							invalidateVar(d);
							Operand *opd21 = new OperandVar(a);
							Operand *opd22 = new OperandVar(b);
							opd2 = new OperandArithExpr(ARITHOPR_ADD, *opd21, *opd22);
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
								make_pred = false;
							}
						}
						else
						{
							if(d_eq_b) // d <- a-d
							{	// [a-d / d], this function f has a f°f=Id property
								update(OperandVar(d), OperandArithExpr(ARITHOPR_SUB, OperandVar(a), OperandVar(d)));
								make_pred = false;
							}
							else // d <- a-b
							{
								invalidateVar(d);
								opd1 = new OperandVar(d);
								Operand *opd21 = new OperandVar(a);
								Operand *opd22 = new OperandVar(b);
								opd2 = new OperandArithExpr(ARITHOPR_SUB, *opd21, *opd22);
							}
						}
					}
					break;
				case SHL:
					opd1 = new OperandVar(d);
					{
						Operand *opd21 = new OperandVar(a);
						t::int32 val;
						// b is usually a tempvar that has been previously set to a constant value
						if(!findConstantValueOfTempVar(b, val))
						{
							DBG(COLOR_Blu << "  [t1 could not be identified as a constant value]")
							make_pred = false;
						}
						else
							DBG(COLOR_Blu << "  [t1 identified as 0x" << io::hex(val) << "]")
						
						// only invalidate now (otherwise we can't find t1 for shl t1, ?0, t1)
						invalidateVar(d);
						Operand *opd22 = new OperandConst(1 << val); // 2^val
						opd2 = new OperandArithExpr(ARITHOPR_MUL, *opd21, *opd22);
					}
					break;
				case ASR: // TODO test: is this really legit?
					assert(!UNTESTED_CRITICAL);
					DBG(COLOR_BIRed << "Untested operator running!")
				case SHR:
					opd1 = new OperandVar(d);
					{
						Operand *opd21 = new OperandVar(a);
						t::int32 val;
						// b is usually a tempvar that has been previously set to a constant value
						if(!findConstantValueOfTempVar(b, val))
						{
							DBG(COLOR_Blu << "  [t1 could not be identified as a constant value]")
							make_pred = false;
						}
						else
							DBG(COLOR_Blu << "  [t1 identified as 0x" << io::hex(val) << "]")
						
						// only invalidate now (otherwise we can't find t1 for shl t1, ?0, t1)
						invalidateVar(d);
						Operand *opd22 = new OperandConst(1 << val); // 2^val
						opd2 = new OperandArithExpr(ARITHOPR_DIV, *opd21, *opd22);
					}
					break;
				case NEG: // TODO test
					assert(!UNTESTED_CRITICAL);
					DBG(COLOR_BIRed << "Untested operator running!")
					opd1 = new OperandVar(d);
					{
						// TODO: additional tests to update instead of invalidate
						invalidateVar(d);
						Operand *opd21 = new OperandConst(-1);
						Operand *opd22 = new OperandVar(a);
						opd2 = new OperandArithExpr(ARITHOPR_MUL, *opd21, *opd22);
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
					assert(!UNTESTED_CRITICAL);
					DBG(COLOR_BIRed << "Untested operator running!")
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
								
								// TODO: is this good?
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
								
								// TODO: is this good?
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
								assert(!UNTESTED_CRITICAL);
								DBG(COLOR_BIRed << "Untested case of operator MUL running!")
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
							invalidateVar(d);
							opd1 = new OperandVar(d);
							opd2 = new OperandConst(1);
						}
						else // d <- d / a
						{
							invalidateVar(d);
							make_pred = false;
							// we cannot just write [d*a / d] because we lost info
						}
					}
					else
					{
						if(d == b) // d <- a / d
						{
							invalidateVar(d);
							make_pred = false;
							// we cannot just write [d*b / d] because we lost info
						}
						else // d <- a / b
						{	// + (d = a/b)
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
					{
						make_pred = false;
						break;
					}
					opd1 = new OperandVar(d);
					{
						Operand *opd21 = new OperandVar(a);
						Operand *opd22 = new OperandVar(b);
						opd2 = new OperandArithExpr(ARITHOPR_MOD, *opd21, *opd22);
					}
					break;
				case SPEC: // special instruction (d: code, cst: sub-code)
					invalidateVar(d);
					make_pred = false;
					break;
				default:
					make_pred = false;
			}
			if(make_pred)
			{
				DBG(COLOR_IGre << "  + " << Predicate(opr, *opd1, *opd2))
				generated_preds += Predicate(opr, *opd1, *opd2);
				//updateGeneratedTempVarsList(*opd1);
				//updateGeneratedTempVarsList(*opd2);
			}
		}
		// all the temporary registers are freed at the end of an assembly instruction,
		// so invalidate them
		invalidateTempVars();
	}
	
	DBG("Predicates generated by the BB: " << generated_preds)
}

// returns true if a variable has been invalidated
bool Analysis::invalidateVar(const OperandVar& opd_var)
{
	bool loop, rtn = false;
	
	//// Try to replace the var that we are about to remove 
	//OperandConst val(0);
	//if(seekValue(opd_var, val))
		//return replaceVarByItsValue(opd_var, val); // all vars have been replaced, so just exit
	
	// Invalidate predicates that have already been labelled
	SLList<LabelledPredicate> top_list = getTopList();
	do {
		loop = false;
		// TODO: optimize this! We are rewinding the whole list every time to look for new predicates to invalidate! (same for below)
		for(SLList<LabelledPredicate>::Iterator iter(top_list); iter; iter++)
		{
			if((*iter).pred().involvesVariable(opd_var))
			{
				DBG(COLOR_Red << "  - " << *iter)
				top_list.remove(iter);
				loop = true;
				rtn = true;
				break;
			}
		}
	} while(loop);
	setTopList(top_list);
	
	// Also invalidate predicates of the current BB
	do {
		loop = false;
		for(SLList<Predicate>::Iterator iter(generated_preds); iter; iter++)
		{
			if((*iter).involvesVariable(opd_var))
			{
				DBG(COLOR_IRed << "  - " << *iter)
				generated_preds.remove(iter);
				loop = true;
				rtn = true;
				break;
			}
		}
	} while(loop);
	
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
					DBG(COLOR_IRed << "- " << *iter)
					generated_preds.remove(iter);
					loop = true;
					break;
				}
			}
		}
	} while(loop);
	
	// Second step: remove everything that is left and still contains a tempvar
	// TODO(!!!): try to write:
	// 		prev_iter = iter; // at end of for loop
	//		prev_iter(generated_preds); // initialization
	//		this way, when we break and loop, we iter from the previous item, instead of the first one
	do {
		loop = false;
		for(SLList<Predicate>::Iterator iter(generated_preds); iter; iter++)
		{
			if((*iter).countTempVars())
			{
				// remove the predicate
				DBG(COLOR_IRed << "- " << *iter)
				generated_preds.remove(iter);
				loop = true;
				break;
			}
		}
	} while(loop);
	
	return rtn;
}

bool Analysis::replaceTempVar(const OperandVar& temp_var, const Operand& expr) //, const SLList<Predicate>::Iterator& start)
{
	// if(!start) return false; // already at the end of the list				
	bool rtn = false;
	for(SLList<Predicate>::Iterator iter(generated_preds); iter; iter++) // begin one position ahead of iterator 'start'
	{
		if((*iter).involvesVariable(temp_var))
		{
			Predicate p = *iter;
			String o = _ << p;
			
			assert(p.updateVar(temp_var, expr)); // if this is false, it means involvesVariable returned a false positive
			if(p.isIdent()) // if we just transformed t1 = X into X = X
				continue;
				
			if(rtn == false)
			{
				DBG(COLOR_IBlu << "[" << expr << " / " << temp_var << "]")
				rtn = true;
			}
				
			DBG(COLOR_Cya << "- " << o)
			DBG(COLOR_ICya << "+ " << p)
			generated_preds.set(iter, p);
		}
	}
	return rtn;
}

/**
 * @fn bool Analysis::findConstantValueOfTempVar(const OperandVar& var, OperandConst& val);
 * For a temporary variable tX, try to find a predicate tX=cst, and returns cst when it finds it.
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

// returns true if a variable has been updated
// second operand is usually an OperandArithExpr
bool Analysis::update(const OperandVar& opd_to_update, const Operand& opd_modifier)
{
	DBG(COLOR_Blu << "  [" << opd_modifier << " / " << opd_to_update << "]")
	
	bool rtn = false;
	
	// Update predicates that have already been labelled
	SLList<LabelledPredicate> top_list = getTopList();
	for(SLList<LabelledPredicate>::Iterator iter(top_list); iter; iter++)
	{
		if((*iter).pred().involvesVariable(opd_to_update))
		{
			DBG(COLOR_Cya << "  - " << *iter)
			
			// updating the predicate
			Predicate p = (*iter).pred();
			if(p.updateVar(opd_to_update, opd_modifier)) // making sure something is updated
			{
				LabelledPredicate lp = LabelledPredicate(p, (*iter).label());
				top_list.set(iter, lp);
				rtn = true;
				DBG(COLOR_ICya << "  + " << lp)
			}
		}
	}
	setTopList(top_list);
	
	// Also update predicates of the current BB
	for(SLList<Predicate>::Iterator iter(generated_preds); iter; iter++)
	{
		if((*iter).involvesVariable(opd_to_update))
		{
			DBG(COLOR_Cya << "  - " << *iter)
			
			// updating the predicate
			Predicate p = *iter;
			if(p.updateVar(opd_to_update, opd_modifier)) // making sure something is updated
			{
				generated_preds.set(iter, p); 
				rtn = true;
				DBG(COLOR_ICya << "  + " << *iter)
			}
		}
	}
	//if(rtn)
		//updateGeneratedTempVarsList(opd_modifier);		
	return rtn;
}

/*
// returns true if it found something
// if it did, the value will be stored in val
bool Analysis::seekValue(const OperandVar& var, OperandConst& val)
{
	bool rtn = false;
	SLList<LabelledPredicate> top_list = getTopList();
	for(SLList<LabelledPredicate>::Iterator iter(top_list); iter; iter++)
	{
		const Predicate& p = (*iter).pred();
		if(p.opr() != CONDOPR_EQ)
			continue;
		// once we found a const value, we do not check further
		// if the algorithm is correct, this should not be a problem (no inconsistency)
		if(p.leftOperand() == var)
			return p.rightOperand().evalConstantOperand(val);
		else if(p.rightOperand() == var)
			return p.leftOperand().evalConstantOperand(val);
	}
	return rtn;
}
*/
