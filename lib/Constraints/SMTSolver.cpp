#include "llvm/Constraints/SMTSolver.hpp"
#include <vector>
#include <cstdio>
#include <iostream>

template<typename LabelType, typename ValueType>
Solver<LabelType,ValueType>::Solver(const Constraint<LabelType,ValueType>& constraint)
                           : iterator(0), variables{}, solution{}, specializations(constraint.get_specializations()) { }

template<typename LabelType, typename ValueType>
std::vector<std::pair<LabelType,ValueType>> Solver<LabelType,ValueType>::next_solution(unsigned max_steps)
{
    if(specializations.size() == 0)
    {
        return {};
    }
    else if(iterator == 0)
    {
        solution.push_back(ValueType());
        variables.push_back(&specializations[iterator].first);
        specializations[iterator].second->begin();
    }
    else if(iterator == specializations.size())
    {
        specializations[--iterator].second->resume();
        ++solution[iterator];
    }

    while(iterator <  specializations.size() && (max_steps--))
    {
        SkipResult result = SkipResult::CHANGE;

        while(result == SkipResult::CHANGE)
        {
            result = specializations[iterator].second->skip_invalid(solution[iterator]);
        }

        if(result != SkipResult::FAIL)
        {
            specializations[iterator++].second->fixate();

            if(iterator <  specializations.size())
            {
                variables.push_back(&specializations[iterator].first);
                solution.push_back(ValueType());
                specializations[iterator].second->begin();
            }
        }
        else
        {
            specializations[iterator--].second->cancel();

            variables.pop_back();
            solution.pop_back();

            if(iterator <  specializations.size())
            {
                specializations[iterator].second->resume();
                ++solution[iterator];
            }
        }
    }

    if(max_steps && iterator == specializations.size())
    {
        std::vector<std::pair<LabelType,ValueType>> result;

        for(unsigned i = 0; i < variables.size(); i++)
            result.emplace_back(*variables[i], solution[i]);

        return result;
    }
    else
    {
        return {};
    }
}

template class Solver<std::string,unsigned>;
