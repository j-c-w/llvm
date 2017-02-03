#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/IR/SymbolTableListTraits.h"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Value.h"
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <vector>

FunctionWrapper::InstructionHashmap FunctionWrapper::get_instruction_hashmap()
{
    InstructionHashmap instr_hash;

    for(unsigned i = 0; i < instructions.size(); i++)
    {
        instr_hash[instructions[i]] = i;
    }

    return instr_hash;
}

FunctionWrapper::ValueHashmap FunctionWrapper::get_value_hashmap()
{
    ValueHashmap value_hash;

    for(unsigned i = 0; i < extra_values.size(); i++)
    {
        value_hash[extra_values[i]] = i;
    }

    return value_hash;
}

FunctionWrapper::ValueHashmap FunctionWrapper::gather_non_instruction_values(InstructionHashmap& instr_hash)
{
    ValueHashmap value_hash;

    for(auto instruction : instructions)
    {
        for(unsigned i = 0; i < instruction->getNumOperands(); i++)
        {
            llvm::Value*       value = instruction->getOperand(i);
            llvm::Instruction* instr = llvm::dyn_cast<llvm::Instruction>(value);

            if(instr_hash.find(instr) == instr_hash.end() &&
               value_hash.find(value) == value_hash.end() &&
               llvm::dyn_cast<llvm::BasicBlock>(value) == nullptr)
            {
                value_hash[value] = extra_values.size();
                extra_values.push_back(value);
            }
        }
    }

    return value_hash;
}

void FunctionWrapper::allocate_graphs()
{
    cfg.resize (extra_values.size() + instructions.size());
    rcfg.resize(extra_values.size() + instructions.size());

    for(unsigned i = 0; i < 4; i++)
    {
        ocfg[i].resize (extra_values.size() + instructions.size());
        rocfg[i].resize(extra_values.size() + instructions.size());
    }

    for(unsigned i = 0; i < 4; i++)
    {
        incoming[i].resize (extra_values.size() + instructions.size());
    }

    dfg.resize (extra_values.size() + instructions.size());
    rdfg.resize(extra_values.size() + instructions.size());
    cdg.resize (extra_values.size() + instructions.size());
    rcdg.resize(extra_values.size() + instructions.size());
    pdg.resize (extra_values.size() + instructions.size());
    rpdg.resize(extra_values.size() + instructions.size());

    for(unsigned i = 0; i < 32; i++)
    {
        odfg[i].resize (extra_values.size() + instructions.size());
        rodfg[i].resize(extra_values.size() + instructions.size());
    }
}


void FunctionWrapper::construct_opcodes_from_llvm(InstructionHashmap& instr_hash)
{
    opcodes.resize(extra_values.size() + instructions.size(), 0);

    for(auto instruction : instructions)
    {
        opcodes[extra_values.size() + instr_hash[instruction]] = instruction->getOpcode();
    }
}

void FunctionWrapper::construct_cdg_from_llvm(InstructionHashmap& instr_hash)
{
    for(auto instruction : instructions)
    {
        if(llvm::TerminatorInst* terminator_instr = llvm::dyn_cast<llvm::TerminatorInst>(instruction))
        {
            for(unsigned i = 0; i < terminator_instr->getNumOperands(); i++)
            {
                if(llvm::BasicBlock* target_block = llvm::dyn_cast<llvm::BasicBlock>(terminator_instr->getOperand(i)))
                {
                    for(auto& target_instruction : *target_block)
                    {
                         cdg[extra_values.size() + instr_hash[instruction]].push_back(
                             extra_values.size() + instr_hash[&target_instruction]);
                        rcdg[extra_values.size() + instr_hash[&target_instruction]].push_back(
                             extra_values.size() + instr_hash[instruction]);
                    }
                }
            }
        }
    }
}

void FunctionWrapper::construct_incoming_from_llvm(InstructionHashmap& instr_hash)
{
    for(auto instruction : instructions)
    {
        if(llvm::PHINode* phi_instr = llvm::dyn_cast<llvm::PHINode>(instruction))
        {
            for(unsigned i = 0; i < phi_instr->getNumIncomingValues(); i++)
            {
                incoming[i][extra_values.size() + instr_hash[instruction]].push_back(
                    extra_values.size() + instr_hash[phi_instr->getIncomingBlock(i)->getTerminator()]);
            }
        }
    }
}


void FunctionWrapper::construct_ocfg_from_llvm(InstructionHashmap& instr_hash)
{
    for(auto instruction : instructions)
    {
        if(llvm::BranchInst* branch_inst = llvm::dyn_cast<llvm::BranchInst>(instruction))
        {
            std::vector<llvm::BasicBlock*> successor_list;
            for(unsigned i = 0; i < branch_inst->getNumOperands(); i++)
            {
                if(llvm::BasicBlock* block_cast = llvm::dyn_cast<llvm::BasicBlock>(branch_inst->getOperand(i)))
                {
                    successor_list.push_back(block_cast);
                }
            }

            for(unsigned i = 0; i < successor_list.size(); i++)
            {
                ocfg [i][extra_values.size() + instr_hash[instruction]].push_back(
                         extra_values.size() + instr_hash[&successor_list[i]->front()]);
                rocfg[i][extra_values.size() + instr_hash[&successor_list[i]->front()]].push_back(
                         extra_values.size() + instr_hash[instruction]);
            }
        }
        else if(llvm::SwitchInst* switch_inst = llvm::dyn_cast<llvm::SwitchInst>(instruction))
        {
            std::vector<llvm::BasicBlock*> successor_list;
            for(unsigned i = 0; i < switch_inst->getNumOperands(); i++)
            {
                if(llvm::BasicBlock* block_cast = llvm::dyn_cast<llvm::BasicBlock>(switch_inst->getOperand(i)))
                {
                    successor_list.push_back(block_cast);
                }
            }

            for(unsigned i = 0; i < successor_list.size(); i++)
            {
                ocfg [i][extra_values.size() + instr_hash[instruction]].push_back(
                         extra_values.size() + instr_hash[&successor_list[i]->front()]);
                rocfg[i][extra_values.size() + instr_hash[&successor_list[i]->front()]].push_back(
                         extra_values.size() + instr_hash[instruction]);
            }
        }
        else if(!llvm::isa<llvm::TerminatorInst>(instruction))
        {
            ocfg [0][extra_values.size() + instr_hash[instruction]].push_back(
                     extra_values.size() + instr_hash[instruction] + 1);
            rocfg[0][extra_values.size() + instr_hash[instruction] + 1].push_back(
                     extra_values.size() + instr_hash[instruction]);
        }
    }
}

void FunctionWrapper::construct_odfg_from_llvm(InstructionHashmap& instr_hash, ValueHashmap& value_hash)
{
    for(auto instruction : instructions)
    {
        for(unsigned i = 0; i < instruction->getNumOperands(); i++)
        {
            llvm::Value*       value = instruction->getOperand(i);
            llvm::Instruction* instr = llvm::dyn_cast<llvm::Instruction>(value);
            auto               it1   = value_hash.find(value);
            auto               it2   = instr_hash.find(instr);

            if(it1 != value_hash.end())
            {
                odfg [i][it1->second].push_back(extra_values.size() + instr_hash[instruction]);
                rodfg[i][extra_values.size() + instr_hash[instruction]].push_back(it1->second);
            }
            if(it2 != instr_hash.end())
            {
                odfg [i][extra_values.size() + it2->second].push_back(extra_values.size() + instr_hash[instruction]);
                rodfg[i][extra_values.size() + instr_hash[instruction]].push_back(extra_values.size() + it2->second);
            }
        }
    }
}

void FunctionWrapper::assemble_subgraphs()
{
    for(unsigned i = 0; i < extra_values.size() + instructions.size(); i++)
    {
        for(unsigned j = 0; j < 4; j++)
        {
             cfg[i].reserve( cfg[i].size() +  ocfg[j][i].size());
            rcfg[i].reserve(rcfg[i].size() + rocfg[j][i].size());

             cfg[i].insert( cfg[i].end(),  ocfg[j][i].begin(),  ocfg[j][i].end());
            rcfg[i].insert(rcfg[i].end(), rocfg[j][i].begin(), rocfg[j][i].end());
        }

        for(unsigned j = 0; j < 32; j++)
        {
             dfg[i].reserve( dfg[i].size() +  odfg[j][i].size());
            rdfg[i].reserve(rdfg[i].size() + rodfg[j][i].size());

             dfg[i].insert( dfg[i].end(),  odfg[j][i].begin(),  odfg[j][i].end());
            rdfg[i].insert(rdfg[i].end(), rodfg[j][i].begin(), rodfg[j][i].end());
        }
    }
}

void FunctionWrapper::generate_pdg()
{
    for(unsigned i = 0; i < extra_values.size() + instructions.size(); i++)
    {
        pdg[i].reserve(cdg[i].size() + dfg[i].size());
        pdg[i].insert(pdg[i].end(), cdg[i].begin(), cdg[i].end());
        pdg[i].insert(pdg[i].end(), dfg[i].begin(), dfg[i].end());

        rpdg[i].reserve(rcdg[i].size() + rdfg[i].size());
        rpdg[i].insert(rpdg[i].end(), rcdg[i].begin(), rcdg[i].end());
        rpdg[i].insert(rpdg[i].end(), rdfg[i].begin(), rdfg[i].end());
    }
}

void FunctionWrapper::sort_graphs()
{
    for(unsigned i = 0; i < extra_values.size() + instructions.size(); i++)
    {
        std::sort( cfg[i].begin(),  cfg[i].end());
        cfg[i].erase(std::unique(cfg[i].begin(), cfg[i].end()), cfg[i].end());
        std::sort(rcfg[i].begin(), rcfg[i].end());
        rcfg[i].erase(std::unique(rcfg[i].begin(), rcfg[i].end()), rcfg[i].end());

        for(unsigned j = 0; j < 4; j++)
        {
            std::sort( ocfg[j][i].begin(),  ocfg[j][i].end());
            ocfg[j][i].erase(std::unique(ocfg[j][i].begin(), ocfg[j][i].end()), ocfg[j][i].end());
            std::sort(rocfg[j][i].begin(), rocfg[j][i].end());
            rocfg[j][i].erase(std::unique(rocfg[j][i].begin(), rocfg[j][i].end()), rocfg[j][i].end());
        }

        std::sort( dfg[i].begin(),  dfg[i].end());
        dfg[i].erase(std::unique(dfg[i].begin(), dfg[i].end()), dfg[i].end());
        std::sort(rdfg[i].begin(), rdfg[i].end());
        rdfg[i].erase(std::unique(rdfg[i].begin(), rdfg[i].end()), rdfg[i].end());
        std::sort( cdg[i].begin(),  cdg[i].end());
        cdg[i].erase(std::unique(cdg[i].begin(), cdg[i].end()), cdg[i].end());
        std::sort(rcdg[i].begin(), rcdg[i].end());
        rcdg[i].erase(std::unique(rcdg[i].begin(), rcdg[i].end()), rcdg[i].end());
        std::sort( pdg[i].begin(),  pdg[i].end());
        pdg[i].erase(std::unique(pdg[i].begin(), pdg[i].end()), pdg[i].end());
        std::sort(rpdg[i].begin(), rpdg[i].end());
        rpdg[i].erase(std::unique(rpdg[i].begin(), rpdg[i].end()), rpdg[i].end());
 
        for(unsigned j = 0; j < 32; j++)
        {
            std::sort( odfg[j][i].begin(),  odfg[j][i].end());
            odfg[j][i].erase(std::unique(odfg[j][i].begin(), odfg[j][i].end()), odfg[j][i].end());
            std::sort(rodfg[j][i].begin(), rodfg[j][i].end());
            rodfg[j][i].erase(std::unique(rodfg[j][i].begin(), rodfg[j][i].end()), rodfg[j][i].end());
        }
    }
}

void FunctionWrapper::make_value_undefined(unsigned a)
{
    extra_values[a] = llvm::UndefValue::get(extra_values[a]->getType());
}

void FunctionWrapper::construct_llvm_values_from_superset(FunctionWrapper& superset,
                                                          const std::vector<unsigned>& value_indizes,
                                                          const std::vector<unsigned>& instr_indizes)
{
    for(auto index : value_indizes)
    {
        if(index < superset.extra_values.size())
            extra_values.push_back(superset.extra_values[index]);
        else if(index < superset.extra_values.size() + superset.instructions.size())
            extra_values.push_back(superset.instructions[index - superset.extra_values.size()]);
        else
            printf("error type A!\n");
    }

    for(auto index : instr_indizes)
    {
        if(index < superset.extra_values.size())
            printf("error type B!\n");
        else if(index < superset.extra_values.size() + superset.instructions.size())
            instructions.push_back(superset.instructions[index - superset.extra_values.size()]);
        else
            printf("error type C!\n");
    }
}

void FunctionWrapper::construct_opcodes_from_superset(FunctionWrapper& superset,
                                                      const std::vector<unsigned>& value_indizes,
                                                      const std::vector<unsigned>& instr_indizes)
{
    for(auto index : value_indizes)
    {
        opcodes.push_back(superset.opcodes[index]);
    }

    for(auto index : instr_indizes)
    {
        opcodes.push_back(superset.opcodes[index]);
    }
}


void FunctionWrapper::construct_cdg_from_superset(FunctionWrapper& superset,
                                                  const std::vector<unsigned>& value_indizes,
                                                  const std::vector<unsigned>& instr_indizes)
{
    for(unsigned i = 0; i < instr_indizes.size(); i++)
    {
        for(unsigned j = 0; j < superset.rcdg[instr_indizes[i]].size(); j++)
        {
            unsigned control_dependence = superset.rcdg[instr_indizes[i]][j];

            auto find_it1 = std::lower_bound(value_indizes.begin(), value_indizes.end(), control_dependence);
            auto find_it2 = std::lower_bound(instr_indizes.begin(), instr_indizes.end(), control_dependence);

            if(find_it1 != value_indizes.end())
            {
                unsigned new_parameter = std::distance(value_indizes.begin(), find_it1);

                 cdg[new_parameter].push_back(value_indizes.size() + i);
                rcdg[value_indizes.size() + i].push_back(new_parameter);
            }
            else if(find_it2 != instr_indizes.end())
            {
                unsigned new_parameter = std::distance(instr_indizes.begin(), find_it2);

                 cdg[value_indizes.size() + new_parameter].push_back(value_indizes.size() + i);
                rcdg[value_indizes.size() + i].push_back(value_indizes.size() + new_parameter);
            }
            else
            {
                printf("not a valid sub set of instructions!\n");
            }
        }
    }
}

// Open ends are denoted by self-references
void FunctionWrapper::construct_ocfg_from_superset(FunctionWrapper& superset,
                                                   const std::vector<unsigned>& value_indizes,
                                                   const std::vector<unsigned>& instr_indizes,
                                                   unsigned cfg_successor)
{
    std::vector<unsigned> status(superset.extra_values.size() + superset.instructions.size(), 0);
    std::vector<unsigned> queue (superset.extra_values.size() + superset.instructions.size(), 0);

    for(unsigned index : instr_indizes)
    {
        status[index] = 1;
    }

    for(unsigned i = 0; i < instr_indizes.size(); i++)
    {
        for(unsigned j = 0; j < 4 && superset.ocfg[j][instr_indizes[i]].size() == 1; j++)
        {
            unsigned child = superset.ocfg[j][instr_indizes[i]][0];

            unsigned* queue_front = &queue[0];
            unsigned* queue_back = &queue[0];

            if(status[child] == 1)
            {
                status[child] = 2;
            }
            else if(status[child] == 0 && child != cfg_successor)
            {
                *(queue_back++) = child;
                status[child]   = 3;
            }

            while(queue_front != queue_back)
            {
                unsigned element = *(queue_front++);

                for(unsigned child : superset.cfg[element])
                {
                    if(status[child] == 1)
                    {
                        status[child] = 2;
                    }
                    else if(status[child] == 0 && child != cfg_successor)
                    {
                        *(queue_back++) = child;
                        status[child]   = 3;
                   }
                }
            }

            std::vector<unsigned> successors;

            for(unsigned i = 0; i < status.size(); i++)
            {
                if(status[i] == 2)
                {
                    successors.push_back(i);
                    status[i] = 1;
                }
                else if(status[i] == 3)
                {
                    status[i] = 0;
                }
            }

            if(successors.size() > 1)
            {
                printf("The pdg is corrupted!");
            }
            else if(successors.size() == 0)
            {
                ocfg[j][value_indizes.size() + i].push_back(value_indizes.size() + i);
            }
            else
            {
                auto find_it = std::lower_bound(instr_indizes.begin(), instr_indizes.end(), successors[0]);

                unsigned new_child = std::distance(instr_indizes.begin(), find_it);

                 ocfg[j][value_indizes.size() + i].push_back(value_indizes.size() + new_child);
                rocfg[j][value_indizes.size() + new_child].push_back(value_indizes.size() + i);
            }
        }
    }
}

// Open ends are denoted by self-references
void FunctionWrapper::construct_incoming_from_superset(FunctionWrapper& superset,
                                                       const std::vector<unsigned>& value_indizes,
                                                       const std::vector<unsigned>& instr_indizes)
{
    std::vector<unsigned> status(superset.extra_values.size() + superset.instructions.size(), 0);
    std::vector<unsigned> queue (superset.extra_values.size() + superset.instructions.size(), 0);

    for(unsigned index : instr_indizes)
    {
        status[index] = 1;
    }

    for(unsigned i = 0; i < instr_indizes.size(); i++)
    {
        for(unsigned j = 0; j < 4 && superset.incoming[j][instr_indizes[i]].size() == 1; j++)
        {
            unsigned child = superset.incoming[j][instr_indizes[i]][0];

            unsigned* queue_front = &queue[0];
            unsigned* queue_back = &queue[0];

            if(status[child] == 1)
            {
                status[child] = 2;
            }
            else if(status[child] == 0)
            {
                *(queue_back++) = child;
                status[child]   = 3;
            }

            while(queue_front != queue_back)
            {
                unsigned element = *(queue_front++);

                for(unsigned child : superset.rcfg[element])
                {
                    if(status[child] == 1)
                    {
                        status[child] = 2;
                    }
                    else if(status[child] == 0)
                    {
                        *(queue_back++) = child;
                        status[child]   = 3;
                   }
                }
            }

            std::vector<unsigned> successors;

            for(unsigned i = 0; i < status.size(); i++)
            {
                if(status[i] == 2)
                {
                    successors.push_back(i);
                    status[i] = 1;
                }
                else if(status[i] == 3)
                {
                    status[i] = 0;
                }
            }

            if(successors.size() > 1)
            {
                fprintf(stderr, "The pdg is corrupted!");
            }
            else if(successors.size() == 0)
            {
                incoming[j][value_indizes.size() + i].push_back(value_indizes.size() + i);
            }
            else
            {
                auto find_it = std::lower_bound(instr_indizes.begin(), instr_indizes.end(), successors[0]);

                unsigned new_child = std::distance(instr_indizes.begin(), find_it);

                incoming[j][value_indizes.size() + i].push_back(value_indizes.size() + new_child);
            }
        }
    }
}

void FunctionWrapper::construct_odfg_from_superset(FunctionWrapper& superset,
                                                   const std::vector<unsigned>& value_indizes,
                                                   const std::vector<unsigned>& instr_indizes)
{
    for(unsigned i = 0; i < instr_indizes.size(); i++)
    {
        for(unsigned j = 0; j < 32 && superset.rodfg[j][instr_indizes[i]].size() == 1; j++)
        {
            unsigned parameter = superset.rodfg[j][instr_indizes[i]][0];

            auto find_it1 = std::lower_bound(value_indizes.begin(), value_indizes.end(), parameter);
            auto find_it2 = std::lower_bound(instr_indizes.begin(), instr_indizes.end(), parameter);

            if(find_it1 != value_indizes.end())
            {
                unsigned new_parameter = std::distance(value_indizes.begin(), find_it1);

                 odfg[j][new_parameter].push_back(value_indizes.size() + i);
                rodfg[j][value_indizes.size() + i].push_back(new_parameter);
            }
            else if(find_it2 != instr_indizes.end())
            {
                unsigned new_parameter = std::distance(instr_indizes.begin(), find_it2);

                 odfg[j][value_indizes.size() + new_parameter].push_back(value_indizes.size() + i);
                rodfg[j][value_indizes.size() + i].push_back(value_indizes.size() + new_parameter);
            }
            else
            {
                printf("not a valid sub set of instructions!\n");
            }
        }
    }
}

std::vector<llvm::BasicBlock*> FunctionWrapper::rebuild_llvm_structure(llvm::LLVMContext& llvm_context)
{
    for(unsigned i = 0; i < extra_values.size(); i++)
    {
        if(llvm::dyn_cast<llvm::Instruction>(extra_values[i]) ||
           llvm::dyn_cast<llvm::Argument>(extra_values[i]))
        {
            extra_values[i] = llvm::UndefValue::get(extra_values[i]->getType());
        }
    }

    for(unsigned i = 0; i < instructions.size(); i++)
    {
        instructions[i] = instructions[i]->clone();
        for(unsigned j = 0; j < instructions[i]->getNumOperands(); j++)
            instructions[i]->setOperand(j, llvm::UndefValue::get(instructions[i]->getOperand(j)->getType()));
    }

    std::vector<std::vector<unsigned>> basic_blocks;
    for(unsigned i = 0; i < instructions.size(); i++)
    {
        if(rcfg[extra_values.size() + i].size() != 1 ||
           rcfg[extra_values.size() + i][0] == extra_values.size() + i ||
           cfg[rcfg[extra_values.size() + i][0]].size() != 1 ||
           opcodes[rcfg[extra_values.size() + i][0]] == llvm::Instruction::Br)
        {
            std::vector<unsigned> new_basic_block(1, i);

            unsigned j = i;

            while(cfg[extra_values.size() + j].size() == 1 &&
                  cfg[extra_values.size() + j][0] != extra_values.size() + j &&
                  rcfg[cfg[extra_values.size() + j][0]].size() == 1 &&
                  opcodes[extra_values.size() + j] != llvm::Instruction::Br)
            {
                j = cfg[extra_values.size() + j][0] - extra_values.size();
                new_basic_block.push_back(j);
            }

            basic_blocks.push_back(new_basic_block);
        }
    }

    std::vector<llvm::BasicBlock*> llvm_basic_blocks;
    for(auto& basic_block : basic_blocks)
    {
        llvm::BasicBlock* new_llvm_basic_block = llvm::BasicBlock::Create(llvm_context);

        for(unsigned instr : basic_block)
        {
            new_llvm_basic_block->getInstList().push_back(instructions[instr]);
        }

        llvm_basic_blocks.push_back(new_llvm_basic_block);
    }

    for(unsigned i = 0; i < instructions.size(); i++)
    {
        for(unsigned j = 0; j < 32 && rodfg[j][extra_values.size() + i].size() == 1; j++)
        {
            if(rodfg[j][extra_values.size() + i][0] < extra_values.size())
                instructions[i]->setOperand(j, extra_values[rodfg[j][extra_values.size() + i][0]]);
            else if(rodfg[j][extra_values.size() + i][0] < extra_values.size() + instructions.size())
                instructions[i]->setOperand(j, instructions[rodfg[j][extra_values.size() + i][0] - extra_values.size()]);
            else
                fprintf(stderr, "error type 1!\n");
        }

        if(opcodes[extra_values.size() + i] == llvm::Instruction::PHI)
        {
            llvm::PHINode* phi_cast = llvm::dyn_cast<llvm::PHINode>(instructions[i]);

            for(unsigned j = 0; j < 4 && incoming[j][extra_values.size() + i].size() == 1; j++)
            {
                if(incoming[j][extra_values.size() + i][0] == extra_values.size() + i)
                    phi_cast->setIncomingBlock(j, nullptr);
                else if(incoming[j][extra_values.size() + i][0] < extra_values.size())
                    phi_cast->setIncomingBlock(j, nullptr);
                else if(incoming[j][extra_values.size() + i][0] < extra_values.size() + instructions.size())
                    phi_cast->setIncomingBlock(j, instructions[incoming[j][extra_values.size() + i][0] - extra_values.size()]->getParent());
                else
                    fprintf(stderr, "error type 2!\n");
            }
        }

        if(opcodes[extra_values.size() + i] == llvm::Instruction::Br)
        {
            for(unsigned j = 0; j < 4 && ocfg[j][extra_values.size() + i].size() == 1; j++)
            {
                if(ocfg[j][extra_values.size() + i][0] == extra_values.size() + i)
                    instructions[i]->setOperand(rdfg[extra_values.size() + i].size()+j, nullptr);
                else if(ocfg[j][extra_values.size() + i][0] < extra_values.size())
                    fprintf(stderr, "error type 3!\n");
                else if(ocfg[j][extra_values.size() + i][0] < extra_values.size() + instructions.size())
                    instructions[i]->setOperand(rdfg[extra_values.size() + i].size()+j, instructions[ocfg[j][extra_values.size() + i][0] - extra_values.size()]->getParent());
                else
                    fprintf(stderr, "error type 4!\n");
            }
        }
    }

    return llvm_basic_blocks;
}

// THIS IS DEPRECATED
llvm::Function* FunctionWrapper::reimplement(llvm::LLVMContext& llvm_context, std::vector<llvm::Type*> extra_arguments)
{
    std::vector<llvm::BasicBlock*> llvm_basic_blocks = rebuild_llvm_structure(llvm_context);

    std::vector<llvm::Type*> argument_types;
    for(unsigned i = 0; i < extra_values.size(); i++)
    {
        if(llvm::UndefValue* llvm_instr = llvm::dyn_cast<llvm::UndefValue>(extra_values[i]))
        {
            argument_types.push_back(llvm_instr->getType());
        }
    }

    argument_types.insert(argument_types.end(), extra_arguments.begin(), extra_arguments.end());

    llvm::Type* return_type = llvm::Type::getVoidTy(llvm_context);


    llvm::FunctionType* function_type = llvm::FunctionType::get(return_type, argument_types, false);

    llvm::Function* llvm_function = llvm::Function::Create(function_type, llvm::GlobalValue::ExternalLinkage);

    auto argument_iterator = llvm_function->getArgumentList().begin();
    for(unsigned i = 0; i < extra_values.size(); i++)
    {
        if(llvm::dyn_cast<llvm::UndefValue>(extra_values[i]))
        {
            extra_values[i] = &*(argument_iterator++);
        }
    }

    llvm::BasicBlock* llvm_start_block = llvm::BasicBlock::Create(llvm_context);
    llvm::BasicBlock* llvm_end_block = llvm::BasicBlock::Create(llvm_context);
    llvm::BranchInst::Create(llvm_basic_blocks[0], llvm_start_block); // THIS IS WRONG! NEED TO DETERMINE ACTUAL STARTING BLOCK
    llvm::ReturnInst::Create(llvm_context, nullptr, llvm_end_block);

    for(unsigned i = 0; i < instructions.size(); i++)
    {
        if(llvm::PHINode* phi_cast = llvm::dyn_cast<llvm::PHINode>(instructions[i]))
        {
            for(unsigned j = 0; j < phi_cast->getNumIncomingValues(); j++)
            {
                if(phi_cast->getIncomingBlock(j) == nullptr)
                {
                    phi_cast->setIncomingBlock(j, llvm_start_block);
                }
            }
        }
        else if(llvm::TerminatorInst* terminator_cast = llvm::dyn_cast<llvm::TerminatorInst>(instructions[i]))
        {
            for(unsigned j = 0; j < terminator_cast->getNumOperands(); j++)
            {
                if(terminator_cast->getOperand(j) == nullptr)
                {
                    terminator_cast->setOperand(j, llvm_end_block);
                }
            }
        }
    }

    llvm_function->getBasicBlockList().push_back(llvm_start_block);
    for(llvm::BasicBlock* block : llvm_basic_blocks)
        llvm_function->getBasicBlockList().push_back(block);
    llvm_function->getBasicBlockList().push_back(llvm_end_block);

    return llvm_function;
}

FunctionWrapper::FunctionWrapper(FunctionWrapper& superset,
                                 const std::vector<unsigned>& value_indizes,
                                 const std::vector<unsigned>& instr_indizes,
                                 unsigned stop_index)
{
    construct_llvm_values_from_superset(superset, value_indizes, instr_indizes);
    allocate_graphs();
    construct_opcodes_from_superset (superset, value_indizes, instr_indizes);
    construct_cdg_from_superset     (superset, value_indizes, instr_indizes);
    construct_incoming_from_superset(superset, value_indizes, instr_indizes);
    construct_ocfg_from_superset    (superset, value_indizes, instr_indizes, stop_index);
    construct_odfg_from_superset    (superset, value_indizes, instr_indizes);
    assemble_subgraphs();
    generate_pdg();
    sort_graphs();
}

FunctionWrapper::FunctionWrapper(llvm::Function& llvm_function)
{
    for(auto& block : llvm_function.getBasicBlockList())
    {
        for(auto& instruction : block)
        {
            instructions.push_back(&instruction);
        }
    }

    auto instr_hash = get_instruction_hashmap();
    auto value_hash = gather_non_instruction_values(instr_hash);

    allocate_graphs();

    construct_opcodes_from_llvm(instr_hash);
    construct_cdg_from_llvm(instr_hash);
    construct_ocfg_from_llvm(instr_hash);
    construct_incoming_from_llvm(instr_hash);
    construct_odfg_from_llvm(instr_hash, value_hash);

    assemble_subgraphs();
    generate_pdg();
    sort_graphs();
}
