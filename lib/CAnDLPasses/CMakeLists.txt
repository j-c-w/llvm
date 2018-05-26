add_llvm_library(LLVMCAnDLPasses

  CustomPassFlangfix.cpp
  CustomPassPreprocess.cpp
  CustomPassReplace.cpp

  DEPENDS
  intrinsics_gen
)
