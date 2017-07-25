#include "llvm/Constraints/CustomPasses.hpp"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

using namespace llvm;

class ResearchFlatten : public ModulePass
{
public:
    static char ID;

    ResearchFlatten() : ModulePass(ID) {}

    bool runOnModule(Module& module) override;
};

bool ResearchFlatten::runOnModule(Module& module)
{
    //Remove Premature GEP instructions
    for(Function& function : module.getFunctionList())
    {
        std::map<Value*,Value*> replacement_table;

        for(BasicBlock& block : function.getBasicBlockList())
        {
            for(Instruction& instruction : block.getInstList())
            {
                if(auto gep_inst = dyn_cast<GetElementPtrInst>(&instruction))
                {
                    std::vector<Value*> reverse_index_vector;
                    for(unsigned i = 1; i < gep_inst->getNumOperands(); i++)
                    {
                        reverse_index_vector.push_back(gep_inst->getOperand(gep_inst->getNumOperands()-i));
                    }

                    while(auto gep_origin = dyn_cast<GetElementPtrInst>(gep_inst->getOperand(0)))
                    {
                        auto first_arg  = gep_origin->getOperand(gep_origin->getNumOperands()-1);
                        auto second_arg = reverse_index_vector.back();

                        auto first_const_arg  = dyn_cast<ConstantInt>(first_arg);
                        auto second_const_arg = dyn_cast<ConstantInt>(second_arg);

                        Value* summed_index = nullptr;

                        if(second_const_arg && second_const_arg->getSExtValue() == 0)
                        {
                            summed_index = gep_origin->getOperand(gep_origin->getNumOperands()-1);
                        }
                        else if(first_const_arg && first_const_arg->getSExtValue() == 0)
                        {
                            summed_index = reverse_index_vector.back();
                        }
                        else
                        {
                            summed_index = BinaryOperator::Create(Instruction::Add,
                                                             gep_origin->getOperand(gep_origin->getNumOperands()-1),
                                                             reverse_index_vector.back(), "", &instruction);
                        }

                        reverse_index_vector.back() = summed_index;

                        for(unsigned i = 2; i < gep_origin->getNumOperands(); i++)
                        {
                            reverse_index_vector.push_back(gep_origin->getOperand(gep_origin->getNumOperands()-i));
                        }

                        gep_inst = gep_origin;
                    }

                    if(gep_inst != &instruction)
                    {
                        std::reverse(reverse_index_vector.begin(), reverse_index_vector.end());

                        auto* new_gep_instr = GetElementPtrInst::Create(gep_inst->getSourceElementType(),
                                                                              gep_inst->getOperand(0),
                                                                              reverse_index_vector,
                                                                              "", &instruction);

                        replacement_table[&instruction] = new_gep_instr;
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
                    auto find_it = replacement_table.find(instruction.getOperand(i));

                    if(find_it != replacement_table.end())
                        instruction.setOperand(i, find_it->second);
                }
            }
        }
    }

    //Flatten array GEP instructions
    for(Function& function : module.getFunctionList())
    {
        std::map<Value*,Value*> replacement_table;

        for(BasicBlock& block : function.getBasicBlockList())
        {
            for(Instruction& instruction : block.getInstList())
            {
                if(auto gep_instr = dyn_cast<GetElementPtrInst>(&instruction))
                {
                    auto source_element_type = gep_instr->getSourceElementType();
                    unsigned active_index    = 2;

                    auto zero_index = ConstantInt::get(Type::getInt64Ty(module.getContext()), 0);

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
                            else if(llvm::dyn_cast<llvm::ConstantInt>(gep_instr->getOperand(active_index)) &&
                                    llvm::dyn_cast<llvm::ConstantInt>(gep_instr->getOperand(active_index))->getSExtValue() == 0)
                            {
                                merged_index = merged_index;
                            }
                            else
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
                            replacement_table[&instruction] = flat_gep_inst;
                        }
                        else
                        {
                            auto replace_instr = new BitCastInst(flat_gep_inst, gep_instr->getType(),
                                                                       "", &instruction);

                            replacement_table[&instruction] = replace_instr;
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
                    auto find_it = replacement_table.find(instruction.getOperand(i));

                    if(find_it != replacement_table.end())
                        instruction.setOperand(i, find_it->second);
                }
            }
        }
    }

    //Split so that only first index can be dynamic
    for(Function& function : module.getFunctionList())
    {
        std::map<Value*,Value*> replacement_table;

        for(BasicBlock& block : function.getBasicBlockList())
        {
            for(Instruction& instruction : block.getInstList())
            {
                if(auto gep_instr = dyn_cast<GetElementPtrInst>(&instruction))
                {
                    std::vector<std::vector<Value*>> index_groups;

                    index_groups.emplace_back(1, gep_instr->getOperand(1));

                    auto zero_index = ConstantInt::get(Type::getInt64Ty(module.getContext()), 0);

                    for(unsigned i = 2; i < gep_instr->getNumOperands(); i++)
                    {
                        if(dyn_cast<ConstantInt>(gep_instr->getOperand(i)))
                        {
                            index_groups.back().push_back(gep_instr->getOperand(i));
                        }
                        else
                        {
                            index_groups.back().push_back(zero_index);
                            index_groups.push_back(std::vector<Value*>(1, gep_instr->getOperand(i)));
                        }
                    }

                    if(index_groups.size() > 1)
                    {
                        Value* last_gep = gep_instr->getOperand(0);

                        for(unsigned i = 0; i < index_groups.size(); i++)
                        {
                            if(auto pointer_type = dyn_cast<PointerType>(last_gep->getType()))
                            {
                                last_gep = GetElementPtrInst::Create(pointer_type->getElementType(),
                                                                           last_gep, index_groups[i],
                                                                           "", &instruction);
                            }
                        }

                        replacement_table[gep_instr] = last_gep;
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
                    auto find_it = replacement_table.find(instruction.getOperand(i));

                    if(find_it != replacement_table.end())
                        instruction.setOperand(i, find_it->second);
                }
            }
        }
    }

    return false;
}

char ResearchFlatten::ID = 0;

//INITIALIZE_PASS_BEGIN(ResearchFlatten, "research-flatten", "Research flatten", false, false)
//INITIALIZE_PASS_END(ResearchFlatten, "research-flatten", "Research flatten", false, false)

static RegisterPass<ResearchFlatten> X("research-flatten", "Research flatten", false, false);

ModulePass *llvm::createResearchFlattenPass() {
  return new ResearchFlatten();
}
