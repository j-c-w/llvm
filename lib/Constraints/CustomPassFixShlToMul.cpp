#include "llvm/Constraints/CustomPasses.hpp"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

using namespace llvm;

class ResearchFixShlToMul : public BasicBlockPass
{
public:
    static char ID;

    ResearchFixShlToMul() : BasicBlockPass(ID) {}

    bool runOnBasicBlock(BasicBlock& module) override;
};

bool ResearchFixShlToMul::runOnBasicBlock(BasicBlock& block)
{
    std::vector<std::pair<Instruction*,Instruction*>> replacements;

    for(Instruction& instruction : block.getInstList())
    {
        ConstantInt* const_arg;
        if(instruction.getOpcode() == Instruction::Shl &&
           (const_arg = dyn_cast<ConstantInt>(instruction.getOperand(1))))
        {
            auto new_inst = BinaryOperator::Create(Instruction::Mul, instruction.getOperand(0),
                                                   ConstantInt::get(const_arg->getType(),
                                                                    1<<const_arg->getZExtValue()));
            replacements.emplace_back(&instruction, new_inst);
        }
    }

    for(auto entry : replacements)
    {
        ReplaceInstWithInst(entry.first, entry.second);
    }

    return false;
}

char ResearchFixShlToMul::ID = 0;

static RegisterPass<ResearchFixShlToMul> X("research-fix-shl-to-mul", "Research fix shl to mul", false, false);

BasicBlockPass *llvm::createResearchFixShlToMulPass() {
  return new ResearchFixShlToMul();
}
