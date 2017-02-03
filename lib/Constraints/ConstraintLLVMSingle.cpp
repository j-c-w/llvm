#include "llvm/Constraints/ConstraintLLVMSingle.hpp"
#include "llvm/Constraints/ConstraintSingle.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Value.h"

ConstraintLLVMSingle::ConstraintLLVMSingle(FunctionWrapper& w, std::string v, std::function<bool(llvm::Value&)> p)
                    : ConstraintSingle<std::string,unsigned>(v, compute_hits(w, p)) { }

ConstraintLLVMSingle::ConstraintLLVMSingle(FunctionWrapper& w, std::string v, std::function<bool(llvm::Instruction&)> p)
                    : ConstraintSingle<std::string,unsigned>(v, compute_hits(w, p)) { }

ConstraintLLVMSingle::ConstraintLLVMSingle(FunctionWrapper& w, std::string v, std::function<bool(llvm::Constant&)> p)
                    : ConstraintSingle<std::string,unsigned>(v, compute_hits(w, p)) { }

std::vector<unsigned> ConstraintLLVMSingle::compute_hits(FunctionWrapper& w, std::function<bool(llvm::Instruction&)> p)
{
    std::vector<unsigned> hits;

    for(unsigned i = w.extra_values.size(); i < w.get_size(); i++)
    {
        llvm::Instruction* instruction = llvm::dyn_cast<llvm::Instruction>(w.get_value(i));

        if(instruction && p(*instruction))
        {
            hits.push_back(i);
        }
    }
    return hits;
}

std::vector<unsigned> ConstraintLLVMSingle::compute_hits(FunctionWrapper& w, std::function<bool(llvm::Value&)> p)
{
    std::vector<unsigned> hits;

    for(unsigned i = 0; i < w.get_size(); i++)
    {
        if(p(*w.get_value(i)))
        {
            hits.push_back(i);
        }
    }
    return hits;
}

std::vector<unsigned> ConstraintLLVMSingle::compute_hits(FunctionWrapper& w, std::function<bool(llvm::Constant&)> p)
{
    std::vector<unsigned> hits;

    for(unsigned i = 0; i < w.extra_values.size(); i++)
    {
        llvm::Constant* constant = llvm::dyn_cast<llvm::Constant>(w.get_value(i));

        if(constant && p(*constant))
        {
            hits.push_back(i);
        }
    }
    return hits;
}
