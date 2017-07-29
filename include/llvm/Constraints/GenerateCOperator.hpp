#ifndef _GENERATE_C_OPERATOR_
#define _GENERATE_C_OPERATOR_
#include <string>

namespace llvm {
class Function;
}

std::string print_c_operator(llvm::Function& function);
std::string print_pretty_c_operator(llvm::Function& function);

#endif