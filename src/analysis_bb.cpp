#include <otawa/cfg/BasicBlock.h>
#include <otawa/sem/inst.h>
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
					invalidateVar(OperandVar(seminsts.reg()));
					// waiting for LOAD/STORE support to make more out of it
					make_pred = false;
					break;
				// case STORE:	// MEM_type(addr) <- reg
					// waiting for LOAD/STORE support
				case SCRATCH:
					invalidateVar(OperandVar(seminsts.d()));
					make_pred = false;
					break;
				case SET:
					invalidateVar(OperandVar(seminsts.d()));
					opd1 = new OperandVar(seminsts.d());
					opd2 = new OperandVar(seminsts.a());
					break;
				case SETI:
					invalidateVar(OperandVar(seminsts.d()));
					opd1 = new OperandVar(seminsts.d());
					opd2 = new OperandConst(seminsts.cst());
					break;
				// case SETP:
					// nothing to do until we support LOAD/STORE
				case CMP:
				case CMPU:
					opd1 = new OperandVar(seminsts.d());
					{
						invalidateVar(OperandVar(seminsts.d()));
						Operand *opd21 = new OperandVar(seminsts.a());
						Operand *opd22 = new OperandVar(seminsts.b());
						opd2 = new OperandArithExpr(ARITHOPR_CMP, *opd21, *opd22);
					}
					break;
				case ADD:
					opd1 = new OperandVar(seminsts.d());
					{
						if(seminsts.d() == seminsts.a()) // d <- d+b
						{
							updateAdd(OperandVar(seminsts.d()), OperandVar(seminsts.b()));
							make_pred = false;
						}
						else if(seminsts.d() == seminsts.b()) // d <- d+a
						{
							updateAdd(OperandVar(seminsts.d()), OperandVar(seminsts.a()));
							make_pred = false;
						}
						else
						{
							// d is not related to a or b, invalidate predicates that involve d
							invalidateVar(OperandVar(seminsts.d()));
							Operand *opd21 = new OperandVar(seminsts.a());
							Operand *opd22 = new OperandVar(seminsts.b());
							opd2 = new OperandArithExpr(ARITHOPR_ADD, *opd21, *opd22);
						}
					}
					break;
				case SUB:
					opd1 = new OperandVar(seminsts.d());
					{
						// TODO: additional tests for updates
						bool d_eq_a = seminsts.d() == seminsts.a(), d_eq_b = seminsts.d() == seminsts.b();
						if(d_eq_a)
						{
							if(d_eq_b) // d <- d-d
							{
								// d is set to 0!
								invalidateVar(OperandVar(seminsts.d()));
								opd1 = new OperandVar(seminsts.d());
								opd2 = new OperandConst(0);
							}
							else // d <- d-b
							{								
								updateSub(OperandVar(seminsts.d()), OperandVar(seminsts.b()), false);
								make_pred = false;
							}
						}
						else
						{
							if(d_eq_b) // d <- a-d
							{
								updateSub(OperandVar(seminsts.d()), OperandVar(seminsts.a()), true);
								make_pred = false;
							}
							else // d <- a-b
							{
								invalidateVar(OperandVar(seminsts.d()));
								Operand *opd21 = new OperandVar(seminsts.a());
								Operand *opd22 = new OperandVar(seminsts.b());
								opd2 = new OperandArithExpr(ARITHOPR_SUB, *opd21, *opd22);
							}
						}
					}
					break;
				case SHL: // TODO test
					DBG(COLOR_BIRed << "Untested operand running!")
					opd1 = new OperandVar(seminsts.d());
					// TODO: !!!! what we need to do here is to build a identify(OperandVar()) function that
					//       would tell us if we can (we normally should be able to) replace that variable by
					//       a const expr. If yes, then do so
					{
						invalidateVar(OperandVar(seminsts.d()));
						Operand *opd21 = new OperandVar(seminsts.a());
						Operand *opd22 = new OperandConst(1 << seminsts.b()); // 2^b
						opd2 = new OperandArithExpr(ARITHOPR_MUL, *opd21, *opd22);
					}
					break;
				case SHR: // TODO test (is this really legit?)
				case ASR: // TODO test
					DBG(COLOR_BIRed << "Untested operand running!")
					opd1 = new OperandVar(seminsts.d());
					{
						// TODO: additional tests to update instead of invalidate
						invalidateVar(OperandVar(seminsts.d()));
						Operand *opd21 = new OperandVar(seminsts.a());
						Operand *opd22 = new OperandConst(1 << seminsts.b()); // 2^b
						opd2 = new OperandArithExpr(ARITHOPR_DIV, *opd21, *opd22);
					}
					break;
				case NEG: // TODO test
					DBG(COLOR_BIRed << "Untested operand running!")
					opd1 = new OperandVar(seminsts.d());
					{
						// TODO: additional tests to update instead of invalidate
						invalidateVar(OperandVar(seminsts.d()));
						Operand *opd21 = new OperandConst(-1);
						Operand *opd22 = new OperandVar(seminsts.a());
						opd2 = new OperandArithExpr(ARITHOPR_MUL, *opd21, *opd22);
					}
					break;
				// case NOT:		// d <- ~a
				// case AND:		// d <- a & b
				// case OR:			// d <- a | b
				// case XOR:		// d <- a ^ b
				case MULU:
				case MUL:
					opd1 = new OperandVar(seminsts.d());
					{
						// TODO: additional tests to update instead of invalidate
						invalidateVar(OperandVar(seminsts.d()));
						Operand *opd21 = new OperandVar(seminsts.a());
						Operand *opd22 = new OperandVar(seminsts.b());
						opd2 = new OperandArithExpr(ARITHOPR_MUL, *opd21, *opd22);
					}
					break;
				case DIVU:
				case DIV:
					opd1 = new OperandVar(seminsts.d());
					{
						// TODO: additional tests to update instead of invalidate
						invalidateVar(OperandVar(seminsts.d()));
						Operand *opd21 = new OperandVar(seminsts.a());
						Operand *opd22 = new OperandVar(seminsts.b());
						opd2 = new OperandArithExpr(ARITHOPR_DIV, *opd21, *opd22);
					}
				case MODU:
				case MOD:
					opd1 = new OperandVar(seminsts.d());
					{
						// TODO: additional tests to update instead of invalidate
						invalidateVar(OperandVar(seminsts.d()));
						Operand *opd21 = new OperandVar(seminsts.a());
						Operand *opd22 = new OperandVar(seminsts.b());
						opd2 = new OperandArithExpr(ARITHOPR_MOD, *opd21, *opd22);
					}
				case SPEC: // special instruction (d: code, cst: sub-code)
					invalidateVar(OperandVar(seminsts.d()));
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


bool Analysis::invalidateTempVars()
{
	bool loop, rtn = false;
	do {
		loop = false;
		for(SLList<Predicate>::Iterator iter(generated_preds); iter; iter++)
		{
			if((*iter).countTempVars())
			{
				OperandVar temp_var(0);
				Operand* expr = NULL;
				if((*iter).getIsolatedTempVar(temp_var, expr)) // returns false if several tempvars
					rtn |= replaceTempVarFrom(temp_var, *expr, iter);
					
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

bool Analysis::replaceTempVarFrom(const OperandVar& temp_var, const Operand& expr, const SLList<Predicate>::Iterator& start)
{
	if(!start) return false; // already at the end of the list
	bool loop, rtn = false;
	do {
		loop = false;
		SLList<Predicate>::Iterator iter(start);
		for(iter++; iter; iter++) // begin one position ahead of iterator 'start'
		{
			if((*iter).involvesVariable(temp_var))
			{
				if(rtn == false)
				{
					DBG(COLOR_IBlu << "[" << expr << " / " << temp_var << "]")
					rtn = true;
				}
				
				DBG(COLOR_Cya << "- " << *iter)
				Predicate p = (*iter);
				assert(p.updateVar(temp_var, expr)); // if this is false, it means involvesVariable returned a false positive
				generated_preds.set(iter, p); 
				
				DBG(COLOR_ICya << "+ " << *iter)
			}
		}
	} while(loop);
	return rtn;
}

/*
bool Analysis::updateGeneratedTempVarsList(const Operand& opd)
{
	DBG(COLOR_IYel << generated_temp_vars << opd.getTempVars())
	if(list<OperandVar> l = opd.getTempVars())
	{
		generated_temp_vars = generated_temp_vars.concat(l);
		return true;
	}
	return false;
}
//*/

/*
bool Analysis::replaceVarByItsValue(const OperandVar& opd_var, const OperandConst& val)
{
	DBG(COLOR_Blu << "  * Replacing " << opd_var << " (about to be invalidated) by " << val);
	return update(opd_var, val);
}
//*/

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

// updates generated_preds and labelled_preds
bool Analysis::updateAdd(OperandVar opd_to_update, OperandVar opd_modifier)
{
	return update(opd_to_update, OperandArithExpr(ARITHOPR_ADD, opd_to_update, opd_modifier));
}

bool Analysis::updateSub(OperandVar opd_to_update, OperandVar opd_modifier, bool reverse)
{
	return update(opd_to_update, OperandArithExpr(
		ARITHOPR_SUB,
		reverse ? opd_modifier : opd_to_update,
		reverse ? opd_to_update : opd_modifier));
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
