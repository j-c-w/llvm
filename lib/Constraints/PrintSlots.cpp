#include "llvm/Constraints/PrintSlots.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>

SolutionHierarchical::SolutionHierarchical(std::vector<std::pair<std::string,llvm::Value*>> input)
                    : dictionary(), llvm_value(nullptr)
{
    if(input.size() == 1 && input.begin()->first.empty())
    {
        llvm_value = input.begin()->second;
    }
    else
    {
        std::map<std::string,std::vector<std::pair<std::string,llvm::Value*>>> one_indirection;

        for(auto entry : input)
        {
            if(entry.second == nullptr) continue; // skip "none" values

            auto find_it = std::find(entry.first.begin(), entry.first.end(), '.');

            std::string variable_head(entry.first.begin(), find_it);

            std::string variable_tail(find_it, entry.first.end());

            if(find_it != entry.first.end())
                variable_tail = std::string(find_it + 1, entry.first.end());

            one_indirection[variable_head].emplace_back(variable_tail, entry.second);
        }

        for(auto entry : one_indirection)
        {
            dictionary.emplace(entry.first, SolutionHierarchical(entry.second));
        }
    }
}

std::string SolutionHierarchical::print_pythonesque()
{
    if(dictionary.empty())
    {
        if(llvm_value != nullptr)
        {
            std::stringstream str_str;
            llvm::raw_os_ostream out_stream(str_str);

            if(llvm::Function* func_value = llvm::dyn_cast<llvm::Function>(llvm_value))
            {
                func_value->printAsOperand(out_stream);
            }
            else if(llvm::Instruction* instr_value = llvm::dyn_cast<llvm::Instruction>(llvm_value))
            {
                out_stream<<*instr_value;
            }
            else
            {
                out_stream<<"  "<<*llvm_value;
            }

            out_stream.flush();
            str_str.flush();

            std::string escaped_string;

            for(char c : str_str.str())
            {
                if(c == '\n')      escaped_string += "\\n";
                else if(c == '\"') escaped_string += "\\\"";
                else if(c == '\'') escaped_string += "\\\'";
                else if(c == '\\') escaped_string += "\\\\";
                else               escaped_string += std::string(1, c);
            }

            while(escaped_string.size() > 0 && escaped_string[0] == ' ')
                escaped_string = std::string(escaped_string.begin() + 1, escaped_string.end());

            return "\"" + escaped_string + "\"";
        }
        else
        {
            return "None";
        }
    }
    else
    {
        std::string result;

        unsigned max_name_length = 0;

        for(auto entry : dictionary)
        {
            auto find_it_open = std::find(entry.first.begin(), entry.first.end(), '[');
            auto basename_str = std::string(entry.first.begin(), find_it_open);

            if(basename_str.size() > max_name_length)
                max_name_length = basename_str.size();
        }

        std::vector<std::string> printed_arrays;

        for(auto entry : dictionary)
        {
            auto find_it_open  = std::find(entry.first.begin(), entry.first.end(), '[');
            auto find_it_close = std::find(find_it_open,        entry.first.end(), ']');

            if(find_it_close + 1 == entry.first.end())
            {
                auto basename_str = std::string(entry.first.begin(), find_it_open);
                auto index_str    = std::string(find_it_open + 1,    find_it_close);

                if(std::find(printed_arrays.begin(), printed_arrays.end(), basename_str) == printed_arrays.end())
                {
                    printed_arrays.push_back(basename_str);

                    std::map<unsigned,SolutionHierarchical> element_map;

                    element_map.emplace(atoi(index_str.c_str()), entry.second);

                    for(auto entry2 : dictionary)
                    {
                        auto find_it_open2  = std::find(entry2.first.begin(), entry2.first.end(), '[');
                        auto find_it_close2 = std::find(find_it_open2,        entry2.first.end(), ']');

                        if(find_it_close2 + 1 == entry2.first.end())
                        {
                            auto index_str2    = std::string(find_it_open2 + 1,    find_it_close2);
                            auto basename_str2 = std::string(entry2.first.begin(), find_it_open2);

                            if(basename_str == basename_str2)
                            {
                                element_map.emplace(atoi(index_str2.c_str()), entry2.second);
                            }
                        }
                    }

//                    std::string assign_string = "  \"" + basename_str + "\""
//                                              + std::string(max_name_length - basename_str.size(), ' ') + " : [ ";

                    std::string assign_string = "  \"" + basename_str + "\" :\n    [ ";

                    std::string part_result;

                    for(auto& entry2 : element_map)
                    {
                        for(char c : entry2.second.print_pythonesque())
                        {
                            if(c == '\n') part_result += "\n      ";
                            else          part_result += std::string(1, c);
                        }

                        if(&entry2 != &*element_map.rbegin())
//                            part_result += ",\n" + std::string(assign_string.size(), ' ');
                            part_result += ",\n      ";
                        else
                            part_result += " ]";
                    }

                    result += assign_string + part_result + ",\n";
                }
            }
            else
            {
                std::string assign_string = "  \"" + entry.first + "\""/*
                                          + std::string(max_name_length - entry.first.size(), ' ')*/ + " : ";

                std::string part_result;

                for(char c : entry.second.print_pythonesque())
                {
                    if(c == '\n') part_result += "\n" + std::string(assign_string.size(), ' ');
                    else          part_result += std::string(1, c);
                }

                result += assign_string + part_result + ",\n";
            }
        }

        if(result.empty())
        {
            return "{ }";
        }
        else
        {
            result.front() = '{';
            result.resize(result.size() - 2);
            return result + " }";
        }
    }
}


/*
static
std::string generate_c_typename(llvm::Type* type)
{
    if(type->isIntegerTy(1))       return "bool";
    else if(type->isIntegerTy(8))  return "char";
    else if(type->isIntegerTy(16)) return "short";
    else if(type->isIntegerTy(32)) return "int";
    else if(type->isIntegerTy(64)) return "long";
    else if(type->isFloatTy())     return "float";
    else if(type->isDoubleTy())    return "double";
    else if(type->isX86_FP80Ty())  return "long double";
    else return "void";
}

static
std::string generate_llvm_typename(llvm::Type* type)
{
    if(type->isIntegerTy(1))       return "bool";
    else if(type->isIntegerTy(8))  return "char";
    else if(type->isIntegerTy(16)) return "short";
    else if(type->isIntegerTy(32)) return "int";
    else if(type->isIntegerTy(64)) return "long";
    else if(type->isFloatTy())     return "float";
    else if(type->isDoubleTy())    return "double";
    else if(type->isX86_FP80Ty())  return "long double";
    else return "void";
}

static
std::string convert_to_c_get_name(std::vector<std::string>& llvm_value_names, int idx, std::vector<unsigned>& closure)
{
    if(llvm_value_names[idx].size())
        return llvm_value_names[idx];
    else
    {
        std::stringstream sstr;
        sstr<<"closure"<<closure.size();
        llvm_value_names[idx] = sstr.str();
        closure.push_back(idx);
        return sstr.str();
    }
}

static
std::vector<std::string> convert_to_c(FunctionWrapper& wrap,
                                      std::vector<unsigned> computation,
                                      std::vector<std::pair<unsigned,std::string>> inputs,
                                      std::vector<unsigned>& closure)
{
    std::vector<std::string> result;

    std::vector<std::string> llvm_value_names;

    for(unsigned i = 0; i < wrap.extra_values.size() + wrap.instructions.size(); i++)
    {
        std::stringstream sstr;

        if(auto const_int = llvm::dyn_cast<llvm::ConstantInt>(wrap.get_value(i)))
        {
            sstr<<"("<<generate_c_typename(const_int->getType())<<")"<<const_int->getZExtValue();
        }
        else if(auto const_float = llvm::dyn_cast<llvm::ConstantFP>(wrap.get_value(i)))
        {
            sstr<<std::setprecision(std::numeric_limits<double>::max_digits10)<<std::fixed;
            sstr<<"("<<generate_c_typename(const_float->getType())<<")"<<const_float->getValueAPF().convertToDouble();
        }
        else if(auto function = llvm::dyn_cast<llvm::Function>(wrap.get_value(i)))
        {
            sstr<<function->getName().str();
        }

        llvm_value_names.push_back(sstr.str());
    }

    for(unsigned i = 0; i < inputs.size(); i++)
    {
        llvm_value_names[inputs[i].first] = inputs[i].second;
    }

    for(unsigned i : computation)
    {
        std::stringstream sstr;
        sstr<<"tmp"<<i;
        llvm_value_names[i] = sstr.str();
    }

    std::vector<unsigned> label_neccessary(wrap.extra_values.size() + wrap.instructions.size(), 0);

    for(unsigned i : computation)
    {
        for(unsigned successor : wrap.cfg[i])
        {
            if(successor != i+1 && successor < label_neccessary.size())
                label_neccessary[successor] = true;
        }
    }

    for(unsigned i : computation)
    {
        if(wrap.get_instruction(i)->getOpcode() == llvm::Instruction::PHI)
        {
            std::stringstream sstr;

            std::string type = generate_llvm_typename(wrap.get_value(i)->getType());

            sstr<<type<<" phi"<<i<<";";

            result.push_back(sstr.str());
        }
    }

    for(unsigned i : computation)
    {
        if(label_neccessary[i])
        {
            std::stringstream sstr;

            sstr<<"label"<<i<<":";

            result.push_back(sstr.str());
        }


        if(auto branch_inst = llvm::dyn_cast<llvm::BranchInst>(wrap.get_instruction(i)))
        {
            for(unsigned j : computation)
            {
                if(auto llvm_phi = llvm::dyn_cast<llvm::PHINode>(wrap.get_instruction(j)))
                {
                    for(unsigned k = 0; k < llvm_phi->getNumIncomingValues(); k++)
                    {
                        if(llvm_phi->getIncomingBlock(k) == branch_inst->getParent())
                        {
                            std::stringstream sstr;
                            sstr<<"phi"<<j<<" = "<<llvm_value_names[wrap.rodfg[k][j][0]]<<";";
                            result.push_back(sstr.str());
                        }
                    }
                }
            }

            std::stringstream sstr;

            if(wrap.cfg[i].size() == 1 && label_neccessary[wrap.cfg[i][0]])
                sstr<<"goto label"<<wrap.cfg[i][0]<<";";
            else if(wrap.cfg[i].size() == 2 && label_neccessary[wrap.cfg[i][0]] && label_neccessary[wrap.cfg[i][1]])
                sstr<<"if("<<llvm_value_names[wrap.rodfg[0][i][0]]<<") goto label"<<wrap.cfg[i][0]<<"; else goto label"<<wrap.cfg[i][1]<<";";
            else if(wrap.cfg[i].size() == 2 && label_neccessary[wrap.cfg[i][0]])
                sstr<<"if("<<llvm_value_names[wrap.rodfg[0][i][0]]<<") goto label"<<wrap.cfg[i][0]<<";";
            else if(wrap.cfg[i].size() == 2 && label_neccessary[wrap.cfg[i][1]])
                sstr<<"if(!"<<llvm_value_names[wrap.rodfg[0][i][0]]<<") goto label"<<wrap.cfg[i][1]<<";";
            else
                sstr<<"//invalid jump";

            result.push_back(sstr.str());
        }
        else
        {
            if(wrap.get_instruction(i)->getOpcode() == llvm::Instruction::PHI)
            {
                std::string type = generate_llvm_typename(wrap.get_value(i)->getType());
                std::string rslt = convert_to_c_get_name(llvm_value_names, i, closure);
    
                std::stringstream sstr;
    
                sstr<<type<<" "<<rslt<<" = phi"<<i<<";";
    
                result.push_back(sstr.str());
            }
            else if(auto binary = llvm::dyn_cast<llvm::BinaryOperator>(wrap.get_instruction(i)))
            {
                std::string type = generate_llvm_typename(wrap.get_value(i)->getType());
                std::string rslt = convert_to_c_get_name(llvm_value_names, i, closure);
                std::string arg1 = convert_to_c_get_name(llvm_value_names, wrap.rodfg[0][i][0], closure);
                std::string arg2 = convert_to_c_get_name(llvm_value_names, wrap.rodfg[1][i][0], closure);
    
                std::stringstream sstr;
    
                sstr<<type<<" "<<rslt<<" = "<<arg1;
    
                if(binary->getOpcode() == llvm::Instruction::Add) sstr<<" + ";
                if(binary->getOpcode() == llvm::Instruction::Sub) sstr<<" - ";
                if(binary->getOpcode() == llvm::Instruction::Mul) sstr<<" * ";
                if(binary->getOpcode() == llvm::Instruction::UDiv) sstr<<" / ";
                if(binary->getOpcode() == llvm::Instruction::SDiv) sstr<<" / ";
                if(binary->getOpcode() == llvm::Instruction::FAdd) sstr<<" + ";
                if(binary->getOpcode() == llvm::Instruction::FSub) sstr<<" - ";
                if(binary->getOpcode() == llvm::Instruction::FMul) sstr<<" * ";
                if(binary->getOpcode() == llvm::Instruction::FDiv) sstr<<" / ";
    
                sstr<<arg2<<";";
    
                result.push_back(sstr.str());
            }
            else if(auto cmp_inst = llvm::dyn_cast<llvm::CmpInst>(wrap.get_instruction(i)))
            {
                std::string type = generate_llvm_typename(wrap.get_value(i)->getType());
                std::string rslt = convert_to_c_get_name(llvm_value_names, i, closure);
                std::string arg1 = convert_to_c_get_name(llvm_value_names, wrap.rodfg[0][i][0], closure);
                std::string arg2 = convert_to_c_get_name(llvm_value_names, wrap.rodfg[1][i][0], closure);
    
                std::stringstream sstr;
    
                sstr<<type<<" "<<rslt<<" = "<<arg1;
    
                if(cmp_inst->getPredicate() == llvm::CmpInst::FCMP_OLT) sstr<<" < ";
                if(cmp_inst->getPredicate() == llvm::CmpInst::FCMP_OLE) sstr<<" <= ";
                if(cmp_inst->getPredicate() == llvm::CmpInst::FCMP_OGT) sstr<<" > ";
                if(cmp_inst->getPredicate() == llvm::CmpInst::FCMP_OGE) sstr<<" >= ";
    
                sstr<<arg2<<";";
    
                result.push_back(sstr.str());
            }
            else if(wrap.get_instruction(i)->getOpcode() == llvm::Instruction::Call)
            {
                std::string type = generate_llvm_typename(wrap.get_value(i)->getType());
                std::string rslt = convert_to_c_get_name(llvm_value_names, i, closure);
    
                std::vector<std::string> arguments;
    
                for(unsigned j = 0; j < 8 && wrap.rodfg[j][i].size(); j++)
                    arguments.push_back(convert_to_c_get_name(llvm_value_names, wrap.rodfg[j][i][0], closure));
    
                std::stringstream sstr;
    
                sstr<<type<<" "<<rslt<<" = "<<arguments.back()<<"(";
    
                for(unsigned j = 0; j + 1 < arguments.size(); j++)
                    sstr<<((j==0)?"":", ")<<arguments[j];
    
                sstr<<");";
    
                result.push_back(sstr.str());
            }
            else if(llvm::dyn_cast<llvm::CastInst>(wrap.get_instruction(i)))
            {
                std::string type = generate_llvm_typename(wrap.get_value(i)->getType());
                std::string rslt = convert_to_c_get_name(llvm_value_names, i, closure);
                std::string arg1 = convert_to_c_get_name(llvm_value_names, wrap.rodfg[0][i][0], closure);
    
                std::stringstream sstr;
    
                sstr<<type<<" "<<rslt<<" = ("<<type<<")"<<arg1<<";";
    
                result.push_back(sstr.str());
            }
            else if(llvm::dyn_cast<llvm::SelectInst>(wrap.get_instruction(i)))
            {
                std::string type = generate_llvm_typename(wrap.get_value(i)->getType());
                std::string rslt = convert_to_c_get_name(llvm_value_names, i, closure);
                std::string arg1 = convert_to_c_get_name(llvm_value_names, wrap.rodfg[0][i][0], closure);
                std::string arg2 = convert_to_c_get_name(llvm_value_names, wrap.rodfg[1][i][0], closure);
                std::string arg3 = convert_to_c_get_name(llvm_value_names, wrap.rodfg[2][i][0], closure);
    
                std::stringstream sstr;
    
                sstr<<type<<" "<<rslt<<" = "<<arg1<<"?"<<arg2<<":"<<arg3<<";";
    
                result.push_back(sstr.str());
            }
            else
            {
                std::stringstream sstr;
                llvm::raw_os_ostream out_stream(sstr);
                out_stream<<*wrap.get_instruction(i);
                out_stream.flush();
                result.push_back(sstr.str());
            }
        }
    }

    return result;
}

std::string SolutionHierarchical::print_pure_function(std::vector<std::string> prefix)
{
    if(prefix.size() > 0)
    {
        return dictionary[prefix.front()].print_pure_function(std::vector<std::string>(prefix.begin()+1, prefix.end()));
    }

    std::vector<std::string> input_names;
    std::vector<std::string> constant_names;
    std::vector<std::string> control_history_names;

    for(unsigned i = 0; i < 32; i++)
    {
        std::stringstream sstr;
        sstr<<"input["<<i<<"]";
        input_names.push_back(sstr.str());
    }

    for(unsigned i = 0; i < 64; i++)
    {
        std::stringstream sstr;
        sstr<<"constants["<<i<<"]";
        constant_names.push_back(sstr.str());
    }

    for(unsigned i = 0; i < 4; i++)
    {
        std::stringstream sstr;
        sstr<<"control_history["<<i<<"]";
        control_history_names.push_back(sstr.str());
    }

    std::vector<unsigned> working_status(function->pdg.size(), 0);
    std::vector<unsigned> working_queue (function->pdg.size(), 0);

    unsigned queue_front = 0;
    unsigned queue_back  = 0;

    working_queue[queue_back++] = dictionary["output"].value;
    working_status[dictionary["output"].value] = 1;

    while(queue_front < queue_back)
    {
        for(unsigned parent : function->rpdg[working_queue[queue_front++]])
        {
            if(working_status[parent] == 0)
            {
                bool is_accounted_for = false;
                for(auto& name : input_names)
                {
                    if(parent == dictionary[name].value)
                    {
                        working_status[parent] = 2;
                        is_accounted_for = true;
                    }
                }

                if(!is_accounted_for)
                {
                    for(auto& name : constant_names)
                    {
                        if(parent == dictionary[name].value)
                        {
                            working_status[parent] = 3;
                            is_accounted_for = true;
                        }
                    }
                }

                if(!is_accounted_for)
                {
                    for(auto& name : control_history_names)
                    {
                        if(parent == dictionary[name].value)
                        {
                            working_status[parent] = 4;
                            is_accounted_for = true;
                        }
                    }
                }

                if(!is_accounted_for)
                {
                    working_status[parent] = 1;
                    working_queue[queue_back++] = parent;
                    
                }
            }
        }
    }

//    std::cerr<<"Identified all used instructions\n";


    std::vector<unsigned> used_instructions;

    std::vector<unsigned> working_status2(function->pdg.size(), 0);
    std::vector<unsigned> working_queue2 (function->pdg.size(), 0);

    unsigned queue_front2 = 0;
    unsigned queue_back2  = 0;

    working_queue2[queue_back2++] = dictionary["inner_sese"].dictionary["begin"].value;
    working_status2[dictionary["inner_sese"].dictionary["begin"].value] = 1;

    while(queue_front2 < queue_back2)
    {
        if(working_status[working_queue2[--queue_back2]] == 1)
        {
            used_instructions.push_back(working_queue2[queue_back2]);
        }

        for(unsigned child : function->cfg[working_queue2[queue_back2]])
        {
            if(child != dictionary["inner_sese"].dictionary["successor"].value && working_status2[child] == 0)
            {
                working_queue2[queue_back2++] = child;
                working_status2[child] = 1;
            }
        }
    }

//    std::cerr<<"assembled all used instructions\n";

    std::vector<unsigned> closure;
    std::vector<std::pair<unsigned,std::string>> inputs;

//    for(unsigned i = 0; i < input_names.size(); i++)
//    {
//        std::stringstream sstr;
//        sstr<<"in.__"<<i;

//        if(dictionary[input_names[i]].value != UINT_MAX-1)
//        {
//            inputs.emplace_back(dictionary[input_names[i]].value, sstr.str());
//        }
//    }

    for(unsigned i = 0; i < working_status.size(); i++)
    {
        if(working_status[i] == 3)
            closure.push_back(i);

        if(working_status[i] == 2)
        {
            bool found_it = false;

            for(unsigned j = 0; j < input_names.size(); j++)
            {
                if(dictionary[input_names[j]].value == i)
                {
                    std::stringstream sstr;

                    if(j == 31) sstr<<"old";
                    else        sstr<<"in._"<<j;

                    inputs.emplace_back(i, sstr.str());
                    found_it = true;
                }
            }

            if(!found_it)
                inputs.emplace_back(i, "other_input");
        }
    }

    std::vector<std::string> result = convert_to_c(*function, used_instructions, inputs, closure);

    std::string final_result;

    for(auto& line : result)
        if(final_result.size() > 0)
            final_result += "\\n" + line;
        else
            final_result += line;

    return "\"" + final_result + "\"";
}*/
