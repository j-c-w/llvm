#include "llvm/Constraints/CustomPasses.hpp"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"

class RemovePHIPass : public llvm::FunctionPass
{
public:
    static char ID;

    RemovePHIPass() : llvm::FunctionPass(ID) {}

    bool runOnFunction(llvm::Function& function) override
    {
        if(function.getBasicBlockList().size() > 0 && function.getBasicBlockList().front().getInstList().size() > 0)
        {
            llvm::Instruction& first_instr = function.getBasicBlockList().front().getInstList().front();

            std::vector<llvm::PHINode*> phi_nodes;

            for(auto& block : function.getBasicBlockList())
            {
                for(auto& instr : block.getInstList())
                {
                    if(auto* phi_cast = llvm::dyn_cast<llvm::PHINode>(&instr))
                    {
                        auto* alloca_inst = new llvm::AllocaInst(phi_cast->getType(), 0, "", &first_instr);

                        for(unsigned i = 0; i < phi_cast->getNumIncomingValues(); i++)
                        {
                            new llvm::StoreInst(phi_cast->getIncomingValue(i),
                                                alloca_inst, phi_cast->getIncomingBlock(i)->getTerminator());
                        }

                        std::vector<std::pair<llvm::User*,unsigned>> user_pairs;
                        for(auto it = phi_cast->user_begin(); it != phi_cast->user_end(); ++it)
                        {
                            for(unsigned i = 0; i < it->getNumOperands(); i++)
                            {
                                if(it->getOperand(i) == phi_cast)
                                {
                                    user_pairs.emplace_back(*it, i);
                                }
                            }
                        }

                        for(auto pair : user_pairs)
                        {
                            pair.first->setOperand(pair.second, new llvm::LoadInst(alloca_inst, "", phi_cast));
                        }

                        phi_nodes.push_back(phi_cast);
                    }
                }
            }

            for(auto phi_node : phi_nodes)
            {
                phi_node->eraseFromParent();
            }
        }

        return false;
    }
};

char RemovePHIPass::ID = 0;

llvm::FunctionPass *llvm::createRemovePHIPass() {
  return new RemovePHIPass();
}
