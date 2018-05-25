add_llvm_library(LLVMIDLPasses

  CustomPassFlangfix.cpp
  CustomPassPreprocess.cpp
  CustomPassReplace.cpp

  TransformDistributive.cpp
  TransformHoistSelection.cpp

  DEPENDS
  intrinsics_gen
)
