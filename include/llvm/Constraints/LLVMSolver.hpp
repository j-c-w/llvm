#ifndef _LLVM_SOLVER_HPP_
#define _LLVM_SOLVER_HPP_
#include "llvm/Constraints/Constraint.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"

class LLVMSolver : public Solver
{
public:
    LLVMSolver(const Constraint& c, FunctionWrapper& w) : Solver(c.get_specializations(w)), wrap(w) { }

    std::vector<std::pair<std::string,llvm::Value*>> next_solution()
    {
        std::vector<std::pair<std::string,llvm::Value*>> result;

        auto solution = Solver::next_solution(UINT_MAX);

        result.reserve(solution.size());
        for(auto& entry : solution)
        {
            result.emplace_back(std::move(entry.first), wrap.get_value(entry.second));
        }

        return result;
    }

    std::vector<std::vector<std::pair<std::string,llvm::Value*>>> all_solutions(unsigned max_solutions = UINT_MAX)
    {
        std::vector<std::vector<std::pair<std::string,llvm::Value*>>> result;

        while(result.size() < max_solutions)
        {
            auto solution = next_solution();

            if(solution.size() > 0) result.emplace_back(std::move(solution));
            else                    break;
        }

        return result;
    }

private:
    FunctionWrapper& wrap;
};

#endif
