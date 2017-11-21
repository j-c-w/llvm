#include "llvm/Constraints/BackendSpecializations.hpp"

template class BackendConstantValue<UINT_MAX-1>;

template class BackendLLVMType<&llvm::Type::isIntegerTy>;
template class BackendLLVMType<&llvm::Type::isFloatTy>;
template class BackendLLVMType<&llvm::Type::isVectorTy>;
template class BackendLLVMType<&llvm::Type::isPointerTy>;

template class BackendOpcode<llvm::Instruction::PHI>;
template class BackendOpcode<llvm::Instruction::Store>;
template class BackendOpcode<llvm::Instruction::Load>;
template class BackendOpcode<llvm::Instruction::Ret>;
template class BackendOpcode<llvm::Instruction::Br>;
template class BackendOpcode<llvm::Instruction::Add>;
template class BackendOpcode<llvm::Instruction::Sub>;
template class BackendOpcode<llvm::Instruction::Mul>;
template class BackendOpcode<llvm::Instruction::FAdd>;
template class BackendOpcode<llvm::Instruction::FSub>;
template class BackendOpcode<llvm::Instruction::FMul>;
template class BackendOpcode<llvm::Instruction::FDiv>;
template class BackendOpcode<llvm::Instruction::Or>;
template class BackendOpcode<llvm::Instruction::And>;
template class BackendOpcode<llvm::Instruction::Shl>;
template class BackendOpcode<llvm::Instruction::Select>;
template class BackendOpcode<llvm::Instruction::SExt>;
template class BackendOpcode<llvm::Instruction::ZExt>;
template class BackendOpcode<llvm::Instruction::GetElementPtr>;
template class BackendOpcode<llvm::Instruction::ICmp>;
template class BackendOpcode<llvm::Instruction::Call>;
template class BackendOpcode<llvm::Instruction::ShuffleVector>;
template class BackendOpcode<llvm::Instruction::InsertElement>;

template class BackendOrderWrap<false,true,false>;
template class BackendOrderWrap<true,false,true>;
template class BackendOrderWrap<true,false,false>;

template class BackendLLVMEdge<&FunctionWrap::dfg, &FunctionWrap::rdfg>;
template class BackendLLVMEdge<&FunctionWrap::cfg, &FunctionWrap::rcfg>;
template class BackendLLVMEdge<&FunctionWrap::cdg, &FunctionWrap::rcdg>;
template class BackendLLVMEdge<&FunctionWrap::pdg, &FunctionWrap::rpdg>;

template class BackendLLVMOperand<0>;
template class BackendLLVMOperand<1>;
template class BackendLLVMOperand<2>;
template class BackendLLVMOperand<3>;

template class BackendLLVMDominate<false, true,  0, &FunctionWrap:: dfg>;
template class BackendLLVMDominate<false, true,  1, &FunctionWrap::rdfg>;
template class BackendLLVMDominate<false, false, 0, &FunctionWrap:: dfg>;
template class BackendLLVMDominate<false, false, 1, &FunctionWrap::rdfg>;

template class BackendLLVMDominate<false, true,  2, &FunctionWrap:: cfg>;
template class BackendLLVMDominate<false, true,  3, &FunctionWrap::rcfg>;
template class BackendLLVMDominate<false, false, 2, &FunctionWrap:: cfg>;
template class BackendLLVMDominate<false, false, 3, &FunctionWrap::rcfg>;

template class BackendLLVMDominate<false, true,  4, &FunctionWrap:: pdg>;
template class BackendLLVMDominate<false, true,  5, &FunctionWrap::rpdg>;
template class BackendLLVMDominate<false, false, 4, &FunctionWrap:: pdg>;
template class BackendLLVMDominate<false, false, 5, &FunctionWrap::rpdg>;

template class BackendLLVMDominate<true, true,  0, &FunctionWrap:: dfg>;
template class BackendLLVMDominate<true, true,  1, &FunctionWrap::rdfg>;
template class BackendLLVMDominate<true, false, 0, &FunctionWrap:: dfg>;
template class BackendLLVMDominate<true, false, 1, &FunctionWrap::rdfg>;

template class BackendLLVMDominate<true, true,  2, &FunctionWrap:: cfg>;
template class BackendLLVMDominate<true, true,  3, &FunctionWrap::rcfg>;
template class BackendLLVMDominate<true, false, 2, &FunctionWrap:: cfg>;
template class BackendLLVMDominate<true, false, 3, &FunctionWrap::rcfg>;

template class BackendLLVMDominate<true, true,  4, &FunctionWrap:: pdg>;
template class BackendLLVMDominate<true, true,  5, &FunctionWrap::rpdg>;
template class BackendLLVMDominate<true, false, 4, &FunctionWrap:: pdg>;
template class BackendLLVMDominate<true, false, 5, &FunctionWrap::rpdg>;

template class BackendLLVMDominate<false, false, UINT_MAX, &FunctionWrap::cfg>;
template class BackendLLVMDominate<false, false, UINT_MAX, &FunctionWrap::dfg>;
template class BackendLLVMDominate<false, false, UINT_MAX, &FunctionWrap::pdg>;
