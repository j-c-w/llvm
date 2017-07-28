add_llvm_library(LLVMConstraints

  SMTSolver.cpp
  PrintSlots.cpp
  FunctionWrap.cpp

  BackendOr.cpp
  BackendAnd.cpp
  BackendEdge.cpp
  BackendSingle.cpp
  BackendCollect.cpp
  BackendDominate.cpp
  BackendOrdering.cpp
  BackendIncomingValue.cpp

  ConstraintOr.cpp
  ConstraintAnd.cpp
  ConstraintCollect.cpp

  IdiomSpecifications.cpp

  CustomPassFlatten.cpp
  CustomPassReplace.cpp
  CustomPassRemovePHI.cpp
  CustomPassPreprocess.cpp
  CustomPassAnnotateBlocks.cpp

  TransformReduction.cpp
  TransformDistributive.cpp
  TransformHoistSelection.cpp

  DEPENDS
  intrinsics_gen
)
