#include "llvm/Constraints/CustomPasses.hpp"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include <functional>

class AnnotateBlocksPass : public llvm::ModulePass
{
public:
    static char ID;

    AnnotateBlocksPass() : llvm::ModulePass(ID) {}

    bool runOnModule(llvm::Module& module) override
    {
        std::hash<llvm::BasicBlock*> hash_function;

        std::vector<std::pair<decltype(hash_function(nullptr)),llvm::GlobalVariable*>> globals;

        // First collect all blocks
        for(llvm::Function& function : module.getFunctionList())
        {
            for(llvm::BasicBlock& block : function.getBasicBlockList())
            {
                auto new_global = new llvm::GlobalVariable(module, llvm::Type::getInt64Ty(module.getContext()), false,
                                                           llvm::GlobalValue::InternalLinkage,
                                            llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.getContext()), 0), "");

                llvm::Instruction* insert_before = block.getFirstNonPHI();

                new llvm::StoreInst(llvm::BinaryOperator::Create(
                                          llvm::Instruction::Add,
                                          new llvm::LoadInst(new_global, "", insert_before),
                                          llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.getContext()), 1),
                                          "", insert_before),
                                    new_global, insert_before);

                globals.emplace_back(hash_function(&block), new_global);
            }
        }

        char print_string[] = ">%lu %lu\n";

        llvm::Constant* constant_values[sizeof(print_string)];

        for(unsigned i = 0; i < sizeof(print_string); i++)
            constant_values[i] = llvm::ConstantInt::get(llvm::Type::getInt8Ty(module.getContext()), print_string[i]);

        auto* print_string_value = new llvm::GlobalVariable(module,
            llvm::ArrayType::get(llvm::Type::getInt8Ty(module.getContext()), sizeof(print_string)), true,
             llvm::GlobalValue::InternalLinkage,
          llvm::ConstantArray::get(llvm::ArrayType::get(llvm::Type::getInt8Ty(module.getContext()), sizeof(print_string)), constant_values), "");

        llvm::Function* printf_function = module.getFunction("printf");

        for(llvm::Function& function : module.getFunctionList())
        {
            if(std::string(function.getName()) == "main")
            {
                for(llvm::BasicBlock& block : function.getBasicBlockList())
                {
                    if(llvm::dyn_cast<llvm::ReturnInst>(block.getTerminator()))
                    {
                        llvm::Instruction* insert_before = block.getTerminator();

                        for(auto& pair : globals)
                        {
                            std::vector<llvm::Value*> arguments = {{
                                        llvm::GetElementPtrInst::CreateInBounds(
                                              llvm::ArrayType::get(llvm::Type::getInt8Ty(module.getContext()), sizeof(print_string)),
                                              print_string_value, 
                                              {llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.getContext()), 0),
                                               llvm::ConstantInt::get(llvm::Type::getInt32Ty(module.getContext()), 0)},
                                              "", insert_before),
                                        llvm::ConstantInt::get(llvm::Type::getInt64Ty(module.getContext()), pair.first),
                                        new llvm::LoadInst(pair.second, "", insert_before)}};

                            llvm::CallInst::Create(printf_function, arguments, "", insert_before);
                        }
                    }
                }
            }
        }

        return false;
    }
};

char AnnotateBlocksPass::ID = 0;

llvm::ModulePass *llvm::createAnnotateBlocksPass() {
  return new AnnotateBlocksPass();
}
