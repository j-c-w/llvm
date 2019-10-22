#include "llvm/CAnDL/CustomPasses.hpp"
#include "llvm/CAnDL/Solution.hpp"
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

bool ResearchReplacer::runOnModule(Module& module)
{
    ModuleSlotTracker slot_tracker(&module);

    std::string filename = module.getName();
    for(char& c : filename) if(c == '/') c = '_';

    std::stringstream sstr;
    sstr<<"replace-report-"<<filename<<".json";
    std::ofstream ofs(sstr.str().c_str());
    ofs<<"{ \"filename\": \""<<(std::string)module.getName()<<"\",\n  \"detected\": [";

    char first_hit1 = true;
    for(Function& function : module.getFunctionList())
    {
        if(!function.isDeclaration())
        {
            for(const auto& solution : GenerateAnalysis("SCoP")(function, 100))
            {
                Instruction* precursor = dyn_cast<Instruction>((Value*)solution.get("precursor"));

                unsigned line_begin = UINT_MAX;
                if(precursor && precursor->getDebugLoc())
                    line_begin = precursor->getDebugLoc().getLine();

                ofs<<(first_hit1?"\n":",\n");
                ofs<<"    {\n      \"function\": \""<<(std::string)function.getName()<<"\",\n";
                if(line_begin != UINT_MAX)
                    ofs<<"      \"line\": "<<line_begin<<",\n";
                else
                    ofs<<"      \"line\": \"???\",\n";
                ofs<<"      \"idioms\": [\n";
                ofs<<"\n    {\n      \"type\": \"SCoP\",\n";
                ofs<<"      \"solution\":\n        ";
                for(char c : solution.prune().print_json(slot_tracker))
                {
                    ofs.put(c);
                    if(c == '\n') ofs<<"        ";
                }
                ofs<<"\n    }";
                ofs<<"]";
                ofs<<"\n    }";
                first_hit1 = false;
            }
        }
    }

    ofs<<"],\n  \"transformations\": [";

    char first_hit = true;
    for(Function& function : module.getFunctionList())
    {
        if(!function.isDeclaration())
        {
            auto solutions = GenerateAnalysis("Experiment")(function, UINT_MAX);

            if(!solutions.empty())
            {
                for(auto& solution : solutions)
                {
                    ofs<<(first_hit?"\n":",\n");
                    ofs<<"    {\n      \"function\": \""<<(std::string)function.getName()<<"\",\n";
                    ofs<<"      \"type\": \"Experiment\",\n";
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

    ofs<<"]\n}\n";

    return false;
}

char ResearchReplacer::ID = 0;

static RegisterPass<ResearchReplacer> X("research-replacer", "Research replacer", false, false);

ModulePass *llvm::createResearchReplacerPass() {
  return new ResearchReplacer();
}