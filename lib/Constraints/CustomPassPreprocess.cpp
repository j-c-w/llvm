#include "llvm/Constraints/CustomPasses.hpp"
#include "llvm/Constraints/LLVMSolver.hpp"
#include "llvm/Constraints/ConstraintSpecifications.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/Constraints/PrintSlots.hpp"
#include "llvm/Constraints/Transforms.hpp"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_os_ostream.h"
#include <fstream>
#include <sstream>

class PreprocessorPass : public llvm::ModulePass
{
public:
    static char ID;

    PreprocessorPass() : llvm::ModulePass(ID) {}

    bool runOnModule(llvm::Module& module) override
    {
        std::ofstream ofs("preprocess-report.txt");

        for(llvm::Function& function : module.getFunctionList())
        {
            bool found_something = false;

            while(true)
            {
            FunctionWrapper wrap(function);

            auto distributive1012_solutions = LLVMSolver(ConstraintDistributive(wrap, 1,0,1,2), wrap).all_solutions();
            auto distributive0012_solutions = LLVMSolver(ConstraintDistributive(wrap, 0,0,1,2), wrap).all_solutions();
            auto hoistselect_solutions      = LLVMSolver(ConstraintHoistSelect (wrap),          wrap).all_solutions();

            if((distributive1012_solutions.size() > 0 || distributive0012_solutions.size() > 0 ||
               hoistselect_solutions.size() > 0) && !found_something)
            {
                std::stringstream str_str;
                llvm::raw_os_ostream out_stream(str_str);
                function.printAsOperand(out_stream);
                out_stream.flush();
                str_str.flush();
                ofs<<"BEGIN FUNCTION PREPROCESSING "<< str_str.str()<<"\n";
                found_something = true;
            }

            for(auto solution : distributive1012_solutions)
            {
                ofs<<"BEGIN DISTRIBUTIVE1012\n"
                   <<SolutionHierarchical(solution).print_pythonesque()<<"\n"
                   <<"END DISTRIBUTIVE1012\n";

                std::map<std::string,llvm::Value*> solution_map(solution.begin(), solution.end());
                transform_distributive(function, solution_map);
                break;
            }

            if(distributive1012_solutions.size() > 0) continue;

            for(auto solution : distributive0012_solutions)
            {
                ofs<<"BEGIN DISTRIBUTIVE0012\n"
                   <<SolutionHierarchical(solution).print_pythonesque()<<"\n"
                   <<"END DISTRIBUTIVE0012\n";

                std::map<std::string,llvm::Value*> solution_map(solution.begin(), solution.end());
                transform_distributive(function, solution_map);
                break;
            }

            if(distributive0012_solutions.size() > 0) continue;

            for(auto solution : hoistselect_solutions)
            {
                ofs<<"BEGIN HOISTSELECT\n"
                   <<SolutionHierarchical(solution).print_pythonesque()<<"\n"
                   <<"END HOISTSELECT\n";

                std::map<std::string,llvm::Value*> solution_map(solution.begin(), solution.end());
                transform_hoistselect_pattern(function, solution_map);
                break;
            }

            if(hoistselect_solutions.size() > 0) continue;

            break;
            }

            if(found_something)
            {
                ofs<<"END FUNCTION PREPROCESSING\n";
            }
        }

        ofs.close();
        return false;
    }
};

char PreprocessorPass::ID = 0;

llvm::ModulePass *llvm::createPreprocessorPass() {
  return new PreprocessorPass();
}
