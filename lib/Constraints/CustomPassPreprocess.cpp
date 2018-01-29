#include "llvm/Constraints/CustomPasses.hpp"
#include "llvm/Constraints/IdiomSpecifications.hpp"
#include "llvm/Constraints/Transforms.hpp"
#include "llvm/IR/ModuleSlotTracker.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace llvm;

class ResearchPreprocessor : public ModulePass
{
public:
    static char ID;

    ResearchPreprocessor() : ModulePass(ID),
                         constraint_specs{{"HOISTSELECT",  DetectHoistSelect,  &transform_hoistselect_pattern},
                                          {"DISTRIBUTIVE", DetectDistributive, &transform_distributive}} { }

    bool runOnModule(Module& module) override;

private:
    std::vector<std::tuple<std::string,std::vector<Solution>(*)(llvm::Function&,unsigned),
                                       void(*)(Function&,std::map<std::string,Value*>)>> constraint_specs;
};

bool ResearchPreprocessor::runOnModule(Module& module)
{
    ModuleSlotTracker slot_tracker(&module);

    std::stringstream sstr;
    sstr<<"preprocess-report-"<<(std::string)module.getName()<<".json";
    std::ofstream ofs(sstr.str().c_str());
    ofs<<"{\n  \"filename\": \""<<(std::string)module.getName()<<"\",\n  \"transformations\": [";

    char first_hit = true;
    for(Function& function : module.getFunctionList())
    {
        if(!function.isDeclaration())
        {
            for(const auto& spec : constraint_specs)
            {
                auto solutions = std::get<1>(spec)(function, UINT_MAX);

                if(!solutions.empty())
                {
                    // This is NOT safe!
                    // Solutions might overlap and then this is entirely unsafe, resulting in invalid pointers!
                    // Need to fix this.
                    for(auto& solution : solutions)
                    {
                        ofs<<(first_hit?"\n":",\n");
                        ofs<<"    {\n      \"function\": \""<<(std::string)function.getName()<<"\",\n";
                        ofs<<"      \"type\": \""<<std::get<0>(spec)<<"\",\n";
                        ofs<<"      \"solution\":\n        ";
                        for(char c : solution.prune().print_json(slot_tracker))
                        {
                            ofs.put(c);
                            if(c == '\n') ofs<<"        ";
                        }
                        ofs<<"\n    }";

                        (*std::get<2>(spec))(function, solution);
                        first_hit = false;
                    }
                }
            }
        }
    }

    ofs<<"]\n}\n";

    return false;
}

char ResearchPreprocessor::ID = 0;

static RegisterPass<ResearchPreprocessor> X("research-preprocessor", "Research preprocessor", false, false);

ModulePass *llvm::createResearchPreprocessorPass() {
  return new ResearchPreprocessor();
}
