#ifndef _CONSTRAINTCOMPOSED_HPP_
#define _CONSTRAINTCOMPOSED_HPP_
#include <climits>
#include <string>
#include <vector>

namespace llvm {
class Function;
class Value;
}

using IdiomInstance = std::vector<std::pair<std::string,llvm::Value*>>;

// This file containts the constraint specifications that are used for idiom detection.
std::vector<IdiomInstance> DetectDistributive(llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<IdiomInstance> DetectHoistSelect (llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<IdiomInstance> DetectGEMM        (llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<IdiomInstance> DetectGEMV        (llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<IdiomInstance> DetectAXPY        (llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<IdiomInstance> DetectDOT         (llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<IdiomInstance> DetectSPMV        (llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<IdiomInstance> DetectReduction   (llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<IdiomInstance> DetectHisto       (llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<IdiomInstance> DetectStencil     (llvm::Function& function, unsigned max_solutions = UINT_MAX);

#endif
