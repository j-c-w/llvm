#include "llvm/Constraints/CustomPasses.hpp"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

class FlattenPass : public llvm::ModulePass
{
public:
    static char ID;

    FlattenPass() : llvm::ModulePass(ID) {}

    bool runOnModule(llvm::Module& module) override
    {
        //Remove Premature GEP instructions
        for(llvm::Function& function : module.getFunctionList())
        {
            std::map<llvm::Value*,llvm::Value*> replacement_table;

            for(llvm::BasicBlock& block : function.getBasicBlockList())
            {
                for(llvm::Instruction& instruction : block.getInstList())
                {
                    if(auto gep_inst = llvm::dyn_cast<llvm::GetElementPtrInst>(&instruction))
                    {
                        std::vector<llvm::Value*> reverse_index_vector;
                        for(unsigned i = 1; i < gep_inst->getNumOperands(); i++)
                        {
                            reverse_index_vector.push_back(gep_inst->getOperand(gep_inst->getNumOperands()-i));
                        }

                        while(auto gep_origin = llvm::dyn_cast<llvm::GetElementPtrInst>(gep_inst->getOperand(0)))
                        {
                            auto first_arg  = gep_origin->getOperand(gep_origin->getNumOperands()-1);
                            auto second_arg = reverse_index_vector.back();

                            auto first_const_arg  = llvm::dyn_cast<llvm::ConstantInt>(first_arg);
                            auto second_const_arg = llvm::dyn_cast<llvm::ConstantInt>(second_arg);

                            llvm::Value* summed_index = nullptr;

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
                                summed_index = llvm::BinaryOperator::Create(llvm::Instruction::Add,
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

                            auto* new_gep_instr = llvm::GetElementPtrInst::Create(gep_inst->getSourceElementType(),
                                                                                  gep_inst->getOperand(0),
                                                                                  reverse_index_vector,
                                                                                  "", &instruction);

                            replacement_table[&instruction] = new_gep_instr;
                        }
                    }
                }
            }

            for(llvm::BasicBlock& block : function.getBasicBlockList())
            {
                for(llvm::Instruction& instruction : block.getInstList())
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
        for(llvm::Function& function : module.getFunctionList())
        {
            std::map<llvm::Value*,llvm::Value*> replacement_table;

            for(llvm::BasicBlock& block : function.getBasicBlockList())
            {
                for(llvm::Instruction& instruction : block.getInstList())
                {
                    if(auto gep_instr = llvm::dyn_cast<llvm::GetElementPtrInst>(&instruction))
                    {
                        auto source_element_type = gep_instr->getSourceElementType();
                        unsigned active_index    = 2;

                        auto zero_index = llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.getContext()), 0);

                        std::vector<llvm::Value*> zero_index_vector;
                        zero_index_vector.push_back(zero_index);

                        auto* merged_index = gep_instr->getOperand(1);

                        while(auto array_type = llvm::dyn_cast<llvm::ArrayType>(source_element_type))
                        {
                            merged_index = llvm::BinaryOperator::Create(llvm::Instruction::Mul, merged_index,
                                                     llvm::ConstantInt::get(merged_index->getType(), 
                                                                            array_type->getNumElements()),
                                                                             "", &instruction);

                            if(active_index < gep_instr->getNumOperands() &&
                               gep_instr->getOperand(active_index)->getType() == merged_index->getType())
                            {
                                merged_index = llvm::BinaryOperator::Create(llvm::Instruction::Add, merged_index,
                                                                            gep_instr->getOperand(active_index),
                                                                            "", &instruction);
                            }
                            active_index++;

                            zero_index_vector.push_back(zero_index);
                   
                            source_element_type = array_type->getElementType();
                        }

                        if(zero_index_vector.size() > 1)
                        {
                            auto first_cast = llvm::GetElementPtrInst::Create(gep_instr->getSourceElementType(),
                                                                              gep_instr->getOperand(0), zero_index_vector,
                                                                              "", &instruction);

                            std::vector<llvm::Value*> new_indizes;
                            new_indizes.push_back(merged_index);

                            for(unsigned i = active_index; i < gep_instr->getNumOperands(); i++)
                                new_indizes.push_back(gep_instr->getOperand(i));

                            auto flat_gep_inst = llvm::GetElementPtrInst::Create(source_element_type, first_cast,
                                                                                 new_indizes, "", &instruction);

                            if(flat_gep_inst->getType() == gep_instr->getType())
                            {
                                replacement_table[&instruction] = flat_gep_inst;
                            }
                            else
                            {
                                auto replace_instr = new llvm::BitCastInst(flat_gep_inst, gep_instr->getType(),
                                                                           "", &instruction);

                                replacement_table[&instruction] = replace_instr;
                            }
                        }
                    }
                }
            }

            for(llvm::BasicBlock& block : function.getBasicBlockList())
            {
                for(llvm::Instruction& instruction : block.getInstList())
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
        for(llvm::Function& function : module.getFunctionList())
        {
            std::map<llvm::Value*,llvm::Value*> replacement_table;

            for(llvm::BasicBlock& block : function.getBasicBlockList())
            {
                for(llvm::Instruction& instruction : block.getInstList())
                {
                    if(auto gep_instr = llvm::dyn_cast<llvm::GetElementPtrInst>(&instruction))
                    {
                        std::vector<std::vector<llvm::Value*>> index_groups;

                        index_groups.emplace_back(1, gep_instr->getOperand(1));

                        auto zero_index = llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.getContext()), 0);

                        for(unsigned i = 2; i < gep_instr->getNumOperands(); i++)
                        {
                            if(llvm::dyn_cast<llvm::ConstantInt>(gep_instr->getOperand(i)))
                            {
                                index_groups.back().push_back(gep_instr->getOperand(i));
                            }
                            else
                            {
                                index_groups.back().push_back(zero_index);
                                index_groups.push_back(std::vector<llvm::Value*>(1, gep_instr->getOperand(i)));
                            }
                        }

                        if(index_groups.size() > 1)
                        {
                            llvm::Value* last_gep = gep_instr->getOperand(0);

                            for(unsigned i = 0; i < index_groups.size(); i++)
                            {
                                if(auto pointer_type = llvm::dyn_cast<llvm::PointerType>(last_gep->getType()))
                                {
                                    last_gep = llvm::GetElementPtrInst::Create(pointer_type->getElementType(),
                                                                               last_gep, index_groups[i],
                                                                               "", &instruction);
                                }
                            }

                            replacement_table[gep_instr] = last_gep;
                        }
                    }
                }
            }

            for(llvm::BasicBlock& block : function.getBasicBlockList())
            {
                for(llvm::Instruction& instruction : block.getInstList())
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
};

char FlattenPass::ID = 0;

llvm::ModulePass *llvm::createFlattenPass() {
  return new FlattenPass();
}
