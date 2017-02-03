#ifndef _LLVM_SOLVER_HPP_
#define _LLVM_SOLVER_HPP_
#include "SMTSolver.hpp"
#include "FunctionWrap.hpp"

class LLVMSolver : public Solver<std::string,unsigned>
{
public:
    LLVMSolver(const Constraint<std::string,unsigned>& c, FunctionWrapper& w) : Solver(c), wrap(w) { }

    std::vector<std::pair<std::string,llvm::Value*>> next_solution()
    {
        std::vector<std::pair<std::string,llvm::Value*>> result;

        for(auto& entry : Solver::next_solution())
        {
            result.emplace_back(entry.first, wrap.get_value(entry.second));
        }

        return result;
    }

    std::vector<std::vector<std::pair<std::string,llvm::Value*>>> all_solutions()
    {
        std::vector<std::vector<std::pair<std::string,llvm::Value*>>> result;

        while(true)
        {
            auto solution = next_solution();

            if(solution.size() > 0) result.push_back(solution);
            else                    break;
        }

        return result;
    }

private:
    FunctionWrapper& wrap;
};

#endif
