#include "llvm/CAnDLPasses/CustomPasses.hpp"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include <iostream>

using namespace llvm;

class ResearchEliminatePointerCasts1 : public BasicBlockPass
{
public:
    static char ID;

    ResearchEliminatePointerCasts1() : BasicBlockPass(ID) {}

    bool runOnBasicBlock(BasicBlock& module) override;
};

bool ResearchEliminatePointerCasts1::runOnBasicBlock(BasicBlock& block)
{
    DataLayout data_layout(block.getParent()->getParent());

    std::vector<std::pair<BasicBlock::iterator,std::pair<Value*,Type*>>> worklist;
    for(Instruction& instruction : block.getInstList())
    {
        Instruction* address;
        Value* precast;
        PointerType* pretype;
        PointerType* posttype;

        if(instruction.getOpcode() == Instruction::Load &&
           (address = dyn_cast<Instruction>(instruction.getOperand(0))) &&
           address->getOpcode() == Instruction::BitCast &&
           (precast = address->getOperand(0)) &&
           (pretype = dyn_cast<PointerType>(precast->getType())) &&
           (posttype = dyn_cast<PointerType>(address->getType())) &&
           data_layout.getTypeSizeInBits(pretype->getElementType())
             == data_layout.getTypeSizeInBits(posttype->getElementType()))
        {
            worklist.emplace_back(&instruction, std::make_pair(precast, posttype->getElementType()));
        }
    }

    for(auto& entry : worklist)
    {
        IRBuilder<> builder(&block, entry.first);
        ReplaceInstWithValue(block.getInstList(), entry.first,
                             builder.CreateBitCast(
                                 builder.CreateLoad(entry.second.first),
                                 entry.second.second));
    }

    return !worklist.empty();
}

char ResearchEliminatePointerCasts1::ID = 0;

static RegisterPass<ResearchEliminatePointerCasts1> X1("research-eliminate-pointer-casts1",
                                                       "Research eliminate pointer casts 1", false, false);

BasicBlockPass *llvm::createResearchEliminatePointerCasts1Pass() {
  return new ResearchEliminatePointerCasts1();
}

class ResearchEliminatePointerCasts2 : public BasicBlockPass
{
public:
    static char ID;

    ResearchEliminatePointerCasts2() : BasicBlockPass(ID) {}

    bool runOnBasicBlock(BasicBlock& module) override;
};

bool ResearchEliminatePointerCasts2::runOnBasicBlock(BasicBlock& block)
{
    std::vector<std::pair<BasicBlock::iterator,std::pair<Value*,Value*>>> worklist;
    for(Instruction& instruction : block.getInstList())
    {
        CastInst* storevalue;
        CastInst* storeaddress;
        Value* precastvalue;
        Value* precastaddress;

        if(instruction.getOpcode() == Instruction::Store &&
           (storevalue = dyn_cast<CastInst>(instruction.getOperand(0))) &&
           storevalue->getOpcode() == Instruction::BitCast &&
           (storeaddress = dyn_cast<CastInst>(instruction.getOperand(1))) &&
           storeaddress->getOpcode() == Instruction::BitCast &&
           (precastvalue = storevalue->getOperand(0)) &&
           (precastaddress = storeaddress->getOperand(0)))
        {
            worklist.emplace_back(&instruction, std::make_pair(precastvalue, precastaddress));
        }
    }

    for(auto& entry : worklist)
    {
        IRBuilder<> builder(&block, entry.first);
        ReplaceInstWithValue(block.getInstList(), entry.first,
                             builder.CreateStore(
                                 entry.second.first,
                                 builder.CreateBitCast(
                                     entry.second.second,
                                     PointerType::getUnqual(entry.second.first->getType()))));
    }

    return !worklist.empty();
}

char ResearchEliminatePointerCasts2::ID = 0;

static RegisterPass<ResearchEliminatePointerCasts2> X2("research-eliminate-pointer-casts2",
                                                       "Research eliminate pointer casts 2", false, false);

BasicBlockPass *llvm::createResearchEliminatePointerCasts2Pass() {
  return new ResearchEliminatePointerCasts2();
}

class ResearchEliminatePointerCasts3 : public BasicBlockPass
{
public:
    static char ID;

    ResearchEliminatePointerCasts3() : BasicBlockPass(ID) {}

    bool runOnBasicBlock(BasicBlock& module) override;
};

bool ResearchEliminatePointerCasts3::runOnBasicBlock(BasicBlock& block)
{
    DataLayout data_layout(block.getParent()->getParent());

    std::vector<std::pair<BasicBlock::iterator,std::pair<Value*,ConstantInt*>>> worklist;
    for(Instruction& instruction : block.getInstList())
    {
        GetElementPtrInst* gepinstr;
        PointerType*       casttype;
        Value*             address;
        PointerType*       addresstype;
        ConstantInt*       index;

        if(instruction.getOpcode() == Instruction::BitCast &&
           (casttype = dyn_cast<PointerType>(instruction.getType())) &&
           (gepinstr = dyn_cast<GetElementPtrInst>(instruction.getOperand(0))) &&
           gepinstr->getNumOperands() == 2 &&
           (address = gepinstr->getOperand(0)) &&
           (addresstype = dyn_cast<PointerType>(address->getType())) &&
           (index = dyn_cast<ConstantInt>(gepinstr->getOperand(1))))
        {
            size_t bitindex = index->getZExtValue() * data_layout.getTypeSizeInBits(addresstype->getElementType());
            size_t bitsize  = data_layout.getTypeSizeInBits(casttype->getElementType());

            if(bitindex % bitsize == 0)
            {
                ConstantInt* newinex = ConstantInt::get(index->getType(), bitindex / bitsize);
                worklist.emplace_back(&instruction, std::make_pair(address, newinex));
            }
        }
    }

    for(auto& entry : worklist)
    {
        IRBuilder<> builder(&block, entry.first);
        ReplaceInstWithValue(block.getInstList(), entry.first,
                             builder.CreateGEP(
                                 builder.CreateBitCast(
                                     entry.second.first,
                                     entry.first->getType()),
                                 entry.second.second));
    }

    return !worklist.empty();
}

char ResearchEliminatePointerCasts3::ID = 0;

static RegisterPass<ResearchEliminatePointerCasts3> X3("research-eliminate-pointer-casts3",
                                                       "Research eliminate pointer casts 3", false, false);

BasicBlockPass *llvm::createResearchEliminatePointerCasts3Pass() {
  return new ResearchEliminatePointerCasts3();
}

class ResearchEliminatePointerCasts4 : public BasicBlockPass
{
public:
    static char ID;

    ResearchEliminatePointerCasts4() : BasicBlockPass(ID) {}

    bool runOnBasicBlock(BasicBlock& module) override;
};

bool ResearchEliminatePointerCasts4::runOnBasicBlock(BasicBlock& block)
{
    DataLayout data_layout(block.getParent()->getParent());

    std::vector<std::pair<BasicBlock::iterator,std::pair<Value*,Value*>>> worklist;
    for(Instruction& instruction : block.getInstList())
    {
        Value* storevalue;
        CastInst* storeaddress;
        Value* precastaddress;
        PointerType* casttype;

        if(instruction.getOpcode() == Instruction::Store &&
           (storevalue = instruction.getOperand(0)) &&
           (storeaddress = dyn_cast<CastInst>(instruction.getOperand(1))) &&
           storeaddress->getOpcode() == Instruction::BitCast &&
           (precastaddress = storeaddress->getOperand(0)) &&
           (casttype = dyn_cast<PointerType>(precastaddress->getType())) &&
           data_layout.getTypeSizeInBits(casttype->getElementType())
               == data_layout.getTypeSizeInBits(storevalue->getType()))
            
        {
            worklist.emplace_back(&instruction, std::make_pair(storevalue, precastaddress));
        }
    }

    for(auto& entry : worklist)
    {
        IRBuilder<> builder(&block, entry.first);
        ReplaceInstWithValue(block.getInstList(), entry.first,
                             builder.CreateStore(
                                 builder.CreateBitCast(
                                     entry.second.first,
                                     dyn_cast<PointerType>(entry.second.second->getType())->getElementType()),
                                 entry.second.second));
    }

    return !worklist.empty();
}

char ResearchEliminatePointerCasts4::ID = 0;

static RegisterPass<ResearchEliminatePointerCasts4> X4("research-eliminate-pointer-casts4",
                                                       "Research eliminate pointer casts 4", false, false);

BasicBlockPass *llvm::createResearchEliminatePointerCasts4Pass() {
  return new ResearchEliminatePointerCasts4();
}

