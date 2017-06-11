#include "llvm/Constraints/CustomPasses.hpp"
#include "llvm/Constraints/LLVMSolver.hpp"
#include "llvm/Constraints/ConstraintSpecifications.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/Constraints/PrintSlots.hpp"
#include "llvm/Constraints/Transforms.hpp"
#include "llvm/Constraints/ConstraintsBasic.hpp"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_os_ostream.h"
#include <fstream>
#include <sstream>

bool llvm::CustomPreprocessorPass::runOnModule(llvm::Module& module)
{
    llvm::ModuleSlotTracker slot_tracker(&module);

    auto constraint_hoist_select = ConstraintHoistSelect();
    auto constraint_distributive = ConstraintDistributive();

    std::ofstream ofs("preprocess-report.txt");

    for(llvm::Function& function : module.getFunctionList())
    {
        if(!function.isDeclaration())
        {
            bool found_something = false;

            while(true)
            {
                FunctionWrapper wrap(function);

                auto hoistselect_solutions = LLVMSolver(constraint_hoist_select, wrap).all_solutions(1);

                if(hoistselect_solutions.size() > 0)
                {
                    if(!found_something)
                    {
                        std::stringstream str_str;
                        llvm::raw_os_ostream out_stream(str_str);
                        function.printAsOperand(out_stream);
                        out_stream.flush();
                        str_str.flush();
                        ofs<<"BEGIN FUNCTION PREPROCESSING "<< str_str.str()<<"\n";
                        found_something = true;
                    }

                    ofs<<"BEGIN HOISTSELECT\n"
                       <<SolutionHierarchical(hoistselect_solutions[0], slot_tracker).print_pythonesque()<<"\n"
                       <<"END HOISTSELECT\n";

                    std::map<std::string,llvm::Value*> solution_map(hoistselect_solutions[0].begin(),
                                                                    hoistselect_solutions[0].end());
                    transform_hoistselect_pattern(function, solution_map);

                    continue;
                }
                else break;
            }

            while(true)
            {
                FunctionWrapper wrap(function);

                auto distributive_solutions = LLVMSolver(constraint_distributive, wrap).all_solutions(1);

                if(distributive_solutions.size() > 0)
                {
                    if(!found_something)
                    {
                        std::stringstream str_str;
                        llvm::raw_os_ostream out_stream(str_str);
                        function.printAsOperand(out_stream);
                        out_stream.flush();
                        str_str.flush();
                        ofs<<"BEGIN FUNCTION PREPROCESSING "<< str_str.str()<<"\n";
                        found_something = true;
                    }

                    ofs<<"BEGIN DISTRIBUTIVE\n"
                       <<SolutionHierarchical(distributive_solutions[0], slot_tracker).print_pythonesque()<<"\n"
                       <<"END DISTRIBUTIVE\n";

                    std::map<std::string,llvm::Value*> solution_map(distributive_solutions[0].begin(),
                                                                    distributive_solutions[0].end());
                    transform_distributive(function, solution_map);

                    continue;
                }
                else break;
            }

            if(found_something)
            {
                ofs<<"END FUNCTION PREPROCESSING\n";
            }
        }
    }

    ofs.close();
    return false;
}

char llvm::CustomPreprocessorPass::ID = 0;

static llvm::RegisterPass<llvm::CustomPreprocessorPass> X("research-preprocessor", "Research preprocessor", false, false);

llvm::ModulePass *llvm::createPreprocessorPass() {
  return new CustomPreprocessorPass();
}
