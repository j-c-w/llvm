add_llvm_library(LLVMIDLPasses

  CustomPassMergePointercalcs.cpp
  CustomPassPointerarithmetic.cpp
  CustomPassReplace.cpp
  CustomPassRemovePHI.cpp
  CustomPassPreprocess.cpp
  CustomPassFixOrToAdd.cpp
  CustomPassFixShlToMul.cpp

  TransformReduction.cpp
  TransformDistributive.cpp
  TransformHoistSelection.cpp

  SeparateSESE.cpp
  GenerateCOperator.cpp

  DEPENDS
  intrinsics_gen
)

