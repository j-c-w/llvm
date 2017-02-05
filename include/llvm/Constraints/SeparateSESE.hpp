#ifndef _SEPARATE_SESE_HPP_
#define _SEPARATE_SESE_HPP_
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Type.h"
#include <vector>
#include <unordered_set>
#include <deque>
#include <iostream>
#include "llvm/Support/raw_os_ostream.h"

/* This class generates a new llvm::Function from a SESE region in an existing llvm::Function. */
class SESEFunction
{
public:
    SESEFunction(llvm::Function& function, llvm::Value* begin, llvm::Value* successor);

    void aggressive_dead_code_elim(std::vector<llvm::Instruction*> keep_values);

    llvm::Function* make_function();

    llvm::Instruction* get_instr(llvm::Instruction* in) { return instr_frwd_transl[in]; }

    void transform_forw(std::vector<std::pair<std::string,llvm::Value*>>& solution)
    {
        for(auto& entry : solution)
        {
            if(entry.second != nullptr)
            {
                auto find_it = values_frwd_transl.find(entry.second);

                if(find_it != values_frwd_transl.end())
                {
                    entry.second = find_it->second;
                }
                else
                {
                    entry.second = nullptr;
                }
            }
        }
    }

private:
    llvm::Function& old_function;

    std::vector<llvm::BasicBlock*> new_block_order;

    std::unordered_map<llvm::BasicBlock*,llvm::BasicBlock*>   block_frwd_transl;
    std::unordered_map<llvm::BasicBlock*,llvm::BasicBlock*>   block_back_transl;
    std::unordered_map<llvm::Instruction*,llvm::Instruction*> instr_frwd_transl;
    std::unordered_map<llvm::Instruction*,llvm::Instruction*> instr_back_transl;

    std::unordered_map<llvm::Value*,llvm::Value*> values_frwd_transl;
    std::unordered_map<llvm::Value*,llvm::Value*> values_back_transl;
};

// This requires blocked(begin->successor->begin)
SESEFunction::SESEFunction(llvm::Function& f, llvm::Value* begin, llvm::Value* successor) : old_function(f)
{
    llvm::BasicBlock* begin_block     = nullptr;
    llvm::BasicBlock* successor_block = nullptr;

    if(auto begin_inst = llvm::dyn_cast<llvm::Instruction>(begin))
        begin_block = begin_inst->getParent();

    if(auto successor_inst = llvm::dyn_cast<llvm::Instruction>(successor))
        successor_block = successor_inst->getParent();

    if(    begin_block == nullptr ||     begin_block->getParent() != &old_function ||
       successor_block == nullptr || successor_block->getParent() != &old_function) return;

    // set up graph traversal temporaries
    std::unordered_set<llvm::BasicBlock*> visited_blocks;
    std::vector<llvm::BasicBlock*>        block_queue;
    unsigned                              queue_front = 0;

    visited_blocks.emplace(begin_block);
    block_queue.push_back(begin_block);

    // search for all relevant blocks
    while(queue_front < block_queue.size())
    {
        llvm::TerminatorInst* term_inst = block_queue[queue_front++]->getTerminator();

        for(unsigned i = 0; i < term_inst->getNumOperands(); i++)
        {
            if(llvm::BasicBlock* target = llvm::dyn_cast<llvm::BasicBlock>(term_inst->getOperand(i)))
            {
                if(target != successor_block && visited_blocks.find(target) == visited_blocks.end())
                {
                    visited_blocks.emplace(target);
                    block_queue.push_back(target);
                }
            }
        }
    }

    // Clone everything
    for(llvm::BasicBlock* old_block : visited_blocks)
    {
        llvm::BasicBlock* new_block = llvm::BasicBlock::Create(old_function.getContext(), old_block->getName());

        block_frwd_transl[old_block] = new_block;
        block_back_transl[new_block] = old_block;

        bool add_instructions = (old_block != begin_block);

        for(llvm::Instruction& old_instr : old_block->getInstList())
        {
            if(begin == &old_instr) add_instructions = true;

            if(add_instructions)
            {
                llvm::Instruction* new_instr = old_instr.clone();

                instr_frwd_transl[&old_instr] = new_instr;
                instr_back_transl[new_instr]  = &old_instr;
                values_frwd_transl[&old_instr] = new_instr;
                values_back_transl[new_instr]  = &old_instr;

                new_block->getInstList().push_back(new_instr);
            }
        }
    }

    // Set new block order as close as possible to old block order.
    new_block_order.push_back(block_frwd_transl[begin_block]);
    for(llvm::BasicBlock& block : old_function.getBasicBlockList())
    {
        if(&block != begin_block)
        {
            auto find_it = block_frwd_transl.find(&block);

            if(find_it != block_frwd_transl.end())
            {
                new_block_order.push_back(find_it->second);
            }
        }
    }

    // Add new finalizer block
    llvm::BasicBlock* final_block = llvm::BasicBlock::Create(old_function.getContext());
    final_block->getInstList().push_back(llvm::ReturnInst::Create(old_function.getContext()));
    new_block_order.push_back(final_block);

    // Reconnect the operands
    for(llvm::BasicBlock* block : new_block_order)
    {
        for(llvm::Instruction& instr : block->getInstList())
        {
            for(unsigned i = 0; i < instr.getNumOperands(); i++)
            {
                if(auto cast_block = llvm::dyn_cast<llvm::BasicBlock>(instr.getOperand(i)))
                {
                    auto find_it = block_frwd_transl.find(cast_block);

                    if(find_it != block_frwd_transl.end())
                    {
                        instr.setOperand(i, find_it->second);
                    }
                    else
                    {
                        instr.setOperand(i, final_block);
                    }
                }
                else if(auto cast_instr = llvm::dyn_cast<llvm::Instruction>(instr.getOperand(i)))
                {
                    auto find_it = instr_frwd_transl.find(cast_instr);

                    if(find_it != instr_frwd_transl.end())
                    {
                        instr.setOperand(i, find_it->second);
                    }
                    else
                    {
                        values_frwd_transl.emplace(instr.getOperand(i), instr.getOperand(i));
                        values_back_transl.emplace(instr.getOperand(i), instr.getOperand(i));
//                        instr.setOperand(i, llvm::UndefValue::get(cast_instr->getType()));
                    }
                }
                else if(!llvm::isa<llvm::Constant>(instr.getOperand(i)))
                {
                    auto find_it = values_frwd_transl.find(instr.getOperand(i));

                    if(find_it != values_frwd_transl.end())
                    {
                        instr.setOperand(i, find_it->second);
                    }
                    else
                    {
                        values_frwd_transl.emplace(instr.getOperand(i), instr.getOperand(i));
                        values_back_transl.emplace(instr.getOperand(i), instr.getOperand(i));
//                        instr.setOperand(i, llvm::UndefValue::get(instr.getOperand(i)->getType()));
                    }
                }
            }
        }
    }

    // Insert early return instructions were possible
    for(llvm::BasicBlock* block : new_block_order)
    {
        if(llvm::BranchInst* branch_cast = llvm::dyn_cast<llvm::BranchInst>(block->getTerminator()))
        {
            if(branch_cast->getNumOperands() == 1 &&
               branch_cast->getOperand(0) == final_block)
            {
                branch_cast->eraseFromParent();
                llvm::ReturnInst::Create(old_function.getContext(), block);
            }
        }
    }

    // Erase final_block if unnecessary
    if(final_block->getNumUses() == 0)
    {
        new_block_order.pop_back();
        delete final_block;
    }

    // Reconnect PHI Nodes
    for(llvm::BasicBlock* block : new_block_order)
    {
        for(llvm::Instruction& instr : block->getInstList())
        {
            if(auto cast_phi = llvm::dyn_cast<llvm::PHINode>(&instr))
            {
                for(unsigned i = 0; i < cast_phi->getNumIncomingValues(); i++)
                {
                    auto find_it = block_frwd_transl.find(cast_phi->getIncomingBlock(i));

                    if(find_it != block_frwd_transl.end())
                    {
                        cast_phi->setIncomingBlock(i, find_it->second);
                    }
                    else
                    {
                        cast_phi->setIncomingBlock(i, final_block);
                    }
                }
            }
        }
    }
}

void SESEFunction::aggressive_dead_code_elim(std::vector<llvm::Instruction*> keep_instructions)
{
    std::vector<llvm::Instruction*>        keep_queue;
    std::unordered_set<llvm::Instruction*> keep_set;

    for(llvm::Instruction* instr : keep_instructions)
    {
        auto find_it = instr_frwd_transl.find(instr);

        if(find_it != instr_frwd_transl.end())
        {
            auto emplace_result = keep_set.emplace(find_it->second);

            if(emplace_result.second)
            {
                keep_queue.push_back(find_it->second);
            }
        }
    }

    for(llvm::BasicBlock* block : new_block_order)
    {
        auto emplace_result = keep_set.emplace(block->getTerminator());

        if(emplace_result.second)
        {
            keep_queue.push_back(block->getTerminator());
        }
    }

    unsigned queue_begin = 0;
    while(queue_begin < keep_queue.size())
    {
        llvm::Instruction* queue_head = keep_queue[queue_begin++];

        keep_set.emplace(queue_head);

        for(unsigned i = 0; i < queue_head->getNumOperands(); i++)
        {
            if(llvm::Instruction* inst_cast = llvm::dyn_cast<llvm::Instruction>(queue_head->getOperand(i)))
            {
                auto emplace_result = keep_set.emplace(inst_cast);

                if(emplace_result.second)
                {
                    keep_queue.push_back(inst_cast);
                }
            }
        }
    }

    std::vector<llvm::Instruction*> delete_instructions;

    for(llvm::BasicBlock* block : new_block_order)
    {
        for(llvm::Instruction& instr : block->getInstList())
        {
            if(keep_set.find(&instr) == keep_set.end())
            {
                delete_instructions.push_back(&instr);
            }
        }
    }

    for(llvm::Instruction* instr : delete_instructions)
    {
        instr->eraseFromParent();
    }
}

llvm::Function* SESEFunction::make_function()
{
    std::vector<llvm::Value*> arguments;

    // Create function object
    std::vector<llvm::Type*> argument_types;
    for(unsigned i = 0; i < arguments.size(); i++)
    {
        argument_types.push_back(arguments[i]->getType());
    }

    llvm::Type* return_type = llvm::Type::getVoidTy(old_function.getContext());

    llvm::FunctionType* function_type = llvm::FunctionType::get(return_type, argument_types, false);

    llvm::Function* new_function = llvm::Function::Create(function_type, llvm::GlobalValue::ExternalLinkage);

    std::vector<llvm::Value*> new_arguments;
    for(llvm::Argument& arg : new_function->getArgumentList())
    {
        new_arguments.push_back(&arg);
    }

    // Fill with basic blocks
    for(llvm::BasicBlock* block : new_block_order)
    {
        new_function->getBasicBlockList().push_back(block);
    }

    // Connect the arguments
    for(unsigned i = 0; i < arguments.size(); i++)
    {
        llvm::Value* transl_argument = nullptr;

        auto find_it = values_frwd_transl.find(arguments[i]);

        if(find_it != values_frwd_transl.end())
        {
            transl_argument = find_it->second;
        }
        else if(auto inst_cast = llvm::dyn_cast<llvm::Instruction>(arguments[i]))
        {
            auto find_it = instr_frwd_transl.find(inst_cast);

            if(find_it != instr_frwd_transl.end())
            {
                transl_argument = find_it->second;
            }
            else
            {
                continue;
            }
        }

        for(llvm::BasicBlock* block : new_block_order)
        {
            for(llvm::Instruction& instr : block->getInstList())
            {
                for(unsigned j = 0; j < instr.getNumOperands(); j++)
                {
                    if(instr.getOperand(j) == transl_argument)
                    {
                        instr.setOperand(j, new_arguments[i]);
                    }
                }
            }
        }
    }

    return new_function;
}

#endif
