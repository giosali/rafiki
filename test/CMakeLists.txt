cmake_minimum_required(VERSION 3.5)

project(test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES
        ../src/autocompletemap.cpp
        ../src/autocompletemap.h
        test_autocompletemap.cpp
        utest.h
)

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})

add_custom_target(run ALL
    COMMAND ./test
)