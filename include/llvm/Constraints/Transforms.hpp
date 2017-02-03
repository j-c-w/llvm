#ifndef _TRANSFORMS_HPP_
#define _TRANSFORMS_HPP_
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Value.h"
#include <sstream>
#include <string>
#include <vector>

void transform_reduction_operator(llvm::Function& function, std::vector<std::map<std::string,llvm::Value*>> scalars,
                                                            std::vector<std::map<std::string,llvm::Value*>> histos);

void transform_distributive(llvm::Function& function, std::map<std::string,llvm::Value*> solution);

void transform_hoistselect_pattern(llvm::Function& wrap, std::map<std::string,llvm::Value*> solution);

#endif
