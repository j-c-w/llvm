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

using namespace llvm;

class ResearchPreprocessor : public ModulePass
{
public:
    static char ID;

    ResearchPreprocessor() : ModulePass(ID)
    {
        constraint_specs.emplace_back("HOISTSELECT",  ConstraintHoistSelect(),  &transform_hoistselect_pattern);
        constraint_specs.emplace_back("DISTRIBUTIVE", ConstraintDistributive(), &transform_distributive);
    }

    bool runOnModule(Module& module) override;

private:
    std::vector<std::tuple<std::string,ConstraintAnd,void(*)(Function&,std::map<std::string,Value*>)>> constraint_specs;
};

bool ResearchPreprocessor::runOnModule(Module& module)
{
    ModuleSlotTracker slot_tracker(&module);

    std::ofstream ofs("preprocess-report.txt");

    for(Function& function : module.getFunctionList())
    {
        if(!function.isDeclaration())
        {
            bool found_something = false;

            for(const auto& spec : constraint_specs)
            {
                while(true)
                {
                    FunctionWrapper wrap(function);

                    auto solutions = LLVMSolver(std::get<1>(spec), wrap).all_solutions(1);

                    if(solutions.empty()) break;

                    if(!found_something)
                    {
                        ofs<<"BEGIN FUNCTION PREPROCESSING "<<(std::string)function.getName()<<"\n";
                        found_something = true;
                    }

                    ofs<<"BEGIN "<<std::get<0>(spec)<<"\n"
                       <<SolutionHierarchical(solutions[0], slot_tracker).print_pythonesque()<<"\n"
                       <<"END "<<std::get<0>(spec)<<"\n";

                    std::map<std::string,Value*> solution_map(solutions[0].begin(), solutions[0].end());
                    (*std::get<2>(spec))(function, solution_map);
                }
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

char ResearchPreprocessor::ID = 0;

static RegisterPass<ResearchPreprocessor> X("research-preprocessor", "Research preprocessor", false, false);

ModulePass *llvm::createResearchPreprocessorPass() {
  return new ResearchPreprocessor();
}
