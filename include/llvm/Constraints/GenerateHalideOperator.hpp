#ifndef _PRINT_HALIDE_OPERATOR_
#define _PRINT_HALIDE_OPERATOR_
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Constraints/CustomPasses.hpp"
#include "llvm/Analysis/DominanceFrontierImpl.h"
#include "llvm/IR/CFG.h"
#include "llvm/Pass.h"
#include "llvm/Analysis/RegionInfo.h"
#include "llvm/Support/raw_os_ostream.h"
#include <unordered_set>

std::string print_halide_stencil(llvm::Function& function)
{
    std::string output;

    output += "typedef struct {\n";
    output += "    Halide::Func func;\n} HalideStruct\n";

    output += "void prepare_stencil(HalideStruct& halide_struct) {\n";
    output += "}\n";

    output += "void do_stencil(HalideStruct& halide_struct) {\n";
    output += "}\n";

    return output;
}

#endif