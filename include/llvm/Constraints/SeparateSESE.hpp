#ifndef _SEPARATE_SESE_HPP_
#define _SEPARATE_SESE_HPP_
#include <unordered_map>
#include <vector>

namespace llvm {
class Instruction;
class BasicBlock;
class Function;
class Value;
}

/* This class generates a new llvm::Function from a SESE region in an existing llvm::Function. */
class SESEFunction
{
public:
    SESEFunction(llvm::Value* begin, llvm::Value* successor);

    llvm::Function* make_function();

    void transform_forw(std::vector<std::pair<std::string,llvm::Value*>>& solution);

private:

    void aggressive_dead_code_elim(std::vector<llvm::Instruction*> keep_values);

    llvm::Instruction* get_instr(llvm::Instruction* in);

    llvm::Function* old_function;

    std::vector<llvm::BasicBlock*> new_block_order;

    std::unordered_map<llvm::BasicBlock*,llvm::BasicBlock*>   block_frwd_transl;
    std::unordered_map<llvm::BasicBlock*,llvm::BasicBlock*>   block_back_transl;
    std::unordered_map<llvm::Instruction*,llvm::Instruction*> instr_frwd_transl;
    std::unordered_map<llvm::Instruction*,llvm::Instruction*> instr_back_transl;

    std::unordered_map<llvm::Value*,llvm::Value*> values_frwd_transl;
    std::unordered_map<llvm::Value*,llvm::Value*> values_back_transl;
};

#endif
