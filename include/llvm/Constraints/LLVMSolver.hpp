#ifndef _LLVM_SOLVER_HPP_
#define _LLVM_SOLVER_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include "llvm/Constraints/Constraint.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include <sstream>
#include <map>

namespace llvm {
class ModuleSlotTracker;
}

class IdiomInstance
{
public:
    operator llvm::Value* () const;
    operator const std::vector<IdiomInstance>* () const;
    operator const std::map<std::string,IdiomInstance>* () const;

    llvm::Value* get() const { return single_value; }

    void prune();
    std::string print_json(llvm::ModuleSlotTracker&) const;

    template<typename ... T>
    llvm::Value* get(std::string label, T...t) const
    {
        if(vector_value.empty())
        {
            auto find_it = map_value.find(label);
            if(find_it != map_value.end())
                return find_it->second.get(t...);
        }
        return nullptr;
    }

    template<typename ... T>
    llvm::Value* get(unsigned idx, T...t) const
    {
        if(map_value.empty() && idx < vector_value.size())
        {
            return vector_value[idx].get(t...);
        }
        return nullptr;
    }

    std::map<std::string,llvm::Value*> flatten() const
    {
        if(single_value)
            return std::map<std::string,llvm::Value*>{{"",single_value}};

        if(map_value.empty())
        {
            std::map<std::string,llvm::Value*> result;

            for(unsigned i = 0; i < vector_value.size(); i++)
            {
                for(auto& part2 : vector_value[i].flatten())
                {
                    std::stringstream sstr;
                    sstr<<"["<<i<<"]";
                    if(!part2.first.empty())
                        sstr<<".";
                    sstr<<part2.first;

                    result.emplace(sstr.str(), part2.second);
                }
            }

            return result;
        }

        if(vector_value.empty())
        {
            std::map<std::string,llvm::Value*> result;

            for(auto& part : map_value)
            {
                for(auto& part2 : part.second.flatten())
                {
                    std::stringstream sstr;
                    sstr<<part.first;
                    if(!part2.first.empty() && part2.first.front() != '[')
                        sstr<<".";
                    sstr<<part2.first;

                    result.emplace(sstr.str(), part2.second);
                }
            }

            return result;
        }

        return {};
    }
/*

    llvm::Value* get() const { return single_value; }

    template<typename ... T>
    auto get(std::string label, T...t) const -> decltype(get_empty(t...))
    {
        auto it = map_value.find(label);
        return it != map_value.end() ? it->second.get(t...) : get_empty(t...);
    }

    template<typename ... T>
    auto get(unsigned idx, T...t) const -> decltype(get_empty(t...))
    {
        return idx < vector_value.size() ? vector_value[idx].get(t...) : get_empty(t...);
    }
*/
    static std::vector<IdiomInstance> Find(const Constraint& constraint, llvm::Function& function,
                                            unsigned max_solutions = UINT_MAX);

private:/*
    llvm::Value* get_empty() const { return nullptr; }

    template<typename ... T>
    auto get_empty(std::string label, T...t) const -> decltype(get_empty(t...)) { return get_empty(t...); }
*/

    IdiomInstance(std::vector<std::string> labels, std::vector<llvm::Value*> values);

    llvm::Value*                         single_value;
    std::vector<IdiomInstance>          vector_value;
    std::map<std::string,IdiomInstance> map_value;
};

std::vector<std::vector<std::pair<std::string,llvm::Value*>>> llvm_solver(const Constraint& constraint,
                                                                          llvm::Function& function,
                                                                          unsigned max_solutions = UINT_MAX);

#endif
