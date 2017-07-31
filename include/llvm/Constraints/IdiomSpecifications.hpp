#ifndef _CONSTRAINTCOMPOSED_HPP_
#define _CONSTRAINTCOMPOSED_HPP_
#include "llvm/Constraints/Solution.hpp"
#include <climits>
#include <string>
#include <vector>

namespace llvm {
class Function;
class Value;
}

// This file containts the constraint specifications that are used for idiom detection.
std::vector<Solution> DetectDistributive(llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<Solution> DetectHoistSelect (llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<Solution> DetectGEMM        (llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<Solution> DetectGEMV        (llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<Solution> DetectAXPY        (llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<Solution> DetectDOT         (llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<Solution> DetectSPMV        (llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<Solution> DetectReduction   (llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<Solution> DetectHisto       (llvm::Function& function, unsigned max_solutions = UINT_MAX);
std::vector<Solution> DetectStencil     (llvm::Function& function, unsigned max_solutions = UINT_MAX);

#endif
