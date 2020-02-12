cmake_minimum_required (VERSION 2.8.12)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual -Wpointer-arith -Wwrite-strings -Werror=format -march=native " )
set(CMAKE_CXX_STANDARD 11)

project(grpc_async)

include("${PROJECT_SOURCE_DIR}/help.cmake")
define_colors()

# 编译结果中的库和可执行文件的路径
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(DEPENDENCY_PREFIX "../../opensource/")
set(INCLUDE_DIRS
        ${PROJECT_SOURCE_DIR}/
        ${PROJECT_SOURCE_DIR}/include
        ${DEPENDENCY_PREFIX}/xtra_rhel7.x/google_prefix/include
        )

include_directories( ${INCLUDE_DIRS} )

link_directories(
        ${PROJECT_SOURCE_DIR}/lib/
        ${DEPENDENCY_PREFIX}/xtra_rhel7.x/google_prefix/lib
)

aux_source_directory(. SOURCE)
add_executable(grpc_async ${SOURCE})

set (EXTRA_LIBS ${EXTRA_LIBS} pthread)
set (EXTRA_LIBS ${EXTRA_LIBS} protoc protobuf)
set (EXTRA_LIBS ${EXTRA_LIBS} grpc++ grpc gpr)

target_link_libraries(
    grpc_async ${EXTRA_LIBS}
)