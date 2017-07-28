#ifndef _CONSTRAINT_SPECIALIZATIONS_HPP_
#define _CONSTRAINT_SPECIALIZATIONS_HPP_
#include "llvm/Constraints/BackendSpecializations.hpp"
#include "llvm/Constraints/ConstraintClasses.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/Constraints/GraphEngine.hpp"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Type.h"

/* In this file, many individual constraints are defined that are simply specializations of some of the very genric
   constraint definitions in the included files. */
using ConstraintUnused = ConstraintSingle<BackendConstantValue<UINT_MAX-1>>;

using ConstraintIntegerType = ConstraintSingle<BackendLLVMType<&llvm::Type::isIntegerTy>>;
using ConstraintFloatType   = ConstraintSingle<BackendLLVMType<&llvm::Type::isFloatTy>>;
using ConstraintPointerType = ConstraintSingle<BackendLLVMType<&llvm::Type::isPointerTy>>;

using ConstraintNotNumericConstant = ConstraintSingle<BackendNotNumericConstant>;
using ConstraintConstant           = ConstraintSingle<BackendConstant>;
using ConstraintPreexecution       = ConstraintSingle<BackendPreexecution>;
using ConstraintArgument           = ConstraintSingle<BackendArgument>;
using ConstraintInstruction        = ConstraintSingle<BackendInstruction>;
using ConstraintFloatZero          = ConstraintSingle<BackendFloatZero>;

using ConstraintPHIInst    = ConstraintSingle<BackendOpcode<llvm::Instruction::PHI>>;
using ConstraintStoreInst  = ConstraintSingle<BackendOpcode<llvm::Instruction::Store>>;
using ConstraintLoadInst   = ConstraintSingle<BackendOpcode<llvm::Instruction::Load>>;
using ConstraintReturnInst = ConstraintSingle<BackendOpcode<llvm::Instruction::Ret>>;
using ConstraintBranchInst = ConstraintSingle<BackendOpcode<llvm::Instruction::Br>>;
using ConstraintAddInst    = ConstraintSingle<BackendOpcode<llvm::Instruction::Add>>;
using ConstraintSubInst    = ConstraintSingle<BackendOpcode<llvm::Instruction::Sub>>;
using ConstraintMulInst    = ConstraintSingle<BackendOpcode<llvm::Instruction::Mul>>;
using ConstraintFAddInst   = ConstraintSingle<BackendOpcode<llvm::Instruction::FAdd>>;
using ConstraintFSubInst   = ConstraintSingle<BackendOpcode<llvm::Instruction::FSub>>;
using ConstraintFMulInst   = ConstraintSingle<BackendOpcode<llvm::Instruction::FMul>>;
using ConstraintFDivInst   = ConstraintSingle<BackendOpcode<llvm::Instruction::FDiv>>;
using ConstraintBitOrInst  = ConstraintSingle<BackendOpcode<llvm::Instruction::Or>>;
using ConstraintLShiftInst = ConstraintSingle<BackendOpcode<llvm::Instruction::Shl>>;
using ConstraintSelectInst = ConstraintSingle<BackendOpcode<llvm::Instruction::Select>>;
using ConstraintSExtInst   = ConstraintSingle<BackendOpcode<llvm::Instruction::SExt>>;
using ConstraintZExtInst   = ConstraintSingle<BackendOpcode<llvm::Instruction::ZExt>>;
using ConstraintGEPInst    = ConstraintSingle<BackendOpcode<llvm::Instruction::GetElementPtr>>;
using ConstraintICmpInst   = ConstraintSingle<BackendOpcode<llvm::Instruction::ICmp>>;

using ConstraintSame     = ConstraintScalar<BackendOrderWrap<false,true,false>,2>;
using ConstraintDistinct = ConstraintScalar<BackendOrderWrap<true,false,true>,2>;
using ConstraintOrder    = ConstraintScalar<BackendOrderWrap<true,false,false>,2>;

using ConstraintIncomingValue = ConstraintScalar<BackendIncomingValue,3>;

using ConstraintDFGEdge  = ConstraintScalar<BackendLLVMEdge<&FunctionWrap::dfg, &FunctionWrap::rdfg>,2>;
using ConstraintCFGEdge  = ConstraintScalar<BackendLLVMEdge<&FunctionWrap::cfg, &FunctionWrap::rcfg>,2>;
using ConstraintCDGEdge  = ConstraintScalar<BackendLLVMEdge<&FunctionWrap::cdg, &FunctionWrap::rcdg>,2>;
using ConstraintPDGEdge  = ConstraintScalar<BackendLLVMEdge<&FunctionWrap::pdg, &FunctionWrap::rpdg>,2>;

using ConstraintFirstOperand  = ConstraintScalar<BackendLLVMOperand<0>,2>;
using ConstraintSecondOperand = ConstraintScalar<BackendLLVMOperand<1>,2>;
using ConstraintThirdOperand  = ConstraintScalar<BackendLLVMOperand<2>,2>;
using ConstraintFourthOperand = ConstraintScalar<BackendLLVMOperand<3>,2>;

using ConstraintDFGDominate       = ConstraintVector<BackendLLVMDominate<false, true,  0, &FunctionWrap:: dfg>,3>;
using ConstraintDFGPostdom        = ConstraintVector<BackendLLVMDominate<false, true,  1, &FunctionWrap::rdfg>,3>;
using ConstraintDFGDominateStrict = ConstraintVector<BackendLLVMDominate<false, false, 0, &FunctionWrap:: dfg>,3>;
using ConstraintDFGPostdomStrict  = ConstraintVector<BackendLLVMDominate<false, false, 1, &FunctionWrap::rdfg>,3>;

using ConstraintCFGDominate       = ConstraintVector<BackendLLVMDominate<false, true,  2, &FunctionWrap:: cfg>,3>;
using ConstraintCFGPostdom        = ConstraintVector<BackendLLVMDominate<false, true,  3, &FunctionWrap::rcfg>,3>;
using ConstraintCFGDominateStrict = ConstraintVector<BackendLLVMDominate<false, false, 2, &FunctionWrap:: cfg>,3>;
using ConstraintCFGPostdomStrict  = ConstraintVector<BackendLLVMDominate<false, false, 3, &FunctionWrap::rcfg>,3>;

using ConstraintPDGDominate       = ConstraintVector<BackendLLVMDominate<false, true,  4, &FunctionWrap:: pdg>,3>;
using ConstraintPDGPostdom        = ConstraintVector<BackendLLVMDominate<false, true,  5, &FunctionWrap::rpdg>,3>;
using ConstraintPDGDominateStrict = ConstraintVector<BackendLLVMDominate<false, false, 4, &FunctionWrap:: pdg>,3>;
using ConstraintPDGPostdomStrict  = ConstraintVector<BackendLLVMDominate<false, false, 5, &FunctionWrap::rpdg>,3>;

using ConstraintDFGNotDominate       = ConstraintVector<BackendLLVMDominate<true, true,  0, &FunctionWrap:: dfg>,3>;
using ConstraintDFGNotPostdom        = ConstraintVector<BackendLLVMDominate<true, true,  1, &FunctionWrap::rdfg>,3>;
using ConstraintDFGNotDominateStrict = ConstraintVector<BackendLLVMDominate<true, false, 0, &FunctionWrap:: dfg>,3>;
using ConstraintDFGNotPostdomStrict  = ConstraintVector<BackendLLVMDominate<true, false, 1, &FunctionWrap::rdfg>,3>;

using ConstraintCFGNotDominate       = ConstraintVector<BackendLLVMDominate<true, true,  2, &FunctionWrap:: cfg>,3>;
using ConstraintCFGNotPostdom        = ConstraintVector<BackendLLVMDominate<true, true,  3, &FunctionWrap::rcfg>,3>;
using ConstraintCFGNotDominateStrict = ConstraintVector<BackendLLVMDominate<true, false, 2, &FunctionWrap:: cfg>,3>;
using ConstraintCFGNotPostdomStrict  = ConstraintVector<BackendLLVMDominate<true, false, 3, &FunctionWrap::rcfg>,3>;

using ConstraintPDGNotDominate       = ConstraintVector<BackendLLVMDominate<true, true,  4, &FunctionWrap:: pdg>,3>;
using ConstraintPDGNotPostdom        = ConstraintVector<BackendLLVMDominate<true, true,  5, &FunctionWrap::rpdg>,3>;
using ConstraintPDGNotDominateStrict = ConstraintVector<BackendLLVMDominate<true, false, 4, &FunctionWrap:: pdg>,3>;
using ConstraintPDGNotPostdomStrict  = ConstraintVector<BackendLLVMDominate<true, false, 5, &FunctionWrap::rpdg>,3>;

using ConstraintCFGBlocked = ConstraintVector<BackendLLVMDominate<false, false, UINT_MAX, &FunctionWrap::cfg>,3>;
using ConstraintDFGBlocked = ConstraintVector<BackendLLVMDominate<false, false, UINT_MAX, &FunctionWrap::dfg>,3>;
using ConstraintPDGBlocked = ConstraintVector<BackendLLVMDominate<false, false, UINT_MAX, &FunctionWrap::pdg>,3>;

#endif
