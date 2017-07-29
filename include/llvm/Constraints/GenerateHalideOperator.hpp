#ifndef _PRINT_HALIDE_OPERATOR_
#define _PRINT_HALIDE_OPERATOR_
#include <string>

namespace llvm {
class Function;
}

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