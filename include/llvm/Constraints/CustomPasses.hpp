#ifndef _CUSTOM_PASSES_HPP_
#define _CUSTOM_PASSES_HPP_
#include "llvm/IR/PassManager.h"

namespace llvm {

class FunctionPass;
class ModulePass;

/* FlattenPass transforms all array accesses to be flat accesses to bare pointers.
   PreprocessorPass implements some additional peephole optimisations.
   ReplacerPass is the central part of this project, it uses the SMT based constraint solver to detect computational
   idioms in LLVM IR code. */
FunctionPass* createRemovePHIPass();
ModulePass*   createFlattenPass();
ModulePass*   createPreprocessorPass();
ModulePass*   createReplacerPass();
ModulePass*   createAnnotateBlocksPass();

class CustomFlattenPass : public llvm::ModulePass
{
public:
    static char ID;

    CustomFlattenPass() : llvm::ModulePass(ID) {}

    bool runOnModule(llvm::Module& module) override;
};

class CustomPreprocessorPass : public llvm::ModulePass
{
public:
    static char ID;

    CustomPreprocessorPass() : llvm::ModulePass(ID) {}

    bool runOnModule(llvm::Module& module) override;
};

class CustomReplacerPass : public llvm::ModulePass
{
public:
    static char ID;

    CustomReplacerPass() : llvm::ModulePass(ID) {}

    bool runOnModule(llvm::Module& module) override;
};

class FlattenPass : public PassInfoMixin<FlattenPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
    return PreservedAnalyses::none();
  }
};

class PreprocessorPass : public PassInfoMixin<PreprocessorPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
    return PreservedAnalyses::none();
  }
};

class ReplacerPass : public PassInfoMixin<ReplacerPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
    return PreservedAnalyses::none();
  }
};

}

#endif
