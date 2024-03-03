# Useful info: https://doc.qt.io/qt-6/qttestlib-tutorial1-example.html

cmake_minimum_required(VERSION 3.5)

project(test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Test)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

set(PROJECT_SOURCES
        ../src/autocompletemap.cpp
        ../src/autocompletemap.h
        test_autocompletemap.cpp
        utest.h
)

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})

# `target_link_libraries` must appear after `add_executable`.
target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Test)

add_custom_target(run ALL
    COMMAND ./test
)