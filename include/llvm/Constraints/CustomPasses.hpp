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
ModulePass*   createResearchFlattenPass();
ModulePass*   createResearchPreprocessorPass();
ModulePass*   createResearchReplacerPass();
ModulePass*   createAnnotateBlocksPass();

class ResearchFlattenPass : public PassInfoMixin<ResearchFlattenPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
    return PreservedAnalyses::none();
  }
};

class ResearchPreprocessorPass : public PassInfoMixin<ResearchPreprocessorPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
    return PreservedAnalyses::none();
  }
};

class ResearchReplacerPass : public PassInfoMixin<ResearchReplacerPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM) {
    return PreservedAnalyses::none();
  }
};

}

#endif
