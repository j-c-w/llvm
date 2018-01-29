add_llvm_library(LLVMConstraints

  Solver.cpp
  Solution.cpp
  FunctionWrap.cpp

  BackendEdge.cpp
  BackendSingle.cpp
  BackendCollect.cpp
  BackendDominate.cpp
  BackendOrdering.cpp
  BackendSameSets.cpp
  BackendIncomingValue.cpp
  BackendSpecializations.cpp

  ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.s

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

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.cpp
                   COMMAND cat ${CMAKE_CURRENT_SOURCE_DIR}/IdiomSpecifications.txt | pypy ${CMAKE_CURRENT_SOURCE_DIR}/IdiomSpecificationGenerator.py > ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.cpp
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/IdiomSpecifications.txt ${CMAKE_CURRENT_SOURCE_DIR}/IdiomSpecificationGenerator.py
                   COMMENT "Generate idiom specifications C++ file")

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.s
                   COMMAND ${CMAKE_CXX_COMPILER} -std=c++11 -fPIC -fno-rtti -fno-exceptions -S -O0 ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.cpp -o ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.s -I ${CMAKE_CURRENT_SOURCE_DIR}/../../include/ -I ${CMAKE_CURRENT_BINARY_DIR}/../../include/
                   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.cpp
                   COMMENT "Generate idiom specifications assembly file")