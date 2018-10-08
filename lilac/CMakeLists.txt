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
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/stage0.py ${CMAKE_CURRENT_SOURCE_DIR}/spmv_csr.lilac
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/stage0.py ${CMAKE_CURRENT_SOURCE_DIR}/spmv_csr.lilac
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "Generate spmv harnesses")

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mm_naive.cc ${CMAKE_CURRENT_BINARY_DIR}/mm_openblas.cc ${CMAKE_CURRENT_BINARY_DIR}/mm_cuda.cc
                   COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/stage0.py ${CMAKE_CURRENT_SOURCE_DIR}/mm.lilac
                   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/stage0.py ${CMAKE_CURRENT_SOURCE_DIR}/mm.lilac
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "Generate mm harnesses")