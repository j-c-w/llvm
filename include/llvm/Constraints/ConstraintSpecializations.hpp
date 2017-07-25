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
using ConstraintUnused = ConstraintScalar<BackendConstantValue<UINT_MAX-1>,1>;

using ConstraintIntegerType = ConstraintScalar<BackendLLVMType<&llvm::Type::isIntegerTy>,1>;
using ConstraintFloatType   = ConstraintScalar<BackendLLVMType<&llvm::Type::isFloatTy>,1>;
using ConstraintPointerType = ConstraintScalar<BackendLLVMType<&llvm::Type::isPointerTy>,1>;

using ConstraintNotNumericConstant = ConstraintScalar<BackendNotNumericConstant,1>;
using ConstraintConstant           = ConstraintScalar<BackendConstant,1>;
using ConstraintPreexecution       = ConstraintScalar<BackendPreexecution,1>;
using ConstraintArgument           = ConstraintScalar<BackendArgument,1>;
using ConstraintInstruction        = ConstraintScalar<BackendInstruction,1>;
using ConstraintFloatZero          = ConstraintScalar<BackendFloatZero,1>;

using ConstraintPHIInst    = ConstraintScalar<BackendOpcode<llvm::Instruction::PHI>,1>;
using ConstraintStoreInst  = ConstraintScalar<BackendOpcode<llvm::Instruction::Store>,1>;
using ConstraintLoadInst   = ConstraintScalar<BackendOpcode<llvm::Instruction::Load>,1>;
using ConstraintReturnInst = ConstraintScalar<BackendOpcode<llvm::Instruction::Ret>,1>;
using ConstraintBranchInst = ConstraintScalar<BackendOpcode<llvm::Instruction::Br>,1>;
using ConstraintAddInst    = ConstraintScalar<BackendOpcode<llvm::Instruction::Add>,1>;
using ConstraintSubInst    = ConstraintScalar<BackendOpcode<llvm::Instruction::Sub>,1>;
using ConstraintMulInst    = ConstraintScalar<BackendOpcode<llvm::Instruction::Mul>,1>;
using ConstraintFAddInst   = ConstraintScalar<BackendOpcode<llvm::Instruction::FAdd>,1>;
using ConstraintFSubInst   = ConstraintScalar<BackendOpcode<llvm::Instruction::FSub>,1>;
using ConstraintFMulInst   = ConstraintScalar<BackendOpcode<llvm::Instruction::FMul>,1>;
using ConstraintFDivInst   = ConstraintScalar<BackendOpcode<llvm::Instruction::FDiv>,1>;
using ConstraintBitOrInst  = ConstraintScalar<BackendOpcode<llvm::Instruction::Or>,1>;
using ConstraintLShiftInst = ConstraintScalar<BackendOpcode<llvm::Instruction::Shl>,1>;
using ConstraintSelectInst = ConstraintScalar<BackendOpcode<llvm::Instruction::Select>,1>;
using ConstraintSExtInst   = ConstraintScalar<BackendOpcode<llvm::Instruction::SExt>,1>;
using ConstraintZExtInst   = ConstraintScalar<BackendOpcode<llvm::Instruction::ZExt>,1>;
using ConstraintGEPInst    = ConstraintScalar<BackendOpcode<llvm::Instruction::GetElementPtr>,1>;
using ConstraintICmpInst   = ConstraintScalar<BackendOpcode<llvm::Instruction::ICmp>,1>;

using ConstraintSame     = ConstraintScalar<BackendOrderWrap<false,true,false>,2>;
using ConstraintDistinct = ConstraintScalar<BackendOrderWrap<true,false,true>,2>;
using ConstraintOrder    = ConstraintScalar<BackendOrderWrap<true,false,false>,2>;

using ConstraintSameBlock     = ConstraintScalar<BackendSameBlock,2>;
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
