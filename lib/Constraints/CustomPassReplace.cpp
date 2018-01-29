#include "llvm/Constraints/CustomPasses.hpp"
#include "llvm/Constraints/IdiomSpecifications.hpp"
#include "llvm/Constraints/Transforms.hpp"
#include "llvm/IR/ModuleSlotTracker.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace llvm;

class ResearchReplacer : public ModulePass
{
public:
    static char ID;

    ResearchReplacer() : ModulePass(ID),
                         simple_constraint_specs{{"Experiment", DetectExperiment}},
                         loop_constraint_specs{{"histo",   DetectHisto},   {"scalar",  DetectReduction},
                                               {"GEMM",    DetectGEMM},    {"GEMV",    DetectGEMV},
                                               {"AXPY",    DetectAXPY},    {"AXPYn",   DetectAXPYn},
                                               {"DOT",     DetectDOT},     {"SPMV",    DetectSPMV},
                                               {"stencil", DetectStencil}, {"stenpls", DetectStencilPlus},
                                               {"SCoP",    DetectSCoP}} { }

    bool runOnModule(Module& module) override;

private:
    std::vector<std::pair<std::string,std::vector<Solution>(*)(Function&,unsigned)>> simple_constraint_specs;
    std::vector<std::pair<std::string,std::vector<Solution>(*)(Function&,unsigned)>> loop_constraint_specs;
};

struct SolutionCluster
{
    Instruction*                                precursor;
    Instruction*                                begin;
    Instruction*                                end;
    Instruction*                                successor;
    Instruction*                                body_precursor;
    Instruction*                                body_begin;
    Instruction*                                body_end;
    Instruction*                                body_successor;
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
            Value* precursor_value      = (Value*)solution.get("precursor");
            Value* begin_value          = (Value*)solution.get("begin");
            Value* end_value            = (Value*)solution.get("end");
            Value* successor_value      = (Value*)solution.get("successor");
            Value* body_precursor_value = (Value*)solution.get("body", "precursor");
            Value* body_begin_value     = (Value*)solution.get("body", "begin");
            Value* body_end_value       = (Value*)solution.get("body", "end");
            Value* body_successor_value = (Value*)solution.get("body", "successor");

            Instruction* precursor      = dyn_cast_or_null<Instruction>(precursor_value);
            Instruction* begin          = dyn_cast_or_null<Instruction>(begin_value);
            Instruction* end            = dyn_cast_or_null<Instruction>(end_value);
            Instruction* successor      = dyn_cast_or_null<Instruction>(successor_value);
            Instruction* body_precursor = dyn_cast_or_null<Instruction>(body_precursor_value);
            Instruction* body_begin     = dyn_cast_or_null<Instruction>(body_begin_value);
            Instruction* body_end       = dyn_cast_or_null<Instruction>(body_end_value);
            Instruction* body_successor = dyn_cast_or_null<Instruction>(body_successor_value);

            if(!precursor || !begin || !end || !successor ||
               !body_precursor || !body_begin || !body_end || !body_successor) continue;

            auto find_it = std::find_if(result.begin(), result.end(), [=](const SolutionCluster& cluster)
                                        { return cluster.begin == begin && cluster.successor == successor; });

            if(find_it == result.end())
                result.push_back(SolutionCluster{precursor, begin, end, successor,
                                                 body_precursor, body_begin, body_end, body_successor,
                                                 {{input.first, {std::move(solution)}}}});
            else
                find_it->solutions[input.first].push_back(std::move(solution));
        }
    }

    return result;
}


bool ResearchReplacer::runOnModule(Module& module)
{
    ModuleSlotTracker slot_tracker(&module);

    std::stringstream sstr;
    sstr<<"replace-report-"<<(std::string)module.getName()<<".json";
    std::ofstream ofs(sstr.str().c_str());
    ofs<<"{\n  \"filename\": \""<<(std::string)module.getName()<<"\"\n  \"loops\": [";

    char first_hit1 = true;
    for(Function& function : module.getFunctionList())
    {
        if(!function.isDeclaration())
        {
            std::vector<std::pair<std::string,std::vector<Solution>>> raw_solutions;

            for(const auto& spec : loop_constraint_specs)
                raw_solutions.push_back({spec.first, spec.second(function,100)});

            auto clustered_solutions = cluster_solutions(std::move(raw_solutions));

            if(clustered_solutions.empty())
                continue;

            for(unsigned i = 0; i < clustered_solutions.size(); i++)
            {
                unsigned line_begin = clustered_solutions[i].precursor->getDebugLoc().getLine();

                ofs<<(first_hit1?"\n":",\n");
                ofs<<"    {\n      \"function\": \""<<(std::string)function.getName()<<"\",\n";
                ofs<<"      \"line\": "<<line_begin<<",\n";
                ofs<<"      \"idioms\": [\n";

                char first_hit2 = true;
                for(const auto& spec : loop_constraint_specs)
                {
                    for(auto& solution : clustered_solutions[i].solutions[spec.first])
                    {
                        ofs<<(first_hit2?"\n":",\n");
                        ofs<<"    {\n      \"type\": \""<<spec.first<<"\",\n";
                        ofs<<"      \"solution\":\n        ";
                        for(char c : solution.prune().print_json(slot_tracker))
                        {
                            ofs.put(c);
                            if(c == '\n') ofs<<"          ";
                        }
                        ofs<<"\n    }";
                        first_hit2 = false;
                    }
                }

                ofs<<"]";

                if(clustered_solutions[i].solutions["histo"].size() > 0 ||
                   clustered_solutions[i].solutions["scalar"].size() > 0)
                {
                    ofs<<",\n      \"operator\": \"";
                    std::vector<Instruction*> outputs;

                    SESEFunction sese_function(clustered_solutions[i].body_begin,
                                               clustered_solutions[i].body_successor);

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
                    for(char c : print_pretty_c_operator(*function))
                    {
                        if(c=='\n') ofs<<"\\n";
                        else if(c=='\t') ofs<<"\\t";
                        else if(c=='\r') ofs<<"\\r";
                        else if(c=='\"') ofs<<"\\\"";
                        else if(c=='\'') ofs<<"\\\'";
                        else ofs<<c;
                    }
                    ofs<<"\"";
                    delete function;
                }

                ofs<<"\n    }";
            }
        }
    }

    ofs<<"],\n  \"transformations\": [";

    char first_hit = true;
    for(Function& function : module.getFunctionList())
    {
        if(!function.isDeclaration())
        {
            for(const auto& spec : simple_constraint_specs)
            {
                auto solutions = spec.second(function, UINT_MAX);

                if(!solutions.empty())
                {
                    for(auto& solution : solutions)
                    {
                        ofs<<(first_hit?"\n":",\n");
                        ofs<<"    {\n      \"function\": \""<<(std::string)function.getName()<<"\",\n";
                        ofs<<"      \"type\": \""<<spec.first<<"\",\n";
                        ofs<<"      \"solution\":\n        ";
                        for(char c : solution.prune().print_json(slot_tracker))
                        {
                            ofs.put(c);
                            if(c == '\n') ofs<<"        ";
                        }
                        ofs<<"\n    }";
                        first_hit = false;
                    }
                }
            }
        }
    }

    ofs<<"]\n}\n";

    return false;
}

char ResearchReplacer::ID = 0;

static RegisterPass<ResearchReplacer> X("research-replacer", "Research replacer", false, false);

ModulePass *llvm::createResearchReplacerPass() {
  return new ResearchReplacer();
}
