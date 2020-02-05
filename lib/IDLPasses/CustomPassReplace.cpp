#include "llvm/IDL/CustomPasses.hpp"
#include "llvm/IDL/Solution.hpp"
#include "llvm/IR/ModuleSlotTracker.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace llvm;

class ResearchReplacerBase : public ModulePass
{
public:
    using Pivot  = std::function<Value*(const Solution&)>;
    using Action = std::function<void(Function&, Solution)>;

    static char ID;

    ResearchReplacerBase(std::vector<std::tuple<std::string,Pivot,Action>> i) : ModulePass(ID), idioms(i) { }

    bool runOnModule(Module& module) override;

private:
    std::vector<std::tuple<std::string,Pivot,Action>> idioms;
};

bool ResearchReplacerBase::runOnModule(Module& module)
{
    ModuleSlotTracker slot_tracker(&module);

    std::string filename = module.getName();
    for(char& c : filename) if(c == '/') c = '_';

    std::stringstream sstr;
    std::streambuf * cout_strbuf = std::cout.rdbuf();
    std::ostream ofs(cout_strbuf);
    ofs<<"****IDL Match Report for :replace-report-"<<filename<<".json\n";
    ofs<<"{ \"filename\": \""<<(std::string)module.getName()<<"\",\n  \"detected\": [";

    char first_hit1 = true;
    for(Function& function : module.getFunctionList())
    {
        if(!function.isDeclaration())
        {
            if(function.getBasicBlockList().size() > 1000) continue;

            for(auto& idiom : idioms)
            {
                for(auto& solution : GenerateAnalysis(std::get<0>(idiom))(function, 99))
                {
                    int pivot_begin = -1;
                    if(const auto& pivot_lookup = std::get<1>(idiom))
                        if(auto pivot = dyn_cast_or_null<Instruction>(pivot_lookup(solution)))
                            if(auto& debugloc = pivot->getDebugLoc())
                                pivot_begin = debugloc.getLine();

                    ofs<<(first_hit1?"{\n":", {\n");
                    ofs<<"    \"function\": \""<<(std::string)function.getName()<<"\",\n";
                    if(pivot_begin > 0)
                        ofs<<"    \"line\": "<<pivot_begin<<",\n";
                    ofs<<"    \"type\": \""<<std::get<0>(idiom)<<"\",\n";
                    ofs<<"    \"solution\":\n     ";
                    for(char c : solution.prune().print_json(slot_tracker))
                    {
                        ofs.put(c);
                        if(c == '\n') ofs<<"     ";
                    }
                    ofs<<"\n  }";
                    first_hit1 = false;

                    if(std::get<2>(idiom))
                        std::get<2>(idiom)(function, solution);
                }
            }
        }
    }

    ofs<<"]\n}\n";
    ofs<<"****IDL Match Report End\n";

    ofs<<"****IDL Replace Source Report: replace-source-"<<filename<<".ll\n";
    std::string string_value;
    llvm::raw_string_ostream out_stream(string_value);
    out_stream<<module;
    ofs<<string_value;
    ofs<<"****IDL Replace Source Report End\n";
    // JW: I think we don't have to do this to cout.
    // ofs.close();

    return false;
}

char ResearchReplacerBase::ID = 0;

class ResearchReplacer : public ResearchReplacerBase
{
public:
    ResearchReplacer() : ResearchReplacerBase({
   {"GEMM", [](const Solution& s)->Value*{ return s["loop"]["comparison"]; }, nullptr},
   /* {"SPMV", [](const Solution& s)->Value*{ return s["loop"]["comparison"]; }, nullptr}, */
   {"SPMV_CSR", [](const Solution& s)->Value*{ return s["loop"]["comparison"]; }, nullptr},
   {"SPMV_JDS", [](const Solution& s)->Value*{ return s["loop"]["comparison"]; }, nullptr} }) { }
};

static RegisterPass<ResearchReplacer> X("research-replacer", "Research replacer", false, false);

ModulePass *llvm::createResearchReplacerPass() {
  return new ResearchReplacer();
}
