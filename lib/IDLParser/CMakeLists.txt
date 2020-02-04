add_llvm_library(LLVMIDLParser
  ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.cpp
  DEPENDS intrinsics_gen
          idl_parser
)

FILE(GLOB IDL_SOURCES *.idl)

set(CMAKE_CXX_STANDARD 17)

add_executable(idl_parser
  Parser.cc
)

add_custom_command(
  OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/PreprocessedIDL.idl
  COMMAND pypy ${CMAKE_CURRENT_SOURCE_DIR}/preprocess.py
               Idioms.idl
            -d ${CMAKE_CURRENT_SOURCE_DIR}
             > ${CMAKE_CURRENT_BINARY_DIR}/PreprocessedIDL.idl
  DEPENDS  ${CMAKE_CURRENT_SOURCE_DIR}/preprocess.py
           "${IDL_SOURCES}"
  COMMENT "Preprocess the IDL file")

add_custom_command(
  OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.cpp
  COMMAND idl_parser ${CMAKE_CURRENT_SOURCE_DIR}/IDL.bnf
                     ${CMAKE_CURRENT_SOURCE_DIR}/PreprocessedIDL.idl
              | pypy ${CMAKE_CURRENT_SOURCE_DIR}/stage3.py
                   > ${CMAKE_CURRENT_BINARY_DIR}/IdiomSpecifications.cpp
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/IDL.bnf
          ${CMAKE_CURRENT_SOURCE_DIR}/PreprocessedIDL.idl
          ${CMAKE_CURRENT_SOURCE_DIR}/stage3.py
          idl_parser
  COMMENT "Generate idiom specifications C++ file"
)
