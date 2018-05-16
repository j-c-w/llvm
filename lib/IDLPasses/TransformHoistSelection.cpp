#include "llvm/IDLPasses/Transforms.hpp"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"
#include <sstream>
#include <string>
#include <vector>

using namespace llvm;

void transform_hoistselect_pattern(Function& function, std::map<std::string,Value*> solution)
{
    SelectInst*        select_inst  = dyn_cast<SelectInst>       (solution["select"]);
    GetElementPtrInst* gep_inst1    = dyn_cast<GetElementPtrInst>(solution["input1"]);
    GetElementPtrInst* gep_inst2    = dyn_cast<GetElementPtrInst>(solution["input2"]);

    if(gep_inst1->getNumOperands() == 2 && gep_inst2->getNumOperands() == 2 &&
       gep_inst1->getOperand(0)->getType() == gep_inst2->getOperand(0)->getType() &&
       gep_inst1->getOperand(1)->getType() == gep_inst2->getOperand(1)->getType())
    {
        BasicBlock::iterator insert_point(select_inst);
        IRBuilder<> builder(select_inst->getParent(), insert_point);

        ReplaceInstWithValue(select_inst->getParent()->getInstList(), insert_point,
            builder.CreateBitCast(
                builder.CreateGEP(
                    gep_inst1->getOperand(0),
                    builder.CreateSelect(select_inst->getCondition(),
                                         gep_inst1->getOperand(1),
                                         gep_inst2->getOperand(1))),
                select_inst->getType()));
    }
}



/* Program to apply hoistselect
insert {select}:
%a = Select {input1}, {input2}
%b = GEP {base}, {a}
done

replace {select} with {result}
*/


