cmake_minimum_required(VERSION 3.5)

project(rafiki VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Network LinguistTools)

set(RES_FILES
    data/resources.qrc
    images/resources.qrc
)
set(TS_FILES translations/${CMAKE_PROJECT_NAME}_en.ts)

set(PROJECT_SOURCES
        src/core/autocompleter.cpp
        src/core/autocompleter.h
        src/core/client.cpp
        src/core/client.h
        src/core/config.cpp
        src/core/config.h
        src/core/crypto.cpp
        src/core/crypto.h
        src/core/finder.cpp
        src/core/finder.h
        src/core/id.cpp
        src/core/id.h
        src/core/inifile.cpp
        src/core/inifile.h
        src/core/input.cpp
        src/core/input.h
        src/core/io.cpp
        src/core/io.h
        src/core/server.cpp
        src/core/server.h
        src/core/shuntingyardalgorithm.cpp
        src/core/shuntingyardalgorithm.h
        src/core/urlparser.cpp
        src/core/urlparser.h
        src/core/utils.h
        src/main.cpp
        src/models/application.cpp
        src/models/application.h
        src/models/calculator.cpp
        src/models/calculator.h
        src/models/filesystementry.cpp
        src/models/filesystementry.h
        src/models/interactable.cpp
        src/models/interactable.h
        src/models/processedresult.cpp
        src/models/processedresult.h
        src/models/processedresultbuilder.cpp
        src/models/processedresultbuilder.h
        src/models/result.cpp
        src/models/result.h
        src/models/trash.cpp
        src/models/trash.h
        src/models/url.cpp
        src/models/url.h
        src/models/websearch.cpp
        src/models/websearch.h
        src/ui/interactivelabel.cpp
        src/ui/interactivelabel.h
        src/ui/mainwindow.cpp
        src/ui/mainwindow.h
        src/ui/mainwindow.ui
        src/ui/searchbox.cpp
        src/ui/searchbox.h
        src/ui/searchbox.ui
        src/ui/searchresult.cpp
        src/ui/searchresult.h
        src/ui/searchresultitem.cpp
        src/ui/searchresultitem.h
        src/ui/searchresult.ui
        src/ui/searchresultlist.cpp
        src/ui/searchresultlist.h
        src/ui/settingswindow.cpp
        src/ui/settingswindow.h
        src/ui/settingswindow.ui
        src/ui/websearchdialog.cpp
        src/ui/websearchdialog.h
        src/ui/websearchdialog.ui
        ${RES_FILES}
        ${TS_FILES}
)

# https://cmake.org/cmake/help/latest/variable/CMAKE_SYSTEM_NAME.html
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(PROJECT_SOURCES
        ${PROJECT_SOURCES}
        src/gnulinux/desktopentry.cpp
        src/gnulinux/desktopentry.h
        src/gnulinux/io.cpp
        src/gnulinux/io.h
    )
endif()

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${CMAKE_PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET rafiki APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_add_translations(${CMAKE_PROJECT_NAME} TS_FILES ${TS_FILES})
else()
    if(ANDROID)
        add_library(${CMAKE_PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${CMAKE_PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Network)

# Capitalizes the project name amd stores it in a variable.
string(LENGTH ${CMAKE_PROJECT_NAME} cmake_project_name_length)
string(SUBSTRING ${CMAKE_PROJECT_NAME} 0 1 cmake_project_name_head)
string(TOUPPER ${cmake_project_name_head} cmake_project_name_head_upper)
math(EXPR cmake_project_name_difference "${_cmake_project_name_length} - 1")
string(SUBSTRING ${CMAKE_PROJECT_NAME} 1 ${cmake_project_name_difference} cmake_project_name_tail)
string(CONCAT cmake_project_name_proper ${cmake_project_name_head_upper} ${cmake_project_name_tail})

target_compile_definitions(${CMAKE_PROJECT_NAME}
    PUBLIC
        PROJECT_NAME="${cmake_project_name_proper}"
        PROJECT_VERSION="${PROJECT_VERSION}"
    PRIVATE
        CMAKE_EXPORT_COMPILE_COMMANDS=1
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.giosali.${CMAKE_PROJECT_NAME})
endif()
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${CMAKE_PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${CMAKE_PROJECT_NAME})
endif()
