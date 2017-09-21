add_llvm_library(LLVMConstraints

  Solver.cpp
  Solution.cpp
  FunctionWrap.cpp

  BackendOr.cpp
  BackendAnd.cpp
  BackendEdge.cpp
  BackendSingle.cpp
  BackendCollect.cpp
  BackendDominate.cpp
  BackendOrdering.cpp
  BackendIncomingValue.cpp

  ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.cpp

  CustomPassFlatten.cpp
  CustomPassReplace.cpp
  CustomPassRemovePHI.cpp
  CustomPassPreprocess.cpp
  CustomPassAnnotateBlocks.cpp

  TransformReduction.cpp
  TransformDistributive.cpp
  TransformHoistSelection.cpp

  SeparateSESE.cpp
  GenerateCOperator.cpp

  DEPENDS
  intrinsics_gen
)

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.cpp
                   COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/IdiomSpecifications.txt | pypy ${CMAKE_CURRENT_SOURCE_DIR}/IdiomSpecificationGenerator.py > ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.cpp
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/IdiomSpecifications.txt ${CMAKE_CURRENT_SOURCE_DIR}/IdiomSpecificationGenerator.py
                   COMMENT "Generate idiom specifications")
