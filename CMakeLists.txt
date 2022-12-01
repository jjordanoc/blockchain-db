cmake_minimum_required(VERSION 3.5)

project(proyecto-sha256-ui VERSION 0.1 LANGUAGES CXX)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17 /Zc:__cplusplus /permissive-")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Concurrent)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        blockwidget.h blockwidget.cpp
        blockidinputbox.h blockidinputbox.cpp
        blockchain.h node.h circular_list.h list.h entry.h crypto.h block.h studententry.h transactionentry.h crypto.cpp globals.h
        createblockform.h createblockform.cpp createblockform.ui
        mainwindow.ui blockwidget.ui
        entrywidget.h entrywidget.cpp entrywidget.ui
        createindexform.h createindexform.cpp createindexform.ui
        compacttrie.h
        queryform.h queryform.cpp queryform.ui
        querydisplayview.h querydisplayview.cpp querydisplayview.ui
        updateentryform.h updateentryform.cpp updateentryform.ui
        findentryform.h findentryform.cpp findentryform.ui
        heap.h
        index.h
        avl.h
        hash.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(proyecto-sha256-ui
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET proyecto-sha256-ui APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(proyecto-sha256-ui SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(proyecto-sha256-ui
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(proyecto-sha256-ui PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Concurrent)
target_link_libraries(proyecto-sha256-ui PRIVATE OpenSSL::SSL OpenSSL::Crypto)

set_target_properties(proyecto-sha256-ui PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(proyecto-sha256-ui)
endif()
