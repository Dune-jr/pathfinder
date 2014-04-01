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
		
		// Parse semantical instructions
		for(Block::InstIter seminsts(block); seminsts; seminsts++)
		{
			DBG(COLOR_IPur << *seminsts << COLOR_RCol)
			
			Operand *opd1 = NULL;
			Operand *opd2 = NULL;
			condoperator_t opr = CONDOPR_EQ; // Default
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
				case SCRATCH: // TODO test
					DBG(COLOR_BIRed << "Untested operand running!")
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
						// TODO test case 1+2
						if(seminsts.d() == seminsts.a())
						{
							updateAdd(OperandVar(seminsts.d()), seminsts.b());
							make_pred = false;
						}
						else if(seminsts.d() == seminsts.b())
						{
							updateAdd(OperandVar(seminsts.d()), seminsts.a());
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
						// TODO: additional tests to update instead of invalidate
						invalidateVar(OperandVar(seminsts.d()));
						Operand *opd21 = new OperandVar(seminsts.a());
						Operand *opd22 = new OperandVar(seminsts.b());
						opd2 = new OperandArithExpr(ARITHOPR_SUB, *opd21, *opd22);
					}
					break;
				case SHL: // TODO test
					DBG(COLOR_BIRed << "Untested operand running!")
					opd1 = new OperandVar(seminsts.d());
					{
						// TODO: additional tests to update instead of invalidate
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
				DBG(COLOR_IGre << "+ " << Predicate(opr, *opd1, *opd2))
				generated_preds += Predicate(opr, *opd1, *opd2);
			}
		}
	}
	
	DBG("Predicates generated by the BB: " << generated_preds)
}

// returns true if a variable has been invalidated
// TODO: problem! We need to update vars that we have not added to the toplist yet!!!
bool Analysis::invalidateVar(const OperandVar& opd_var)
{
	// TODO: we could implement sth to save some info, see yellow notes "28/03/14"
	bool rtn = false;
	
	// Invalidate predicates that have already been labelled
	SLList<LabelledPredicate> top_list = getTopList();
	bool loop;
	do {
		loop = false;
		// TODO: optimize this! We are rewinding the whole list every time to look for new predicates to invalidate! (same for below)
		for(SLList<LabelledPredicate>::Iterator iter(top_list); iter; iter++)
		{
			if((*iter).pred().isInvolvedVariable(opd_var))
			{
				DBG(COLOR_Red << "- " << *iter)
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
			if((*iter).isInvolvedVariable(opd_var))
			{
				DBG(COLOR_IRed << "- " << *iter)
				generated_preds.remove(iter);
				loop = true;
				rtn = true;
				break;
			}
		}
	} while(loop);
	
	return rtn;
}

// returns true if a variable has been updated
// second operand is usually an OperandArithExpr
bool Analysis::update(const OperandVar& opd_to_update, const Operand& opd_modifier)
{
	DBG(COLOR_IYel << "update called, opd_to_update=" << opd_to_update << ", opd_modifier=" << opd_modifier)
	bool rtn = false;
	
	// Update predicates that have already been labelled
	SLList<LabelledPredicate> top_list = getTopList();
	for(SLList<LabelledPredicate>::Iterator iter(top_list); iter; iter++)
	{
		if((*iter).pred().isInvolvedVariable(opd_to_update))
		{
			DBG(COLOR_ICya << "* " << *iter)
			
			// updating the predicate
			Predicate p = (*iter).pred();
			if(p.updateVar(opd_to_update, opd_modifier)) // making sure something is updated
			{
				top_list.set(iter, LabelledPredicate(p, (*iter).label()));
				rtn = true;
			}
		}
	}
	setTopList(top_list);
	
	// Also update predicates of the current BB
	for(SLList<Predicate>::Iterator iter(generated_preds); iter; iter++)
	{
		if((*iter).isInvolvedVariable(opd_to_update))
		{
			DBG(COLOR_ICya << "< " << *iter)
			
			// updating the predicate
			Predicate p = *iter;
			if(p.updateVar(opd_to_update, opd_modifier)) // making sure something is updated
			{
				static int nyaa = 0;
				if(nyaa == 0) { nyaa = 1; // TODO remove
				generated_preds.set(iter, p); }
				rtn = true;
				DBG(COLOR_ICya << "> " << *iter)
			}
		}
	}
	
	return rtn;
}

// updates generated_preds and labelled_preds
bool Analysis::updateAdd(OperandVar opd_to_update, OperandVar opd_modifier)
{
	// replaces d with d + a or d + b
	return update(opd_to_update, OperandArithExpr(ARITHOPR_ADD, opd_to_update, opd_modifier));
}
