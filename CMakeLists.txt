# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(Lab3_Stream LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
find_package(GTest REQUIRED)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CORE_DIR ${SRC_DIR}/core)

add_library(core_lib INTERFACE)
target_include_directories(core_lib INTERFACE
    ${SRC_DIR}
    ${CORE_DIR}
    ${CORE_DIR}/Array
    ${CORE_DIR}/List
)

add_executable(Lab3Tests
    tests/test_stream.cpp
)

target_link_libraries(Lab3Tests PRIVATE core_lib GTest::gtest GTest::gtest_main)

enable_testing()
add_test(NAME StreamTests COMMAND Lab3Tests)