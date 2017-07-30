#include "llvm/Constraints/PrintSlots.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

SolutionHierarchical::SolutionHierarchical(std::vector<std::pair<std::string,std::string>> input)
{
    if(input.size() == 1 && input.begin()->first.empty())
    {
        printed_value = input.begin()->second;
    }
    else
    {
        std::map<std::string,std::vector<std::pair<std::string,std::string>>> one_indirection;

        for(const auto& entry : input)
        {
            if(entry.second == "None") continue; // skip "none" values
            else if(entry.first.size() > 0 && entry.first.front() == '.') continue;

            auto find_it = std::find(entry.first.begin(), entry.first.end(), '.');

            std::string variable_head(entry.first.begin(), find_it);

            std::string variable_tail(find_it, entry.first.end());

            if(find_it != entry.first.end())
                variable_tail = std::string(find_it + 1, entry.first.end());

            one_indirection[variable_head].emplace_back(variable_tail, entry.second);
        }

        for(const auto& entry : one_indirection)
        {
            dictionary.emplace(entry.first, SolutionHierarchical(entry.second));
        }
    }
}

std::vector<std::pair<std::string,std::string>> SolutionHierarchical::generate_strings(
                          std::vector<std::pair<std::string,llvm::Value*>> input, llvm::ModuleSlotTracker& slot_tracker)
{
    std::unordered_map<llvm::Value*,std::string> previously_generated;

    std::vector<std::pair<std::string,std::string>> result;

    for(auto& entry : input)
    {
        if(entry.second != nullptr)
        {
            auto find_it = previously_generated.find(entry.second);

            if(find_it != previously_generated.end())
            {
                result.emplace_back(entry.first, find_it->second);
            }
            else
            {
                std::string string_value;

                llvm::raw_string_ostream out_stream(string_value);

                if(llvm::Function* func_value = llvm::dyn_cast<llvm::Function>(entry.second))
                {
                    func_value->printAsOperand(out_stream, true, slot_tracker);
                }
                else if(llvm::Instruction* instr_value = llvm::dyn_cast<llvm::Instruction>(entry.second))
                {
                    instr_value->print(out_stream, slot_tracker);
                }
                else
                {
                    out_stream<<"  ";
                    entry.second->print(out_stream, slot_tracker);
                }

                std::string escaped_string;

                for(char c : out_stream.str())
                {
                    if(c == '\n')      escaped_string += "\\n";
                    else if(c == '\"') escaped_string += "\\\"";
                    else if(c == '\'') escaped_string += "\\\'";
                    else if(c == '\\') escaped_string += "\\\\";
                    else               escaped_string += std::string(1, c);
                }

                while(escaped_string.size() > 0 && escaped_string[0] == ' ')
                    escaped_string = std::string(escaped_string.begin() + 1, escaped_string.end());

                escaped_string = "\"" + escaped_string + "\"";

                result.emplace_back(entry.first, escaped_string);
                previously_generated.emplace(entry.second, escaped_string);
            }
        }
        else
        {
           result.emplace_back(entry.first, "None");
        }
    }

    return result;
}

std::string SolutionHierarchical::print_pythonesque() const
{
    if(dictionary.empty())
    {
        return printed_value;
    }
    else
    {
        std::string result;
        result.reserve(1024*1024);

        std::vector<std::string> printed_arrays;

        for(const auto& entry : dictionary)
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

                    for(const auto& entry2 : dictionary)
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

                    std::vector<std::string> part_results;
                    for(const auto& entry2 : element_map)
                    {
                        part_results.push_back(entry2.second.print_pythonesque());
                    }

                    unsigned total_size = (part_results.size()-1)*4;
                    for(const auto& entry2 : part_results)
                    {
                        for(char c : entry2)
                        {
                            if(c == '\n') total_size += 3;
                            else          total_size += 1;
                        }
                    }

                    std::string part_result(total_size, ' ');

                    unsigned dst = 0;
                    for(const auto& entry2 : part_results)
                    {
                        for(char c : entry2)
                        {
                            part_result[dst++] = c;

                            if(c == '\n') dst += 2;
                        }

                        if(&entry2 != &*part_results.rbegin())
                        {
                            part_result[dst++] = ',';
                            part_result[dst++] = '\n';
                            dst += 2;
                        }
                    }

                    result += " \"" + basename_str + "\":\n [" + part_result + " ],\n";
                }
            }
            else
            {
                std::string part_result;
                for(char c : entry.second.print_pythonesque())
                {
                    part_result.push_back(c);

                    if(c == '\n')
                    {
                        for(unsigned i = 0; i < entry.first.size() + 4; i++)
                        {
                            part_result.push_back(' ');
                        }
                    }
                }

                result += " \"" + entry.first + "\":" + part_result + ",\n";
            }
        }

        result.shrink_to_fit();
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
