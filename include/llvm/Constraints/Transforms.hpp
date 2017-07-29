#ifndef _TRANSFORMS_HPP_
#define _TRANSFORMS_HPP_
#include <string>
#include <vector>
#include <map>

namespace llvm {
class Function;
class Value;
}

void transform_reduction_operator(llvm::Function& function, std::vector<std::map<std::string,llvm::Value*>> scalars,
                                                            std::vector<std::map<std::string,llvm::Value*>> histos);

void transform_distributive(llvm::Function& function, std::map<std::string,llvm::Value*> solution);

void transform_hoistselect_pattern(llvm::Function& wrap, std::map<std::string,llvm::Value*> solution);

#endif
