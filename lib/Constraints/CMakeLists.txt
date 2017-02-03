add_llvm_library(Constraints

  SMTSolver.cpp
  PrintSlots.cpp
  GraphEngine.cpp
  FunctionWrap.cpp

  ConstraintOr.cpp
  ConstraintAnd.cpp
  ConstraintEdge.cpp
  ConstraintRange.cpp
  ConstraintSingle.cpp
  ConstraintDominate.cpp
  ConstraintOrdering.cpp
  ConstraintLLVMSingle.cpp
  ConstraintSharedFate.cpp

  ConstraintSpecifications.cpp

  CustomPassFlatten.cpp
  CustomPassReplace.cpp
  CustomPassPreprocess.cpp

  TransformReduction.cpp
  TransformDistributive.cpp
  TransformHoistSelection.cpp
)

