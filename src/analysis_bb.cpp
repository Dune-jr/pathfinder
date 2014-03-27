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
		DBG(COLOR_Gre << *insts << COLOR_RCol)
		
		Block block;
		insts->semInsts(block);
		
		
		// Parse semantical instructions
		for(Block::InstIter seminsts(block); seminsts; seminsts++)
		{
			DBG(COLOR_IGre << *seminsts << COLOR_RCol)
			
			switch(opcode opr = seminsts.op())
			{
				case SETI:
					static bool once = true;
					if(once) { once = false;
						OperandVar opd1 = OperandVar(-12);
						DBG(COLOR_BIPur << "&opd1 = " << &opd1 << COLOR_RCol);
						OperandConst opd2 = OperandConst(4);
						pred_list += Predicate(CONDOPR_EQ, opd1, opd2);
					}
					break;
			}
		}
	}
	return pred_list;
}
