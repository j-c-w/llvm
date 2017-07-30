#ifndef _LLVM_SOLVER_HPP_
#define _LLVM_SOLVER_HPP_
#include "llvm/Constraints/SMTSolver.hpp"
#include "llvm/Constraints/Constraint.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"

std::vector<std::vector<std::pair<std::string,llvm::Value*>>> llvm_solver(const Constraint& constraint,
                                                                          llvm::Function& function,
                                                                          unsigned max_solutions = UINT_MAX)
{
    FunctionWrap wrap(function);
    std::vector<std::unique_ptr<SolverAtom>> specials;
    constraint.get_specials(wrap, specials);
    Solver solver(std::move(specials));

    std::vector<std::string> labels(constraint.begin(), constraint.end());

    std::vector<std::vector<std::pair<std::string,llvm::Value*>>> result;

    while(result.size() < max_solutions)
    {
        auto solution = solver.next_solution();

        if(solution.empty()) break;

        std::vector<std::pair<std::string,llvm::Value*>> part_solution(solution.size());

        for(unsigned i = 0; i < solution.size() && i < labels.size(); i++)
        {
            part_solution[i].first  = labels[i];
            part_solution[i].second = solution[i] < wrap.size() ? wrap[solution[i]] : nullptr;
        }

        result.emplace_back(std::move(part_solution));
    }

    return result;
}

#endif
