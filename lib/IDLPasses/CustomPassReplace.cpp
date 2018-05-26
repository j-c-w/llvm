#include "llvm/IDLPasses/CustomPasses.hpp"
#include "llvm/IDLParser/Solution.hpp"
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
    ofs<<"{\"filename\": \""<<(std::string)module.getName()<<"\",\n \"detected\": [";

    char first_hit1 = true;
    for(Function& function : module.getFunctionList())
    {
        for(auto& idiom : std::vector<std::string>{"GEMM", "SPMV"})
        {
            for(auto& solution : GenerateAnalysis(idiom)(function, 100))
            {
                unsigned line_begin = 999;

                if(auto precursor = dyn_cast<Instruction>((Value*)solution["precursor"]))
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

                if(idiom == "SPMV")
                {
                    replace_spmv(function, solution);
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
