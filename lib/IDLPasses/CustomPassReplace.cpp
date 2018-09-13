#include "llvm/IDL/CustomPasses.hpp"
#include "llvm/IDL/Solution.hpp"
#include "llvm/IR/ModuleSlotTracker.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

using namespace llvm;

class ResearchReplacer : public ModulePass
{
public:
    static char ID;

    ResearchReplacer() : ModulePass(ID) { }

    bool runOnModule(Module& module) override;
};

void replace_idiom(Function& function, Solution solution, std::string harness_name,
                   Value* insertion, std::vector<Value*> variables, std::vector<Value*> removeeffects)
{
    Instruction* insertion_point = dyn_cast_or_null<Instruction>(insertion);

    std::vector<Value*> variable_values;
    std::vector<Type*>  variable_types;
    for(auto value : variables)
    {
        if(value)
        {
            variable_values.push_back(value);
            variable_types.push_back(value->getType());
        }
    }

    FunctionType* func_type = FunctionType::get(Type::getVoidTy(function.getContext()), variable_types, false);
    Constant*          func = function.getParent()->getOrInsertFunction(harness_name, func_type);

    CallInst::Create(func, variable_values, "", insertion_point);

    for(auto& value : removeeffects)
    {
        if(auto inst = dyn_cast_or_null<Instruction>(value))
        {
            inst->removeFromParent();
        }
    }
}

bool ResearchReplacer::runOnModule(Module& module)
{
    ModuleSlotTracker slot_tracker(&module);

    std::string filename = module.getName();
    for(char& c : filename) if(c == '/') c = '_';

    std::stringstream sstr;
    sstr<<"replace-report-"<<filename<<".json";
    std::ofstream ofs(sstr.str().c_str());
    ofs<<"{\"filename\": \""<<(std::string)module.getName()<<"\",\n \"detected\": [";

    char first_hit1 = true;
    for(Function& function : module.getFunctionList())
    {
        for(auto& idiom : std::vector<std::string>{"GEMM", "SPMV_CSR", "SPMV_JDS", "VectorAdd", "VectorDot"})
        {
            for(auto& solution : GenerateAnalysis(idiom)(function, 100))
            {
                unsigned line_begin = 999;

                if(auto precursor = dyn_cast_or_null<Instruction>((Value*)solution["precursor"]))
                    if(auto& debugloc = precursor->getDebugLoc())
                        line_begin = debugloc.getLine();

                ofs<<(first_hit1?"\n":",\n");
                ofs<<"  {\"function\": \""<<(std::string)function.getName()<<"\",\n";
                ofs<<"   \"line\": "<<line_begin<<",\n";
                ofs<<"   \"type\": \""<<idiom<<"\",\n";
                ofs<<"   \"solution\":\n    ";
                for(char c : solution.prune().print_json(slot_tracker))
                {
                    ofs.put(c);
                    if(c == '\n') ofs<<"    ";
                }
                ofs<<"\n  }";
                first_hit1 = false;

                if(idiom == "GEMM")
                {/*
                    replace_idiom(function, solution, "gemm_harness", solution["precursor"],
                                  {solution["loop"][0]["iter_end"],
                                   solution["loop"][1]["iter_end"],
                                   solution["loop"][2]["iter_end"],
                                   solution["iter_begin_read"]["base_pointer"],
                                   solution["idx_read"]["base_pointer"],
                                   solution["iter_end"]}, {solution["output"]["store"]});*/
                }
                if(idiom == "SPMV_CSR")
                {
                    replace_idiom(function, solution, "spmv_csr_harness", solution["successor"],
                                  {solution["output"]["base_pointer"],
                                   solution["matrix_read"]["base_pointer"],
                                   solution["vector_read"]["base_pointer"],
                                   solution["iter_begin_read"]["base_pointer"],
                                   solution["index_read"]["base_pointer"],
                                   solution["iter_end"]}, {solution["output"]["store"]});
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
