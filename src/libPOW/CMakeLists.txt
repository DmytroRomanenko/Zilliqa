add_library (POW pow.cpp)
include_directories(${CMAKE_SOURCE_DIR}/src/depends/)

target_include_directories (POW PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries (POW PRIVATE ethash Constants Common Server ${JSONCPP_LINK_TARGETS} ${JSONRPCCPP_CLIENT_LINK_TARGETS})

if(OPENCL_MINE)
    find_library(OPENCL_LIBRARIES OpenCL ENV LD_LIBRARY_PATH)
    target_link_libraries(POW PRIVATE ${OPENCL_LIBRARIES} ethash-cl)
endif()

if(CUDA_MINE)
    target_link_libraries(POW PRIVATE ethash-cuda)
endif()
