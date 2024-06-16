# Useful info: https://doc.qt.io/qt-6/qttestlib-tutorial1-example.html

cmake_minimum_required(VERSION 3.5)

project(test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Test)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

set(PROJECT_SOURCES
        ../src/core/autocompleter.cpp
        ../src/core/autocompleter.h
        ../src/core/input.cpp
        ../src/core/input.h
        ../src/core/shuntingyardalgorithm.cpp
        ../src/core/shuntingyardalgorithm.h
        ../src/core/urlparser.cpp
        ../src/core/urlparser.h
        ../src/core/utils.h
        test_autocompleter.cpp
        test_shuntingyardalgorithm.cpp
        test_utils.cpp
        test_urlparser.cpp
        utest.h
)

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})

# `target_link_libraries` must appear after `add_executable`.
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Test)

add_custom_target(run ALL
    COMMAND ${PROJECT_NAME}
)