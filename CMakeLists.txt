#
#  Just having fun
#
#
cmake_minimum_required(VERSION 3.12)

set(TINY_ENGINE_VERSION "0.0.0")

project(tiny_engine VERSION ${TINY_ENGINE_VERSION} LANGUAGES CXX)

file(GLOB_RECURSE TE_CORE_SOURCE ${PROJECT_SOURCE_DIR}/src/core/*.cpp)
file(GLOB_RECURSE TE_CORE_HEADER ${PROJECT_SOURCE_DIR}/include/core/*.h ${PROJECT_SOURCE_DIR}/include/core/*.inl)
add_library(te_core ${TE_CORE_SOURCE} ${TE_CORE_HEADER})
target_include_directories(te_core
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


file(GLOB_RECURSE TE_OS_SOURCE ${PROJECT_SOURCE_DIR}/src/os/*.cpp)
file(GLOB_RECURSE TE_OS_HEADER ${PROJECT_SOURCE_DIR}/include/os/*.h ${PROJECT_SOURCE_DIR}/include/os/*.inl)
add_library(te_os ${TE_OS_SOURCE} ${TE_OS_HEADER})
target_include_directories(te_os
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(te_os PUBLIC OpenMP::OpenMP_CXX)
endif()

file(GLOB_RECURSE TE_TEST_SOURCE ${PROJECT_SOURCE_DIR}/src/test/*.cpp)
add_executable(te_test_win ${TE_TEST_SOURCE})
target_link_libraries(te_test_win PRIVATE te_core te_os)