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

add_executable(Lab3TestsArray
    tests/test_ArraySequence.cpp
    ${CORE_SRC}         
)
add_executable(Lab3TestsLinkedList
    tests/test_LinkedList.cpp
    ${CORE_SRC}         
)
add_executable(Lab3TestsListSeq
    tests/test_ListSequence.cpp
    ${CORE_SRC}         
)

add_executable(Lab3TestsDynArray
    tests/test_DynamicArray.cpp
    ${CORE_SRC}         
)

add_executable(Lab3TestsStream
    tests/test_stream.cpp
    ${CORE_SRC}         
)

target_include_directories(Lab3TestsArray PRIVATE
    src
    src/core
    src/core/Array
    src/core/List
    src/exceptions
)

target_include_directories(Lab3TestsLinkedList PRIVATE
    src
    src/core
    src/core/Array
    src/core/List
    src/exceptions
)

target_include_directories(Lab3TestsListSeq PRIVATE
    src
    src/core
    src/core/Array
    src/core/List
    src/exceptions
)

target_include_directories(Lab3TestsDynArray PRIVATE
    src
    src/core
    src/core/Array
    src/core/List
    src/exceptions
)

target_include_directories(Lab3TestsStream PRIVATE
    src
    src/core
    src/core/Array
    src/core/List
    src/exceptions
)

target_link_libraries(Lab3TestsArray PRIVATE GTest::gtest_main)
target_link_libraries(Lab3TestsLinkedList PRIVATE GTest::gtest_main)
target_link_libraries(Lab3TestsListSeq PRIVATE GTest::gtest_main)
target_link_libraries(Lab3TestsDynArray PRIVATE GTest::gtest_main)
target_link_libraries(Lab3TestsStream PRIVATE GTest::gtest_main)

add_test(NAME Lab3TestsArray COMMAND Lab3TestsArray)
add_test(NAME Lab3TestsLinkedList COMMAND Lab3TestsLinkedList)
add_test(NAME Lab3TestsListSeq COMMAND Lab3TestsListSeq)
add_test(NAME Lab3TestsDynArray COMMAND Lab3TestsDynArray)
add_test(NAME Lab3TestsStream COMMAND Lab3TestsStream)