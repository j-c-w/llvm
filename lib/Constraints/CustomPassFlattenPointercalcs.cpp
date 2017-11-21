#include "llvm/Constraints/CustomPasses.hpp"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

using namespace llvm;

class ResearchFlattenPointercalcs : public FunctionPass
{
public:
    static char ID;

    ResearchFlattenPointercalcs() : FunctionPass(ID) {}

    bool runOnFunction(Function& function) override;
};

bool ResearchFlattenPointercalcs::runOnFunction(Function& function)
{
    // Flatten array GEP instructions
    std::map<Instruction*,Instruction*> replacements;

    for(BasicBlock& block : function.getBasicBlockList())
    {
        for(Instruction& instruction : block.getInstList())
        {
            if(auto gep_instr = dyn_cast<GetElementPtrInst>(&instruction))
            {
                auto source_element_type = gep_instr->getSourceElementType();
                unsigned active_index    = 2;

                auto zero_index = ConstantInt::get(Type::getInt64Ty(function.getParent()->getContext()), 0);

                std::vector<Value*> zero_index_vector;
                zero_index_vector.push_back(zero_index);

                auto* merged_index = gep_instr->getOperand(1);

                while(auto array_type = dyn_cast<ArrayType>(source_element_type))
                {
                    if(llvm::dyn_cast<llvm::ConstantInt>(merged_index) == nullptr ||
                       llvm::dyn_cast<llvm::ConstantInt>(merged_index)->getSExtValue() != 0)
                    {
                        merged_index = BinaryOperator::Create(Instruction::Mul, merged_index,
                                                 ConstantInt::get(merged_index->getType(), 
                                                                        array_type->getNumElements()),
                                                                         "", &instruction);
                    }

                    if(active_index < gep_instr->getNumOperands() &&
                       gep_instr->getOperand(active_index)->getType() == merged_index->getType())
                    {
                        if(llvm::dyn_cast<llvm::ConstantInt>(merged_index) &&
                           llvm::dyn_cast<llvm::ConstantInt>(merged_index)->getSExtValue() == 0)
                        {
                            merged_index = gep_instr->getOperand(active_index);
                        }
                        else if(llvm::dyn_cast<llvm::ConstantInt>(gep_instr->getOperand(active_index)) == nullptr ||
                                llvm::dyn_cast<llvm::ConstantInt>(gep_instr->getOperand(active_index))->getSExtValue() != 0)
                        {
                            merged_index = BinaryOperator::Create(Instruction::Add, merged_index,
                                                                        gep_instr->getOperand(active_index),
                                                                        "", &instruction);
                        }
                    }
                    active_index++;

                    zero_index_vector.push_back(zero_index);
           
                    source_element_type = array_type->getElementType();
                }

                if(zero_index_vector.size() > 1)
                {
                    auto first_cast = GetElementPtrInst::Create(gep_instr->getSourceElementType(),
                                                                      gep_instr->getOperand(0), zero_index_vector,
                                                                      "", &instruction);

                    std::vector<Value*> new_indizes;
                    new_indizes.push_back(merged_index);

                    for(unsigned i = active_index; i < gep_instr->getNumOperands(); i++)
                        new_indizes.push_back(gep_instr->getOperand(i));

                    auto flat_gep_inst = GetElementPtrInst::Create(source_element_type, first_cast,
                                                                         new_indizes, "", &instruction);

                    if(flat_gep_inst->getType() == gep_instr->getType())
                    {
                        replacements[&instruction] = flat_gep_inst;
                    }
                    else
                    {
                        auto replace_instr = new BitCastInst(flat_gep_inst, gep_instr->getType(),
                                                                   "", &instruction);

                        replacements[&instruction] = replace_instr;
                    }
                }
            }
        }
    }

    for(BasicBlock& block : function.getBasicBlockList())
    {
        for(Instruction& instruction : block.getInstList())
        {
            for(unsigned i = 0; i < instruction.getNumOperands(); i++)
            {
                auto find_it = replacements.find((Instruction*)instruction.getOperand(i));

                if(find_it != replacements.end())
                    instruction.setOperand(i, find_it->second);
            }
        }
    }

    return false;
}

char ResearchFlattenPointercalcs::ID = 0;

static RegisterPass<ResearchFlattenPointercalcs> X("research-flatten-pointercalcs", "Research flatten pointer calculations", false, false);

FunctionPass *llvm::createResearchFlattenPointercalcsPass() {
  return new ResearchFlattenPointercalcs();
}
