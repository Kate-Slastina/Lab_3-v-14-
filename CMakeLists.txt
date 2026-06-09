cmake_minimum_required(VERSION 3.19)
project(Lab3_Combined LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

set(CORE_SRC
    src/core/DynamicArray.tpp
    src/core/ArraySequence.tpp
    src/core/LinkedList.tpp
    src/core/ListSequence.tpp
    src/generators/RecurrentSequences.cpp
    src/core/Complex.cpp
    src/core/Student.cpp
    src/core/Teacher.cpp
)

set(ELEMENTS_SRC
    src/elements/Number.cpp
    src/elements/StudentElement.cpp
    src/elements/TeacherElement.cpp
)

qt_add_executable(Lab3_GUI
    gui/main.cpp
    gui/mainwindow.cpp
    gui/mainwindow.h
    gui/mainwindow.ui
    ${CORE_SRC}
    ${ELEMENTS_SRC}
)

target_include_directories(Lab3_GUI PRIVATE
    src
    src/core
    src/core/Array
    src/core/List
    src/elements
    src/generators
    src/algorithms
    src/exceptions
)

target_link_libraries(Lab3_GUI PRIVATE Qt::Core Qt::Widgets)

enable_testing()
find_package(GTest REQUIRED)

add_executable(Lab3Tests
    tests/test_ArraySequence.cpp
    tests/test_LinkedList.cpp
    tests/test_ListSequence.cpp
    tests/test_DynamicArray.cpp
    tests/test_stream.cpp
    ${CORE_SRC}         
)

target_include_directories(Lab3Tests PRIVATE
    src
    src/core
    src/core/Array
    src/core/List
    src/exceptions
)

target_link_libraries(Lab3Tests PRIVATE GTest::gtest GTest::gtest_main)