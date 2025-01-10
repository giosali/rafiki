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

set(RES_FILES
    ../data/resources.qrc
)

set(PROJECT_SOURCES
    ../src/core/bridges/applicationbridge.cpp
    ../src/core/bridges/calculatorbridge.cpp
    ../src/core/bridges/featurebridge.cpp
    ../src/core/bridges/filesystementrybridge.cpp
    ../src/core/bridges/refreshbridge.cpp
    ../src/core/bridges/settingsbridge.cpp
    ../src/core/bridges/trashbridge.cpp
    ../src/core/bridges/urlbridge.cpp
    ../src/core/bridges/websearchbridge.cpp
    ../src/core/models/applicationmodel.cpp
    ../src/core/models/calculatormodel.cpp
    ../src/core/models/featuremodel.cpp
    ../src/core/models/filesystementrymodel.cpp
    ../src/core/models/refreshmodel.cpp
    ../src/core/models/settingsmodel.cpp
    ../src/core/models/trashmodel.cpp
    ../src/core/models/urlmodel.cpp
    ../src/core/models/websearchmodel.cpp
    ../src/core/objects/applicationobject.cpp
    ../src/core/objects/calculatorobject.cpp
    ../src/core/objects/featureobject.cpp
    ../src/core/objects/filesystementryobject.cpp
    ../src/core/objects/refreshobject.cpp
    ../src/core/objects/settingsobject.cpp
    ../src/core/objects/trashobject.cpp
    ../src/core/objects/urlobject.cpp
    ../src/core/objects/websearchobject.cpp
    ../src/core/crypto.cpp
    ../src/core/fetcher.cpp
    ../src/core/file.cpp
    ../src/core/indexer.cpp
    ../src/core/paths.cpp
    ../src/core/settings.cpp
    ../src/core/theme.cpp
    ../src/core/utilities.cpp
    ../src/ui/calculatortab.cpp
    ../src/ui/generaltab.cpp
    ../src/ui/filesearchtab.cpp
    ../src/ui/interactivelabel.cpp
    ../src/ui/settingswindow.cpp
    ../src/ui/websearchdialog.cpp
    calculatorbridge_test.cpp
    htrie_map_test.cpp
    indexer_test.cpp
    urlbridge_test.cpp
    utilities_test.cpp
    websearchbridge_test.cpp
    main.cpp
    ${RES_FILES}
)

add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})

# Link Qt libraries. This must appear after add_executable.
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Test
    Qt6::Widgets
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/duckduckgo.json ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resources/firefox.desktop ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
