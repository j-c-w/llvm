#include "llvm/IDLPasses/CustomPasses.hpp"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Transforms/Scalar.h"
#include <iostream>

using namespace llvm;

class ResearchFlangfix : public FunctionPass
{
public:
    static char ID;

    ResearchFlangfix() : FunctionPass(ID) {}

    bool runOnFunction(Function& function) override;
};

bool ResearchFlangfix::runOnFunction(Function& function)
{
    // Remove secondary induction variables.
    for(BasicBlock& block : function.getBasicBlockList())
    {
        std::vector<std::pair<PHINode*,int>> induction_variables;
    
        for(Instruction& inst : block.getInstList())
        {
            if(auto phinode = dyn_cast<PHINode>(&inst))
            {
                if(auto addinst = dyn_cast<BinaryOperator>(phinode->getOperand(1)))
                {
                    if(auto constint = dyn_cast<ConstantInt>(addinst->getOperand(1)))
                    {
                        if(phinode->getNumOperands() == 2 &&   //check same target block as well
                           addinst->getOpcode() == Instruction::Add &&
                           addinst->getOperand(0) == phinode)
                        {
                            induction_variables.emplace_back(phinode, constint->getSExtValue());
                        }
                    }
                }
            }
        }

        if(induction_variables.size() > 1 && induction_variables[0].second == 1)
        {
            for(unsigned i = 1; i < induction_variables.size(); i++)
            {
                Instruction* instruction = dyn_cast<Instruction>(induction_variables[i].first->getOperand(1));
                BasicBlock::iterator insert_point(instruction);
                IRBuilder<> builder(instruction->getParent(), insert_point);

                BasicBlock::iterator insert_point2(induction_variables[i].first);

                auto current_iteration =
                    builder.CreateSExtOrTrunc(
                        builder.CreateSub(
                            builder.CreateAdd(
                                induction_variables[0].first,
                                ConstantInt::get(induction_variables[0].first->getType(), 1), "", true, true),
                            induction_variables[0].first->getOperand(0), "", true, true),
                        induction_variables[i].first->getType());


                if(induction_variables[i].second != 1 && induction_variables[i].second != -1)
                {
                    current_iteration =
                        builder.CreateMul(
                            current_iteration,
                            ConstantInt::get(induction_variables[i].first->getType(),
                                             abs(induction_variables[i].second)), "", true, true);
                }


                Value* recomputed_next_iterator;

                if(induction_variables[i].second >= 0)
                {
                    recomputed_next_iterator =
                        builder.CreateAdd(
                            induction_variables[i].first->getOperand(0),
                            current_iteration, "", true, true);
                }
                else
                {
                    recomputed_next_iterator =
                        builder.CreateSub(
                            induction_variables[i].first->getOperand(0),
                            current_iteration, "", true, true);
                }

                auto recomputed_current_iterator =
                    builder.CreateAdd(
                        recomputed_next_iterator,
                        ConstantInt::get(induction_variables[i].first->getType(),
                                         -induction_variables[i].second), "", true, true);

                ReplaceInstWithValue(block.getInstList(), insert_point2, recomputed_current_iterator);
            }
        }
    }

    // Simplify comparisons slightly.
    for(BasicBlock& block : function.getBasicBlockList())
    {
        for(Instruction& inst : block.getInstList())
        {
            if(auto compare = dyn_cast<CmpInst>(&inst))
            {
                if(auto addition = dyn_cast<BinaryOperator>(compare->getOperand(0)))
                {
                    if(addition->getOpcode() == Instruction::Add &&
                       addition->getOperand(1) == compare->getOperand(1))
                    {
                        compare->setOperand(0, addition->getOperand(0));
                        compare->setOperand(1, ConstantInt::get(addition->getOperand(0)->getType(), 0));
                    }
                }
            }
        }
    }

    // Simplify comparison with zero and a subtraction.
    for(BasicBlock& block : function.getBasicBlockList())
    {
        for(Instruction& inst : block.getInstList())
        {
            if(auto compare = dyn_cast<CmpInst>(&inst))
            {
                if(auto subtraction = dyn_cast<BinaryOperator>(compare->getOperand(0)))
                {
                    if(auto constant = dyn_cast<Constant>(compare->getOperand(1)))
                    {
                        if(subtraction->getOpcode() == Instruction::Sub && constant->isZeroValue())
                        {
                            compare->setOperand(0, subtraction->getOperand(0));
                            compare->setOperand(1, subtraction->getOperand(1));
                        }
                    }
                }
            }
        }
    }

    // Raise comparisons with one truncated value to larger ints.
    for(BasicBlock& block : function.getBasicBlockList())
    {
        std::vector<std::tuple<BasicBlock::iterator,CmpInst::Predicate,Value*,Value*>> worklist;

        for(Instruction& inst : block.getInstList())
        {
            if(auto compare = dyn_cast<CmpInst>(&inst))
            {
                if(auto truncation = dyn_cast<TruncInst>(compare->getOperand(1)))
                {
                    worklist.emplace_back(compare, compare->getPredicate(),
                                          compare->getOperand(0), truncation->getOperand(0));
                }
            }
        }

        for(auto& entry : worklist)
        {
            IRBuilder<> builder(&block, std::get<0>(entry));

            ReplaceInstWithValue(block.getInstList(), std::get<0>(entry),
                builder.CreateICmp(std::get<1>(entry),
                    builder.CreateSExt(std::get<2>(entry), std::get<3>(entry)->getType()),
                    std::get<3>(entry)));
        }
    }

    // Raise BinaryOperations if they are extended later.
    for(BasicBlock& block : function.getBasicBlockList())
    {
        std::vector<std::tuple<BasicBlock::iterator,Instruction::BinaryOps,Value*,Value*>> worklist;

        for(Instruction& inst : block.getInstList())
        {
            if(auto extension = dyn_cast<SExtInst>(&inst))
            {
                if(auto binaryop = dyn_cast<BinaryOperator>(extension->getOperand(0)))
                {
                    worklist.emplace_back(extension, binaryop->getOpcode(),
                                          binaryop->getOperand(0), binaryop->getOperand(1));
                }
            }
        }

        for(auto& entry : worklist)
        {
            IRBuilder<> builder(&block, std::get<0>(entry));

            ReplaceInstWithValue(block.getInstList(), std::get<0>(entry),
                builder.CreateBinOp(std::get<1>(entry),
                    builder.CreateSExt(std::get<2>(entry), std::get<0>(entry)->getType()),
                    builder.CreateSExt(std::get<3>(entry), std::get<0>(entry)->getType())));
        }
    }

    // Make one based loops zero based.
    for(BasicBlock& block : function.getBasicBlockList())
    {
        std::vector<std::pair<PHINode*,BinaryOperator*>> worklist;
    
        for(Instruction& inst : block.getInstList())
        {
            if(auto phinode = dyn_cast<PHINode>(&inst))
            {
                if(auto constint1 = dyn_cast<ConstantInt>(phinode->getOperand(0)))
                {
                    if(auto addinst = dyn_cast<BinaryOperator>(phinode->getOperand(1)))
                    {
                        if(auto constint2 = dyn_cast<ConstantInt>(addinst->getOperand(1)))
                        {
                            if(phinode->getNumOperands() == 2 &&
                               addinst->getOpcode() == Instruction::Add &&
                               addinst->getOperand(0) == phinode &&
                               constint1->isOneValue() && constint2->isOneValue())
                            {
                                phinode->setOperand(0, ConstantInt::get(phinode->getType(), 0));
                                worklist.emplace_back(phinode, addinst);
                            }
                        }
                    }
                }
            }
        }

        for(auto& entry : worklist)
        {
            BasicBlock::iterator insert_point(block.getFirstNonPHI());
            IRBuilder<> builder(&block, insert_point);

            auto onebased = builder.CreateAdd(entry.first, ConstantInt::get(entry.first->getType(), 1));

            std::vector<std::pair<Instruction*, int>> newworklist;

            for(auto& use : entry.first->uses())
            {
                if(auto instruction = dyn_cast<Instruction>(use.getUser()))
                {
                    if(instruction != entry.second && instruction != onebased)
                    {
                        newworklist.emplace_back(instruction, use.getOperandNo());
                    }
                }
            }

            for(auto& entry2 : newworklist)
            {
                entry2.first->setOperand(entry2.second, onebased);
            }
        }
    }

    // Put constant offset on typed arrays.
    for(BasicBlock& block : function.getBasicBlockList())
    {
        std::vector<std::tuple<BasicBlock::iterator,BitCastInst*,Value*,Constant*>> worklist;
    
        DataLayout data_layout(block.getParent()->getParent());

        for(Instruction& inst : block.getInstList())
        {
            if(auto bitcastinst = dyn_cast<BitCastInst>(&inst))
            {
                if(auto pointertype = dyn_cast<PointerType>(bitcastinst->getType()))
                {
                    if(auto gepinst = dyn_cast<GetElementPtrInst>(bitcastinst->getOperand(0)))
                    {
                        if(auto constant = dyn_cast<ConstantInt>(gepinst->getOperand(1)))
                        {
                            if(gepinst->getNumOperands() == 2)
                            {
                                int size1 = data_layout.getTypeAllocSize(gepinst->getSourceElementType());
                                int size2 = data_layout.getTypeAllocSize(pointertype->getElementType());

                                worklist.emplace_back(bitcastinst, bitcastinst, gepinst->getOperand(0),
                                                      ConstantInt::get(constant->getType(),
                                                                       constant->getSExtValue()*size1/size2));
                            }
                        }
                    }
                }
            }
        }

        for(auto& entry : worklist)
        {
            IRBuilder<> builder(&block, std::get<0>(entry));

            ReplaceInstWithValue(block.getInstList(), std::get<0>(entry),
                builder.CreateGEP(
                    builder.CreateBitCast(std::get<2>(entry), std::get<1>(entry)->getType()),
                    std::get<3>(entry)));
        }
    }



    auto pass1 = createResearchPointerarithmeticPass();
    auto pass2 = createResearchMergePointercalcsPass();
/*
    for(auto& block : function.getBasicBlockList())
        pass1->runOnBasicBlock(block);

    for(BasicBlock& block : function.getBasicBlockList())
    {
        std::vector<std::pair<BasicBlock::iterator,Value*>> worklist;
        for(Instruction& instruction : block.getInstList())
        {
            if(auto bitcast_inst = dyn_cast<BitCastInst>(&instruction))
            {
                if(auto bitcast_inst2 = dyn_cast<BitCastInst>(bitcast_inst->getOperand(0)))
                {
                    auto origin = bitcast_inst2->getOperand(0);

                    while(auto bitcast_inst3 = dyn_cast<BitCastInst>(origin))
                    {
                        origin = bitcast_inst3->getOperand(0);
                    }

                    worklist.emplace_back(&instruction, origin);
                }
            }
        }

        for(auto& entry : worklist)
        {
            IRBuilder<> builder(&block, entry.first);
            ReplaceInstWithValue(block.getInstList(), entry.first,
                                 builder.CreateBitCast(entry.second, entry.first->getType()));
        }
    }
*/
    pass2->runOnFunction(function);
/*
    while(true)
    {
        std::vector<Instruction*> worklist;

        for(BasicBlock& block : function.getBasicBlockList())
        {
            for(Instruction& instruction : block.getInstList())
            {
                if((isa<BitCastInst>(&instruction) || isa<GetElementPtrInst>(&instruction)) && instruction.use_empty())
                {
                    worklist.push_back(&instruction);
                }
            }
        }

        for(auto instruction : worklist)
            instruction->eraseFromParent();

        if(worklist.empty())
            break;
    }
*/
    return false;
}

char ResearchFlangfix::ID = 0;

static RegisterPass<ResearchFlangfix> X("research-flangfix", "Research flangfix", false, false);

FunctionPass *llvm::createResearchFlangfixPass() {
  return new ResearchFlangfix();
}
