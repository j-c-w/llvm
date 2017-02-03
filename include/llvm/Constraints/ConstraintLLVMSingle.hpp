#ifndef _CONSTRAINT_LLVM_SINGLE_HPP_
#define _CONSTRAINT_LLVM_SINGLE_HPP_
#include "llvm/Constraints/ConstraintSingle.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Value.h"

/* This class implements constraints on individual variables that restrict the type of LLVM IR value that is accepted.
   This can be used to filter for specific instructions, data types etc. */
class ConstraintLLVMSingle : public ConstraintSingle<std::string,unsigned>
{
public:
    ConstraintLLVMSingle(FunctionWrapper& wrapper, std::string variable, std::function<bool(llvm::Value&)>       pred);
    ConstraintLLVMSingle(FunctionWrapper& wrapper, std::string variable, std::function<bool(llvm::Instruction&)> pred);
    ConstraintLLVMSingle(FunctionWrapper& wrapper, std::string variable, std::function<bool(llvm::Constant&)>    pred);

private:
    static std::vector<unsigned> compute_hits(FunctionWrapper& wrapper, std::function<bool(llvm::Instruction&)> pred);
    static std::vector<unsigned> compute_hits(FunctionWrapper& wrapper, std::function<bool(llvm::Value&)>       pred);
    static std::vector<unsigned> compute_hits(FunctionWrapper& wrapper, std::function<bool(llvm::Constant&)>    pred);
};

#endif
