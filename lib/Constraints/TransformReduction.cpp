#include "llvm/Constraints/Transforms.hpp"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

using namespace llvm;

void transform_reduction_operator(Function& function, std::vector<std::map<std::string,Value*>> scalars,
                                                      std::vector<std::map<std::string,Value*>> histos)
{
    std::vector<Value*> old_value_fields;
    std::vector<Value*> updated_scalar_fields;
    std::vector<Value*> histobin_instr_fields;
    std::vector<Value*> load_instr_fields;
    std::vector<Value*> store_instr_fields;
    std::vector<Type*>  reduction_type_fields;

    for(const auto& solution : scalars)
    {
        auto find_old_value      = solution.find("old_value");
        auto find_updated_scalar = solution.find("update_expr.output");

        if(find_old_value      != solution.end() && find_old_value->second      != nullptr &&
           find_updated_scalar != solution.end() && find_updated_scalar->second != nullptr)
        {
            old_value_fields.push_back(find_old_value->second);
            updated_scalar_fields.push_back(find_updated_scalar->second);
            reduction_type_fields.push_back(find_old_value->second->getType());
        }
    }

    for(const auto& solution : histos)
    {
        auto find_store_instr    = solution.find("store_instr");
        auto find_histobin_instr = solution.find("index_expr.output");
        auto find_load_instr     = solution.find("old_value");

        if(find_store_instr    != solution.end() && find_store_instr->second    != nullptr &&
           find_histobin_instr != solution.end() && find_histobin_instr->second != nullptr &&
           find_load_instr     != solution.end() && find_load_instr->second     != nullptr)
        {
            histobin_instr_fields.push_back(find_histobin_instr->second);
            store_instr_fields.push_back(find_store_instr->second);
            load_instr_fields.push_back(find_load_instr->second);

            if(auto store_cast = dyn_cast<StoreInst>(find_store_instr->second))
            {
                reduction_type_fields.push_back(store_cast->getOperand(1)->getType());
            }
        }
    }

    Type* reduction_type     = StructType::get(function.getContext(), reduction_type_fields);
    Type* reduction_ptr_type = PointerType::get(reduction_type, 0);

    function.getArgumentList().push_back(new Argument(reduction_ptr_type));

    Argument* reduction_argument = &function.getArgumentList().back();

    std::vector<Value*> arguments;

    for(auto solution : scalars)
    {
        for(unsigned j = 0; j < 30; j++)
        {
            std::stringstream sstr;
            sstr<<"update_expr.input["<<j<<"]";

            auto find_input_j = solution.find(sstr.str());

            if(find_input_j != solution.end() && find_input_j->second != nullptr)
            {
                if(std::find(arguments.begin(), arguments.end(), find_input_j->second) == arguments.end())
                {
                    arguments.push_back(find_input_j->second);
                }
            }
        }
    }

    for(auto solution : histos)
    {
        for(unsigned j = 0; j < 30; j++)
        {
            std::stringstream sstr;
            sstr<<"update_expr.input["<<j<<"]";

            auto find_input_j = solution.find(sstr.str());

            if(find_input_j != solution.end() && find_input_j->second != nullptr)
            {
                if(std::find(arguments.begin(), arguments.end(), find_input_j->second) == arguments.end())
                {
                    arguments.push_back(find_input_j->second);
                }
            }
        }

        for(unsigned j = 0; j < 30; j++)
        {
            std::stringstream sstr;
            sstr<<"index_expr.input["<<j<<"]";

            auto find_input_j = solution.find(sstr.str());

            if(find_input_j != solution.end() && find_input_j->second != nullptr)
            {
                if(std::find(arguments.begin(), arguments.end(), find_input_j->second) == arguments.end())
                {
                    arguments.push_back(find_input_j->second);
                }
            }
        }
    }

    std::vector<Argument*> actual_arguments;

    for(unsigned i = 0; i < arguments.size(); i++)
    {
        function.getArgumentList().push_back(new llvm::Argument(arguments[i]->getType()));
        actual_arguments.push_back(&function.getArgumentList().back());
    }

    std::vector<Value*> scalar_geps;
    std::vector<Value*> scalar_loads;
    std::vector<Value*> histo_geps;
    std::vector<Value*> histo_loads;

    if(function.getBasicBlockList().empty() || function.getBasicBlockList().front().getInstList().empty()) return;

    Instruction& first_inst = function.getBasicBlockList().front().getInstList().front();

    for(unsigned i = 0; i < old_value_fields.size(); i++)
    {
        scalar_geps.push_back(GetElementPtrInst::Create(reduction_type, reduction_argument,
                              {ConstantInt::get(Type::getInt64Ty(function.getContext()), 0),
                               ConstantInt::get(Type::getInt32Ty(function.getContext()), i)}, "", &first_inst));

        scalar_loads.push_back(new LoadInst(scalar_geps.back(), "", &first_inst));
    }

    for(unsigned i = 0; i < load_instr_fields.size(); i++)
    {
        unsigned abs_idx = old_value_fields.size() + i;

        histo_geps.push_back(GetElementPtrInst::Create(reduction_type, reduction_argument,
                             {ConstantInt::get(Type::getInt64Ty(function.getContext()), 0),
                              ConstantInt::get(Type::getInt32Ty(function.getContext()), abs_idx)}, "", &first_inst));

        histo_loads.push_back(new LoadInst(histo_geps.back(), "", &first_inst));
    }


    std::vector<Value*> scalar_stores;

    for(unsigned j = 0; j < old_value_fields.size(); j++)
    {
        scalar_stores.push_back(new StoreInst(updated_scalar_fields[j], scalar_geps[j], false,
                                              dyn_cast<Instruction>(updated_scalar_fields[j])->getParent()->getTerminator()));
    }

    for(BasicBlock& basic_block : function.getBasicBlockList())
    {
        for(Instruction& instruction : basic_block.getInstList())
        {
            for(unsigned j = 0; j < old_value_fields.size(); j++)
            {
                for(unsigned i = 0; i < instruction.getNumOperands(); i++)
                {
                    if(instruction.getOperand(i) == old_value_fields[j])
                    {
                        instruction.setOperand(i, scalar_loads[j]);
                    }
                }
            }

            for(unsigned j = 0; j < histobin_instr_fields.size(); j++)
            {
                if(&instruction == histobin_instr_fields[j])
                {
                    instruction.setOperand(0, histo_loads[j]);
                }
            }

            for(unsigned j = 0; j < arguments.size(); j++)
            {
                for(unsigned i = 0; i < instruction.getNumOperands(); i++)
                {
                    if(instruction.getOperand(i) == arguments[j])
                    {
                        instruction.setOperand(i, actual_arguments[j]);
                    }
                }
            }
        }
    }

    // dead code elimination happens here, it is very naive though, need to either enhance or use proper existing 
    // implementation instead.
    while(true)
    {
        bool found_useless_instruction = false;

        for(BasicBlock& basic_block : function.getBasicBlockList())
        {
            for(Instruction& instruction : basic_block.getInstList())
            {
                if(instruction.getNumUses() == 0 && !isa<TerminatorInst>(&instruction) &&
                   std::find(store_instr_fields.begin(), store_instr_fields.end(), &instruction) == store_instr_fields.end() &&
                   std::find(scalar_stores.begin(), scalar_stores.end(), &instruction) == scalar_stores.end())
                {
                    found_useless_instruction = true;
                    instruction.eraseFromParent();
                    break;
                }
            }

            if(found_useless_instruction)
            {
                break;
            }
        }

        if(!found_useless_instruction)
        {
            break;
        }
    }
}
