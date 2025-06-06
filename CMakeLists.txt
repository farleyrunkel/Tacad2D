﻿cmake_minimum_required(VERSION 3.15)

project(TaCAD VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools Test OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools Core5Compat Test OpenGLWidgets)

include(SetupOpenCASCADE)
include(SetupSARibbon)
include(SetupCxxopts)
include(SetupQtAD)
include(SetupBoost)
include(SetupOpenGL)
include(SetupRTTR)

add_subdirectory(ext)
add_subdirectory(src)
