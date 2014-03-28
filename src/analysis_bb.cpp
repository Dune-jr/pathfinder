#include <otawa/cfg/BasicBlock.h>
#include <otawa/sem/inst.h>
#include "predicate.h"
#include "debug.h"
#include "analysis.h"

using namespace otawa::sem;

SLList<Predicate> Analysis::analyzeBB(const BasicBlock *bb)
{
	SLList<Predicate> pred_list;
	
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
			switch(seminsts.op()) // TODO: test unsigned ADD SUB MUL DIV MOD CMP
			{
				// case BRANCH:	// perform a branch on content of register a
					// TODO
				// case IF:		// continue if condition cond is meet in register sr, else skip "jump" instructions
					// TODO
				// case LOAD:	// reg <- MEM_type(addr)
					// waiting for LOAD/STORE support
				// case STORE:	// MEM_type(addr) <- reg
					// waiting for LOAD/STORE support
				// case SCRATCH:
					// just invalidate d
				case SET:
					opd1 = new OperandVar(seminsts.d());
					opd2 = new OperandVar(seminsts.a());
					break;
				case SETI:
					opd1 = new OperandVar(seminsts.d());
					opd2 = new OperandConst(seminsts.cst());
					break;
				// case SETP:
					// nothing to do until we support LOAD/STORE
				case CMP:
				case CMPU:
					opd1 = new OperandVar(seminsts.d());
					{
						Operand *opd21 = new OperandVar(seminsts.a());
						Operand *opd22 = new OperandVar(seminsts.b());
						opd2 = new OperandArithExpr(ARITHOPR_CMP, *opd21, *opd22);
					}
					break;
				case ADD:
					opd1 = new OperandVar(seminsts.d());
					{
						Operand *opd21 = new OperandVar(seminsts.a());
						Operand *opd22 = new OperandVar(seminsts.b());
						opd2 = new OperandArithExpr(ARITHOPR_ADD, *opd21, *opd22);
					}
					break;
				case SUB:
					opd1 = new OperandVar(seminsts.d());
					{
						Operand *opd21 = new OperandVar(seminsts.a());
						Operand *opd22 = new OperandVar(seminsts.b());
						opd2 = new OperandArithExpr(ARITHOPR_SUB, *opd21, *opd22);
					}
					break;
				case SHL: // TODO test
					DBG(COLOR_IRed << "Untested operand running!")
					opd1 = new OperandVar(seminsts.d());
					{
						Operand *opd21 = new OperandVar(seminsts.a());
						Operand *opd22 = new OperandConst(1 << seminsts.b()); // 2^b
						opd2 = new OperandArithExpr(ARITHOPR_MUL, *opd21, *opd22);
					}
					break;
				case SHR: // TODO test (is this really legit?)
				case ASR: // TODO test
					DBG(COLOR_IRed << "Untested operand running!")
					opd1 = new OperandVar(seminsts.d());
					{
						Operand *opd21 = new OperandVar(seminsts.a());
						Operand *opd22 = new OperandConst(1 << seminsts.b()); // 2^b
						opd2 = new OperandArithExpr(ARITHOPR_DIV, *opd21, *opd22);
					}
					break;
				case NEG: // TODO test
					DBG(COLOR_IRed << "Untested operand running!")
					opd1 = new OperandVar(seminsts.d());
					{
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
						Operand *opd21 = new OperandVar(seminsts.a());
						Operand *opd22 = new OperandVar(seminsts.b());
						opd2 = new OperandArithExpr(ARITHOPR_MUL, *opd21, *opd22);
					}
					break;
				case DIVU:
				case DIV:
					opd1 = new OperandVar(seminsts.d());
					{
						Operand *opd21 = new OperandVar(seminsts.a());
						Operand *opd22 = new OperandVar(seminsts.b());
						opd2 = new OperandArithExpr(ARITHOPR_DIV, *opd21, *opd22);
					}
				case MODU:
				case MOD:
					opd1 = new OperandVar(seminsts.d());
					{
						Operand *opd21 = new OperandVar(seminsts.a());
						Operand *opd22 = new OperandVar(seminsts.b());
						opd2 = new OperandArithExpr(ARITHOPR_MOD, *opd21, *opd22);
					}
				// case SPEC:		// special instruction (d: code, cst: sub-code)
					// Invalidate the whole local labelled pred list in that case...
				default:
					make_pred = false;
			}
			if(make_pred)
			{
				DBG(COLOR_IGre << "> " << Predicate(opr, *opd1, *opd2))
				pred_list += Predicate(opr, *opd1, *opd2);
			}
		}
	}
	return pred_list;
}
