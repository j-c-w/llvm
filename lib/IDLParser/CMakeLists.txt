add_llvm_library(LLVMIDLParser
  IdiomSpecifications.s

  DEPENDS
  intrinsics_gen
)

add_library(spmv_naive SHARED
    spmv_csr_naive.cc
)

add_library(mm_naive SHARED
    mm_naive.cc
)

add_library(mm_openblas SHARED
    mm_openblas.cc
)
target_link_libraries(mm_openblas openblas)


ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/spmv_csr_naive.cc ${CMAKE_CURRENT_BINARY_DIR}/spmv_csr_mkl.cc ${CMAKE_CURRENT_BINARY_DIR}/spmv_csr_cuda.cc
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/stage0.py ${CMAKE_CURRENT_SOURCE_DIR}/abstract.txt
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/stage0.py ${CMAKE_CURRENT_SOURCE_DIR}/abstract.txt
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "Generate spmv harnesses")

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mm_naive.cc ${CMAKE_CURRENT_BINARY_DIR}/mm_openblas.cc ${CMAKE_CURRENT_BINARY_DIR}/mm_cuda.cc
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/stage0.py ${CMAKE_CURRENT_SOURCE_DIR}/abstract2.txt
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/stage0.py ${CMAKE_CURRENT_SOURCE_DIR}/abstract2.txt
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "Generate mm harnesses")

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/BNFParser
                   COMMAND ghc ${CMAKE_CURRENT_SOURCE_DIR}/stage1.hs -hidir ${CMAKE_CURRENT_BINARY_DIR} -odir ${CMAKE_CURRENT_BINARY_DIR} -i${CMAKE_CURRENT_SOURCE_DIR} -o ${CMAKE_CURRENT_BINARY_DIR}/BNFParser
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/stage1.hs ${CMAKE_CURRENT_SOURCE_DIR}/BNFLower.hs ${CMAKE_CURRENT_SOURCE_DIR}/BNFOptimize.hs ${CMAKE_CURRENT_SOURCE_DIR}/BNFParser.hs ${CMAKE_CURRENT_SOURCE_DIR}/BNFPrefix.hs ${CMAKE_CURRENT_SOURCE_DIR}/GraphCalculations.hs
                   COMMENT "Generate IDL grammar parser")

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/GeneratedParser.hs
                   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/BNFParser < ${CMAKE_CURRENT_SOURCE_DIR}/GrammarSpecification.txt > ${CMAKE_CURRENT_BINARY_DIR}/GeneratedParser.hs
                   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/BNFParser ${CMAKE_CURRENT_SOURCE_DIR}/GrammarSpecification.txt
                   COMMENT "Parse IDL grammar")

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/IDLParser
                   COMMAND ghc ${CMAKE_CURRENT_SOURCE_DIR}/stage2.hs -hidir ${CMAKE_CURRENT_BINARY_DIR} -odir ${CMAKE_CURRENT_BINARY_DIR} -i${CMAKE_CURRENT_BINARY_DIR} -o ${CMAKE_CURRENT_BINARY_DIR}/IDLParser
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/stage2.hs ${CMAKE_CURRENT_BINARY_DIR}/GeneratedParser.hs
                   COMMENT "Generate IDL program parser")

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.cpp
                   COMMAND ${CMAKE_CURRENT_BINARY_DIR}/IDLParser < ${CMAKE_CURRENT_SOURCE_DIR}/IdiomSpecification.txt | pypy ${CMAKE_CURRENT_SOURCE_DIR}/stage3.py > ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.cpp
                   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/IDLParser ${CMAKE_CURRENT_SOURCE_DIR}/IdiomSpecification.txt ${CMAKE_CURRENT_SOURCE_DIR}/stage3.py
                   COMMENT "Generate idiom specifications C++ file")

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.s
                   COMMAND ${CMAKE_CXX_COMPILER} -std=c++11 -fPIC -fno-rtti -fno-exceptions -S -O0 ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.cpp -o ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.s -I ${CMAKE_CURRENT_SOURCE_DIR}/../../include/ -I ${CMAKE_CURRENT_BINARY_DIR}/../../include/
                   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.cpp
                   COMMENT "Generate idiom specifications assembly file")
