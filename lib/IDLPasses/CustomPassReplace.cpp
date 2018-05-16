#include "llvm/IDLPasses/CustomPasses.hpp"
#include "llvm/IDLParser/Solution.hpp"
#include "llvm/IDLPasses/Transforms.hpp"
#include "llvm/IR/ModuleSlotTracker.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/IRBuilder.h"
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
                         loop_constraint_specs{/*{"histo",      GenerateAnalysis("Histo")},
                                               {"scalar",     GenerateAnalysis("Reduction")},
                                               {"GEMM",       GenerateAnalysis("GEMM")},
                                               {"SPMV",       GenerateAnalysis("SPMV")},
                                               {"stencil",    GenerateAnalysis("Stencil")},*/
                                               {"Experiment", GenerateAnalysis("Experiment")}} { }

    bool runOnModule(Module& module) override;

private:
    std::vector<std::pair<std::string,std::vector<Solution>(*)(Function&,unsigned)>> loop_constraint_specs;
};

struct SolutionCluster
{
    Instruction*                                precursor;
    Instruction*                                begin;
    Instruction*                                end;
    Instruction*                                successor;
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

            if(precursor_value == nullptr)
                precursor_value = (Value*)solution.get("store_new_next");

            Instruction* precursor      = dyn_cast_or_null<Instruction>(precursor_value);
            Instruction* begin          = dyn_cast_or_null<Instruction>(begin_value);
            Instruction* end            = dyn_cast_or_null<Instruction>(end_value);
            Instruction* successor      = dyn_cast_or_null<Instruction>(successor_value);

            

            auto find_it = std::find_if(result.begin(), result.end(), [=](const SolutionCluster& cluster)
                                        { return cluster.begin == begin && cluster.successor == successor; });

            if(find_it == result.end())
                result.push_back(SolutionCluster{precursor, begin, end, successor,
                                                 {{input.first, {std::move(solution)}}}});
            else
                find_it->solutions[input.first].push_back(std::move(solution));
        }
    }

    return result;
}

void replace_spmv(Function& function, Solution solution)
{
    Instruction* precursor = dyn_cast_or_null<Instruction>((Value*)solution.get("precursor"));

    Instruction* store = dyn_cast_or_null<Instruction>((Value*)solution.get("output").get("store"));

    if(store)
    {
        auto ov     = (Value*)solution.get("output").get("base_pointer");
        auto a      = (Value*)solution.get("seq_read").get("base_pointer");
        auto iv     = (Value*)solution.get("indir_read").get("base_pointer");
        auto rowstr = (Value*)solution.get("iter_begin_read").get("base_pointer");
        auto colidx = (Value*)solution.get("idx_read").get("base_pointer");
        auto rows   = (Value*)solution.get("iter_end");

        store->removeFromParent();
        FunctionType* func_type = FunctionType::get(ov->getType(),
                                     {ov->getType(), a->getType(), iv->getType(),
                                     rowstr->getType(), colidx->getType(), rows->getType()}, false);


        Constant* func = function.getParent()->getOrInsertFunction("spmv_harness", func_type);

    //    BasicBlock::iterator iter(precursor);
    //    SmartIRBuilder builder(comparison->getParent(), iter);


        CallInst::Create(func, {ov, a, iv, rowstr, colidx, rows}, "", precursor);
    }
}

bool ResearchReplacer::runOnModule(Module& module)
{
    ModuleSlotTracker slot_tracker(&module);

    std::string filename = module.getName();
    for(char& c : filename)
        if(c == '/')
            c = '_';

    std::stringstream sstr;
    sstr<<"replace-report-"<<filename<<".json";
    std::ofstream ofs(sstr.str().c_str());
    ofs<<"{\n  \"filename\": \""<<(std::string)module.getName()<<"\",\n  \"loops\": [";

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
                unsigned line_begin = 999;

                if(clustered_solutions[i].precursor && clustered_solutions[i].precursor->getDebugLoc())
                    line_begin = clustered_solutions[i].precursor->getDebugLoc().getLine();

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
                            if(c == '\n') ofs<<"        ";
                        }
                        ofs<<"\n    }";
                        first_hit2 = false;
                    }
                }

                for(auto& solution : clustered_solutions[i].solutions["SPMV"])
                {
                    replace_spmv(function, solution);
                }

                ofs<<"]";

                if(clustered_solutions[i].solutions["histo"].size() > 0 ||
                   clustered_solutions[i].solutions["scalar"].size() > 0)
                {
                    ofs<<",\n      \"operator\": \"";
                    /*
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
                    delete function;
                    */
                    ofs<<"\"";
                }

                ofs<<"\n    }";
                first_hit1 = false;
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
