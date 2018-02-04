add_llvm_library(LLVMConstraints

  CustomPassMergePointercalcs.cpp
  CustomPassFlattenPointercalcs.cpp
  CustomPassSplitPointercalcs.cpp
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

