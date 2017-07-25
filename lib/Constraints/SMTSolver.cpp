#include "llvm/Constraints/SMTSolver.hpp"
#include "llvm/Constraints/Constraint.hpp"
#include <vector>
#include <cstdio>
#include <iostream>

Solver::Solver(std::vector<std::pair<std::string,SpecializedContainer>> s)
      : iterator(0), variables{}, solution{}, specializations(std::move(s))
{ }

std::vector<std::pair<std::string,Specialized::Value>> Solver::next_solution(unsigned max_steps)
{
    std::vector<unsigned> solver_histogram(specializations.size(), 0);

    if(specializations.size() == 0)
    {
        return {};
    }
    else if(iterator == 0)
    {
        solution.push_back(Specialized::Value());
        variables.push_back(&specializations[iterator].first);
        specializations[iterator].second->begin();
    }
    else if(iterator == specializations.size())
    {
        --iterator;
        specializations[iterator].second->resume(solution[iterator]);
        ++solution[iterator];
    }

    while(iterator <  specializations.size() && (max_steps--))
    {
        solver_histogram[iterator]++;

        SkipResult result = SkipResult::CHANGE;

        while(result == SkipResult::CHANGE)
        {
            result = specializations[iterator].second->skip_invalid(solution[iterator]);
        }

        if(result != SkipResult::FAIL)
        {
            specializations[iterator].second->fixate(solution[iterator]);
            ++iterator;

            if(iterator <  specializations.size())
            {
                variables.push_back(&specializations[iterator].first);
                solution.push_back(Specialized::Value());
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
                specializations[iterator].second->resume(solution[iterator]);
                ++solution[iterator];
            }
        }
    }

    if(max_steps && iterator == specializations.size())
    {
        std::vector<std::pair<std::string,Specialized::Value>> result;

        result.reserve(variables.size());
        for(unsigned i = 0; i < variables.size(); i++)
            result.emplace_back(*variables[i], solution[i]);

        return result;
    }
    else
    {
        return {};
    }
}

