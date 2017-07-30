#ifndef _TRANSFORMS_HPP_
#define _TRANSFORMS_HPP_
#include <unordered_map>
#include <string>
#include <vector>
#include <map>

namespace llvm {
class Instruction;
class BasicBlock;
class Function;
class Value;
}

void transform_reduction_operator(llvm::Function& function, std::vector<std::map<std::string,llvm::Value*>> scalars,
                                                            std::vector<std::map<std::string,llvm::Value*>> histos);

void transform_distributive(llvm::Function& function, std::map<std::string,llvm::Value*> solution);

void transform_hoistselect_pattern(llvm::Function& wrap, std::map<std::string,llvm::Value*> solution);


std::string print_c_operator(llvm::Function& function);
std::string print_pretty_c_operator(llvm::Function& function);

inline
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
