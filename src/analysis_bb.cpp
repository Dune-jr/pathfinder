#include <otawa/cfg/BasicBlock.h>
#include <otawa/sem/inst.h>
#include "predicate.h"
#include "debug.h"
#include "analysis.h"

using namespace otawa::sem;

list<Predicate> Analysis::analyzeBB(BasicBlock *bb)
{
	list<Predicate> pred_list;
	
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
					//for(
					//LPList += LabelledPredicate(
					break;
			}
		}
	}
}
