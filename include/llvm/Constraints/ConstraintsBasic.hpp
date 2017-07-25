#ifndef _CONSTRAINTS_BASIC_HPP_
#define _CONSTRAINTS_BASIC_HPP_
#include "llvm/Constraints/BackendSpecializations.hpp"
#include "llvm/Constraints/ConstraintAtomic.hpp"
#include "llvm/Constraints/FunctionWrap.hpp"
#include "llvm/Constraints/GraphEngine.hpp"
#include "llvm/IR/Function.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Type.h"

/* In this file, many individual constraints are defined that are simply specializations of some of the very genric
   constraint definitions in the included files. */
using ConstraintUnused = ConstraintScalar<BackendConstantValue_<UINT_MAX-1>,1>;

using ConstraintIntegerType = ConstraintScalar<BackendLLVMType_<&llvm::Type::isIntegerTy>,1>;
using ConstraintFloatType   = ConstraintScalar<BackendLLVMType_<&llvm::Type::isFloatTy>,1>;
using ConstraintPointerType = ConstraintScalar<BackendLLVMType_<&llvm::Type::isPointerTy>,1>;

using ConstraintNotNumericConstant = ConstraintScalar<BackendNotNumericConstant_,1>;
using ConstraintConstant           = ConstraintScalar<BackendConstant_,1>;
using ConstraintPreexecution       = ConstraintScalar<BackendPreexecution_,1>;
using ConstraintArgument           = ConstraintScalar<BackendArgument_,1>;
using ConstraintInstruction        = ConstraintScalar<BackendInstruction_,1>;
using ConstraintFloatZero          = ConstraintScalar<BackendFloatZero_,1>;

using ConstraintPHIInst    = ConstraintScalar<BackendOpcode_<llvm::Instruction::PHI>,1>;
using ConstraintStoreInst  = ConstraintScalar<BackendOpcode_<llvm::Instruction::Store>,1>;
using ConstraintLoadInst   = ConstraintScalar<BackendOpcode_<llvm::Instruction::Load>,1>;
using ConstraintReturnInst = ConstraintScalar<BackendOpcode_<llvm::Instruction::Ret>,1>;
using ConstraintBranchInst = ConstraintScalar<BackendOpcode_<llvm::Instruction::Br>,1>;
using ConstraintAddInst    = ConstraintScalar<BackendOpcode_<llvm::Instruction::Add>,1>;
using ConstraintSubInst    = ConstraintScalar<BackendOpcode_<llvm::Instruction::Sub>,1>;
using ConstraintMulInst    = ConstraintScalar<BackendOpcode_<llvm::Instruction::Mul>,1>;
using ConstraintFAddInst   = ConstraintScalar<BackendOpcode_<llvm::Instruction::FAdd>,1>;
using ConstraintFSubInst   = ConstraintScalar<BackendOpcode_<llvm::Instruction::FSub>,1>;
using ConstraintFMulInst   = ConstraintScalar<BackendOpcode_<llvm::Instruction::FMul>,1>;
using ConstraintFDivInst   = ConstraintScalar<BackendOpcode_<llvm::Instruction::FDiv>,1>;
using ConstraintBitOrInst  = ConstraintScalar<BackendOpcode_<llvm::Instruction::Or>,1>;
using ConstraintLShiftInst = ConstraintScalar<BackendOpcode_<llvm::Instruction::Shl>,1>;
using ConstraintSelectInst = ConstraintScalar<BackendOpcode_<llvm::Instruction::Select>,1>;
using ConstraintSExtInst   = ConstraintScalar<BackendOpcode_<llvm::Instruction::SExt>,1>;
using ConstraintZExtInst   = ConstraintScalar<BackendOpcode_<llvm::Instruction::ZExt>,1>;
using ConstraintGEPInst    = ConstraintScalar<BackendOpcode_<llvm::Instruction::GetElementPtr>,1>;
using ConstraintICmpInst   = ConstraintScalar<BackendOpcode_<llvm::Instruction::ICmp>,1>;

using ConstraintSame     = ConstraintScalar<BackendOrderWrap_<false,true,false>,2>;
using ConstraintDistinct = ConstraintScalar<BackendOrderWrap_<true,false,true>,2>;
using ConstraintOrder    = ConstraintScalar<BackendOrderWrap_<true,false,false>,2>;

using ConstraintSameBlock     = ConstraintScalar<BackendSameBlock_,2>;
using ConstraintIncomingValue = ConstraintScalar<BackendIncomingValue_,3>;

using ConstraintDFGEdge  = ConstraintScalar<BackendLLVMEdge_<&FunctionWrapper::dfg, &FunctionWrapper::rdfg>,2>;
using ConstraintCFGEdge  = ConstraintScalar<BackendLLVMEdge_<&FunctionWrapper::cfg, &FunctionWrapper::rcfg>,2>;
using ConstraintCDGEdge  = ConstraintScalar<BackendLLVMEdge_<&FunctionWrapper::cdg, &FunctionWrapper::rcdg>,2>;
using ConstraintPDGEdge  = ConstraintScalar<BackendLLVMEdge_<&FunctionWrapper::pdg, &FunctionWrapper::rpdg>,2>;

using ConstraintFirstArgument  = ConstraintScalar<BackendLLVMArgument_<0, 32, &FunctionWrapper::odfg, &FunctionWrapper::rodfg>,2>;
using ConstraintSecondArgument = ConstraintScalar<BackendLLVMArgument_<1, 32, &FunctionWrapper::odfg, &FunctionWrapper::rodfg>,2>;
using ConstraintThirdArgument  = ConstraintScalar<BackendLLVMArgument_<2, 32, &FunctionWrapper::odfg, &FunctionWrapper::rodfg>,2>;
using ConstraintFourthArgument = ConstraintScalar<BackendLLVMArgument_<3, 32, &FunctionWrapper::odfg, &FunctionWrapper::rodfg>,2>;

using ConstraintDFGDominate       = ConstraintVector<BackendLLVMDominate_<false, true,  0, &FunctionWrapper:: dfg>,3>;
using ConstraintDFGPostdom        = ConstraintVector<BackendLLVMDominate_<false, true,  1, &FunctionWrapper::rdfg>,3>;
using ConstraintDFGDominateStrict = ConstraintVector<BackendLLVMDominate_<false, false, 0, &FunctionWrapper:: dfg>,3>;
using ConstraintDFGPostdomStrict  = ConstraintVector<BackendLLVMDominate_<false, false, 1, &FunctionWrapper::rdfg>,3>;

using ConstraintCFGDominate       = ConstraintVector<BackendLLVMDominate_<false, true,  2, &FunctionWrapper:: cfg>,3>;
using ConstraintCFGPostdom        = ConstraintVector<BackendLLVMDominate_<false, true,  3, &FunctionWrapper::rcfg>,3>;
using ConstraintCFGDominateStrict = ConstraintVector<BackendLLVMDominate_<false, false, 2, &FunctionWrapper:: cfg>,3>;
using ConstraintCFGPostdomStrict  = ConstraintVector<BackendLLVMDominate_<false, false, 3, &FunctionWrapper::rcfg>,3>;

using ConstraintPDGDominate       = ConstraintVector<BackendLLVMDominate_<false, true,  4, &FunctionWrapper:: pdg>,3>;
using ConstraintPDGPostdom        = ConstraintVector<BackendLLVMDominate_<false, true,  5, &FunctionWrapper::rpdg>,3>;
using ConstraintPDGDominateStrict = ConstraintVector<BackendLLVMDominate_<false, false, 4, &FunctionWrapper:: pdg>,3>;
using ConstraintPDGPostdomStrict  = ConstraintVector<BackendLLVMDominate_<false, false, 5, &FunctionWrapper::rpdg>,3>;

using ConstraintDFGNotDominate       = ConstraintVector<BackendLLVMDominate_<true, true,  0, &FunctionWrapper:: dfg>,3>;
using ConstraintDFGNotPostdom        = ConstraintVector<BackendLLVMDominate_<true, true,  1, &FunctionWrapper::rdfg>,3>;
using ConstraintDFGNotDominateStrict = ConstraintVector<BackendLLVMDominate_<true, false, 0, &FunctionWrapper:: dfg>,3>;
using ConstraintDFGNotPostdomStrict  = ConstraintVector<BackendLLVMDominate_<true, false, 1, &FunctionWrapper::rdfg>,3>;

using ConstraintCFGNotDominate       = ConstraintVector<BackendLLVMDominate_<true, true,  2, &FunctionWrapper:: cfg>,3>;
using ConstraintCFGNotPostdom        = ConstraintVector<BackendLLVMDominate_<true, true,  3, &FunctionWrapper::rcfg>,3>;
using ConstraintCFGNotDominateStrict = ConstraintVector<BackendLLVMDominate_<true, false, 2, &FunctionWrapper:: cfg>,3>;
using ConstraintCFGNotPostdomStrict  = ConstraintVector<BackendLLVMDominate_<true, false, 3, &FunctionWrapper::rcfg>,3>;

using ConstraintPDGNotDominate       = ConstraintVector<BackendLLVMDominate_<true, true,  4, &FunctionWrapper:: pdg>,3>;
using ConstraintPDGNotPostdom        = ConstraintVector<BackendLLVMDominate_<true, true,  5, &FunctionWrapper::rpdg>,3>;
using ConstraintPDGNotDominateStrict = ConstraintVector<BackendLLVMDominate_<true, false, 4, &FunctionWrapper:: pdg>,3>;
using ConstraintPDGNotPostdomStrict  = ConstraintVector<BackendLLVMDominate_<true, false, 5, &FunctionWrapper::rpdg>,3>;

using ConstraintCFGBlocked = ConstraintVector<BackendLLVMDominate_<false, false, UINT_MAX, &FunctionWrapper::cfg>,3>;
using ConstraintDFGBlocked = ConstraintVector<BackendLLVMDominate_<false, false, UINT_MAX, &FunctionWrapper::dfg>,3>;
using ConstraintPDGBlocked = ConstraintVector<BackendLLVMDominate_<false, false, UINT_MAX, &FunctionWrapper::pdg>,3>;

#endif
