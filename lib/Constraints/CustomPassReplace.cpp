#include "llvm/Constraints/CustomPasses.hpp"
#include "llvm/Constraints/IdiomSpecifications.hpp"
#include "llvm/Constraints/Transforms.hpp"
#include "llvm/IR/ModuleSlotTracker.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace llvm;

struct SolutionCluster
{
    Value*                                      begin;
    Value*                                      successor;
    std::map<std::string,std::vector<Solution>> solutions;
};

// This function applies some postprocessing to bundle together reductions.
// The proper way would be to encode it in a new constraint specification "composed reduction" but this will do for now.
std::vector<SolutionCluster> cluster_solutions(std::vector<std::pair<std::string,std::vector<Solution>>> inputs)
{
    std::vector<SolutionCluster> result;

    for(auto& input : inputs)
    {
        for(const auto& solution : input.second)
        {
            llvm::Value* begin     = solution.get("body", "begin");
            llvm::Value* successor = solution.get("body", "successor");

            if(!begin && !successor) continue;

            auto find_it = std::find_if(result.begin(), result.end(), [=](const SolutionCluster& cluster)
                                        { return cluster.begin == begin && cluster.successor == successor; });

            if(find_it == result.end())
                result.push_back(SolutionCluster{begin, successor, {{input.first, {std::move(solution)}}}});
            else
                find_it->solutions[input.first].push_back(std::move(solution));
        }
    }

    return result;
}

class ResearchReplacer : public ModulePass
{
public:
    static char ID;

    ResearchReplacer() : ModulePass(ID),
                         constraint_specs{{"histo",   DetectHisto},   {"scalar",  DetectReduction},
                                          {"GEMM",    DetectGEMM},    {"GEMV",    DetectGEMV},
                                          {"AXPY",    DetectAXPY},    {"AXPYn",   DetectAXPYn},
                                          {"DOT",     DetectDOT},     {"SPMV",    DetectSPMV},
                                          {"stencil", DetectStencil}, {"stenpls", DetectStencilPlus}} { }

    bool runOnModule(Module& module) override;

private:
    std::vector<std::pair<std::string,std::vector<Solution>(*)(Function&,unsigned)>> constraint_specs;
};

bool ResearchReplacer::runOnModule(Module& module)
{
    ModuleSlotTracker slot_tracker(&module);

    std::ofstream ofs("replace-report.txt");

    for(Function& function : module.getFunctionList())
    {
        if(!function.isDeclaration())
        {
            std::vector<std::pair<std::string,std::vector<Solution>>> raw_solutions;

            for(const auto& spec : constraint_specs)
                raw_solutions.push_back({spec.first, spec.second(function,10)});

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
                        ofs<<"BEGIN "<<spec.first<<"\n"<<solution.prune().print_json(slot_tracker)<<"\n"
                             <<"END "<<spec.first<<"\n";
                    }
                }

                if(clustered_solutions[i].solutions["histo"].size() > 0 ||
                   clustered_solutions[i].solutions["scalar"].size() > 0)
                {
                    ofs<<"BEGIN OPERATOR\n";
                    std::vector<Instruction*> outputs;

                    SESEFunction sese_function(clustered_solutions[i].begin, clustered_solutions[i].successor);

                    Function* function = sese_function.make_function();

                    std::vector<std::map<std::string,Value*>> scalar_solutions;
                    for(const auto& solution : clustered_solutions[i].solutions["scalar"])
                        scalar_solutions.emplace_back(sese_function.transform_forw(solution));

                    std::vector<std::map<std::string,Value*>> histo_solutions;
                    for(const auto& solution : clustered_solutions[i].solutions["histo"])
                        histo_solutions.emplace_back(sese_function.transform_forw(solution));

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
