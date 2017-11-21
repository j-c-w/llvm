#include "llvm/Constraints/CustomPasses.hpp"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

using namespace llvm;

class ResearchFixOrToAdd : public BasicBlockPass
{
public:
    static char ID;

    ResearchFixOrToAdd() : BasicBlockPass(ID) {}

    bool runOnBasicBlock(BasicBlock& block) override;
};

bool ResearchFixOrToAdd::runOnBasicBlock(BasicBlock& block)
{
    std::vector<std::pair<Instruction*,Instruction*>> replacements;

    for(Instruction& instruction : block.getInstList())
    {
        Instruction* shl_inst;
        ConstantInt* or_const_arg;
        ConstantInt* shl_const_arg;
        if(instruction.getOpcode() == Instruction::Or &&
           (shl_inst = dyn_cast<Instruction>(instruction.getOperand(0))) &&
           shl_inst->getOpcode() == Instruction::Shl &&
           (or_const_arg = dyn_cast<ConstantInt>(shl_inst->getOperand(1))) &&
           (shl_const_arg = dyn_cast<ConstantInt>(shl_inst->getOperand(1))) &&
           or_const_arg->getZExtValue() < 1<<shl_const_arg->getZExtValue())
        {
            auto new_inst = BinaryOperator::Create(Instruction::Add, shl_inst, or_const_arg);
            replacements.emplace_back(&instruction, new_inst);
        }
    }

    for(auto entry : replacements)
    {
        ReplaceInstWithInst(entry.first, entry.second);
    }

    return false;
}

char ResearchFixOrToAdd::ID = 0;

static RegisterPass<ResearchFixOrToAdd> X("research-fix-or-to-add", "Research fix or to add", false, false);

BasicBlockPass *llvm::createResearchFixOrToAddPass() {
  return new ResearchFixOrToAdd();
}
