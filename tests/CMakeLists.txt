cmake_minimum_required(VERSION 3.30)

project(test LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS 
    Core
    Gui
    Test
    Widgets
)

include_directories(
    ../src
    ../include
)

set(PROJECT_SOURCES
    ../src/core/bridges/calculatorbridge.cpp
    ../src/core/bridges/featurebridge.cpp
    ../src/core/bridges/urlbridge.cpp
    ../src/core/bridges/websearchbridge.cpp
    ../src/core/models/calculatormodel.cpp
    ../src/core/models/featuremodel.cpp
    ../src/core/models/urlmodel.cpp
    ../src/core/models/websearchmodel.cpp
    ../src/core/objects/calculatorobject.cpp
    ../src/core/objects/featureobject.cpp
    ../src/core/objects/urlobject.cpp
    ../src/core/objects/websearchobject.cpp
    ../src/core/file.cpp
    ../src/core/paths.cpp
    ../src/core/settings.cpp
    ../src/core/utilities.cpp
    core/bridges/calculatorbridge_test.cpp
    core/bridges/urlbridge_test.cpp
    core/bridges/websearchbridge_test.cpp
    core/utilities_test.cpp
    include/tsl/htrie_map_test.cpp
    main.cpp
)

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})

# Link Qt libraries. This must appear after add_executable.
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Test
    Qt6::Widgets
)
