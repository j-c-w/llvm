#include "llvm/Constraints/CustomPasses.hpp"
#include "llvm/Constraints/LLVMSolver.hpp"
#include "llvm/Constraints/IdiomSpecifications.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/Constraints/PrintSlots.hpp"
#include "llvm/Constraints/Transforms.hpp"
#include "llvm/Constraints/GenerateCOperator.hpp"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Constraints/SeparateSESE.hpp"
#include "llvm/Support/raw_os_ostream.h"
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>

using namespace llvm;

using Solution = std::vector<std::pair<std::string,Value*>>;

struct SolutionCluster
{
    Value*                                                begin;
    Value*                                                end;
    std::unordered_map<std::string,std::vector<Solution>> solutions;
};

// This function applies some postprocessing to bundle together reductions.
// The proper way would be to encode it in a new constraint specification "composed reduction" but this will do for now.
std::vector<SolutionCluster> cluster_solutions(std::vector<std::pair<std::string,std::vector<Solution>>> inputs)
{
    std::vector<SolutionCluster> result;

    for(auto& cluster_spec : inputs)
    {
        for(const auto& solution : cluster_spec.second)
        {
            auto find_begin_it = std::find_if(solution.begin(), solution.end(),
                 [](const std::pair<std::string,Value*>& a) { return a.first == "body_sese.begin"; });
            auto find_end_it = std::find_if(solution.begin(), solution.end(),
                 [](const std::pair<std::string,Value*>& a) { return a.first == "body_sese.successor"; });

            if(find_begin_it != solution.end() && find_begin_it->second != nullptr &&
               find_end_it   != solution.end() && find_end_it->second   != nullptr)
            {
                bool new_entry = true;
                for(unsigned i = 0; i < result.size(); i++)
                {
                    if(result[i].begin == find_begin_it->second && result[i].end == find_end_it->second)
                    {
                        new_entry = false;

                        result[i].solutions[cluster_spec.first].push_back(std::move(solution));
                    }
                }

                if(new_entry)
                {
                    result.push_back({find_begin_it->second, find_end_it->second,
                                      std::unordered_map<std::string,std::vector<Solution>>()});
                    result.back().solutions[cluster_spec.first].push_back(std::move(solution));
                }
            }
        }
    }

    return result;
}

class ResearchReplacer : public ModulePass
{
public:
    static char ID;

    ResearchReplacer() : ModulePass(ID)
    {
        constraint_specs.emplace_back("histo",   ConstraintHisto());
        constraint_specs.emplace_back("scalar",  ConstraintReduction());
        constraint_specs.emplace_back("GEMM",    ConstraintGEMM());
        constraint_specs.emplace_back("GEMV",    ConstraintGEMV());
        constraint_specs.emplace_back("AXPY",    ConstraintAXPY());
        constraint_specs.emplace_back("DOT",     ConstraintDOT());
        constraint_specs.emplace_back("SPMV",    ConstraintSPMV());
        constraint_specs.emplace_back("stencil", ConstraintStencil());
    }

    bool runOnModule(Module& module) override;

private:
    std::vector<std::pair<std::string,ConstraintAnd>> constraint_specs;
};

bool ResearchReplacer::runOnModule(Module& module)
{
    ModuleSlotTracker slot_tracker(&module);

    std::ofstream ofs("replace-report.txt");

    for(Function& function : module.getFunctionList())
    {
        if(!function.isDeclaration())
        {
            FunctionWrapper wrap(function);

            std::vector<std::pair<std::string,std::vector<Solution>>> raw_solutions;

            for(const auto& spec : constraint_specs)
                raw_solutions.push_back({spec.first, LLVMSolver(spec.second, wrap).all_solutions()});

            auto clustered_solutions = cluster_solutions(std::move(raw_solutions));

            if(clustered_solutions.empty()) continue;

            ofs<<"BEGIN FUNCTION TRANSFORMATION "<<(std::string)function.getName()<<"\n";

            for(unsigned i = 0; i < clustered_solutions.size(); i++)
            {
                ofs<<"BEGIN LOOP\n";

                for(const auto& spec : constraint_specs)
                {
                    for(auto solution : clustered_solutions[i].solutions[spec.first])
                    {
                        ofs<<"BEGIN "<<spec.first<<"\n"
                           <<SolutionHierarchical(solution, slot_tracker).print_pythonesque()<<"\n"
                           <<"END "<<spec.first<<"\n";
                    }
                }

                if(clustered_solutions[i].solutions["histo"].size() > 0 ||
                   clustered_solutions[i].solutions["scalar"].size() > 0)
                {
                    ofs<<"BEGIN OPERATOR\n";
                    std::vector<Instruction*> outputs;

                    SESEFunction sese_function(function, clustered_solutions[i].begin, clustered_solutions[i].end);

                    Function* function = sese_function.make_function();

                    std::vector<std::map<std::string,Value*>> scalar_solutions;
                    std::vector<std::map<std::string,Value*>> histo_solutions;

                    for(auto& solution : clustered_solutions[i].solutions["scalar"])
                    {
                        sese_function.transform_forw(solution);
                        scalar_solutions.emplace_back(solution.begin(), solution.end());
                    }

                    for(auto& solution : clustered_solutions[i].solutions["histo"])
                    {
                        sese_function.transform_forw(solution);
                        histo_solutions.emplace_back(solution.begin(), solution.end());
                    }

                    transform_reduction_operator(*function, scalar_solutions, histo_solutions);

                    function->setName("op");
                    function->arg_begin()->setName("acc");
                    ofs<<print_pretty_c_operator(*function);
                    delete function;
                }

                ofs<<"END LOOP\n";
            }

            ofs<<"END FUNCTION TRANSFORMATION\n";
        }
    }

    ofs.close();
    return false;
}

char ResearchReplacer::ID = 0;

static RegisterPass<ResearchReplacer> X("research-replacer", "Research replacer", false, false);

ModulePass *llvm::createResearchReplacerPass() {
  return new ResearchReplacer();
}
