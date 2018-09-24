#include "llvm/IDL/CustomPasses.hpp"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

using namespace llvm;

class ResearchFlangfix : public FunctionPass
{
public:
    static char ID;

    ResearchFlangfix() : FunctionPass(ID) {}

    bool runOnFunction(Function& function) override;
};

bool ResearchFlangfix::runOnFunction(Function& function)
{
    return false;
}

char ResearchFlangfix::ID = 0;

static RegisterPass<ResearchFlangfix> X("research-flangfix", "Research flangfix", false, false);

FunctionPass *llvm::createResearchFlangfixPass() {
  return new ResearchFlangfix();
}
