#ifndef _FUNCTIONWRAP_HH_
#define _FUNCTIONWRAP_HH_
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Value.h"
#include <unordered_map>
#include <vector>

/* This class is used as a wrapper around llvm::Function. It contains many convenience functions and data members in no
   particular constellation.
   The most crucial part is the mapping unsigned->{llvm::Value* instanced used by a given llvm::Function} as well as the
   extraction of data and control flow graphs. */
class FunctionWrapper
{
public:
    FunctionWrapper(FunctionWrapper& superset,
                    const std::vector<unsigned>& value_indizes,
                    const std::vector<unsigned>& instr_indizes, unsigned stop_index);
    FunctionWrapper(llvm::Function& llvm_function);

    using Graph              = std::vector<std::vector<unsigned>>;
    using InstructionHashmap = std::unordered_map<llvm::Instruction*,unsigned>;
    using ValueHashmap       = std::unordered_map<llvm::Value*,unsigned>;

    InstructionHashmap get_instruction_hashmap();
    ValueHashmap       get_value_hashmap();

    ValueHashmap gather_non_instruction_values(InstructionHashmap& instr_hash);
    void allocate_graphs();
    void construct_opcodes_from_llvm(InstructionHashmap& instr_hash);
    void construct_cdg_from_llvm(InstructionHashmap& instr_hash);
    void construct_ocfg_from_llvm(InstructionHashmap& instr_hash);
    void construct_incoming_from_llvm(InstructionHashmap& instr_hash);
    void construct_odfg_from_llvm(InstructionHashmap& instr_hash, ValueHashmap& value_hash);
    void generate_pdg();
    void assemble_subgraphs();
    void sort_graphs();

    void make_value_undefined(unsigned a);

    void construct_llvm_values_from_superset(FunctionWrapper& superset,
                                             const std::vector<unsigned>& value_indizes,
                                             const std::vector<unsigned>& instr_indizes);

    void construct_opcodes_from_superset(FunctionWrapper& superset,
                                         const std::vector<unsigned>& value_indizes,
                                         const std::vector<unsigned>& instr_indizes);

    void construct_cdg_from_superset(FunctionWrapper& superset,
                                     const std::vector<unsigned>& value_indizes,
                                     const std::vector<unsigned>& instr_indizes);

    void construct_ocfg_from_superset(FunctionWrapper& superset,
                                      const std::vector<unsigned>& value_indizes,
                                      const std::vector<unsigned>& instr_indizes, unsigned stop_index);


    void construct_incoming_from_superset(FunctionWrapper& superset,
                                          const std::vector<unsigned>& value_indizes,
                                          const std::vector<unsigned>& instr_indizes);

    void construct_odfg_from_superset(FunctionWrapper& superset,
                                      const std::vector<unsigned>& value_indizes,
                                      const std::vector<unsigned>& instr_indizes);

    std::vector<llvm::BasicBlock*> rebuild_llvm_structure(llvm::LLVMContext& llvm_context);

    llvm::Function* reimplement(llvm::LLVMContext& llvm_context, std::vector<llvm::Type*> extra_arguments);


    llvm::Value* get_value(unsigned i) const { return (i < extra_values.size())
                                                    ? extra_values[i]
                                                    : (i < extra_values.size() + instructions.size())
                                                    ? instructions[i-extra_values.size()] : nullptr; }

    llvm::Instruction* get_instruction(unsigned i) const { return (i < extra_values.size())
                                                                ? nullptr
                                                                : (i < extra_values.size() + instructions.size())
                                                                ? instructions[i-extra_values.size()] : nullptr; }

    unsigned begin()             const { return 0; }
    unsigned values_begin()      const { return 0; }
    unsigned values_end()        const { return extra_values.size(); }
    unsigned instruction_begin() const { return extra_values.size(); }
    unsigned instruction_end()   const { return extra_values.size() + instructions.size(); }
    unsigned end()               const { return extra_values.size() + instructions.size(); }

    unsigned add_binary_instruction(llvm::Instruction::BinaryOps opcode, unsigned operand1, unsigned operand2, unsigned insert_before)
    {
        llvm::Value*       llvm_operand1      = get_value(operand1);
        llvm::Value*       llvm_operand2      = get_value(operand2);
        llvm::Instruction* llvm_insert_before = llvm::dyn_cast<llvm::Instruction>(get_value(insert_before));

        opcodes.push_back(opcode);

        instructions.push_back(llvm::BinaryOperator::Create(opcode, llvm_operand1, llvm_operand2, "",
                                                            llvm_insert_before));

        return extra_values.size() + instructions.size() - 1;
    }

    unsigned get_operand(unsigned instruction, unsigned char index)
    {
        return rodfg[index][instruction][0];
    }

    void set_operand(unsigned instruction, unsigned char index, unsigned operand)
    {
        llvm::Instruction* llvm_instruction = llvm::dyn_cast<llvm::Instruction>(get_value(instruction));
        llvm::Value*       llvm_operand     = get_value(operand);

        llvm_instruction->setOperand(index, llvm_operand);
    }

    void erase_instruction(unsigned instruction)
    {
        llvm::Instruction* llvm_instruction = llvm::dyn_cast<llvm::Instruction>(get_value(instruction));
        llvm_instruction->eraseFromParent();
    }

    unsigned get_size() const { return get_num_instrs(); }

    unsigned get_num_instrs() const { return extra_values.size() + instructions.size(); }

    void duplicate_expression(unsigned expr, std::vector<unsigned> data_roots, std::vector<unsigned> control_roots);

    std::vector<llvm::Value*>       extra_values;
    std::vector<llvm::Instruction*> instructions;
    
    Graph            cfg;
    Graph            rcfg;
    Graph            ocfg[4];
    Graph            rocfg[4];
    Graph            incoming[4];
    Graph            dfg;
    Graph            rdfg;
    Graph            odfg[32];
    Graph            rodfg[32];
    Graph            cdg;
    Graph            rcdg;
    Graph            pdg;
    Graph            rpdg;
    std::vector<int> opcodes;
};

#endif
