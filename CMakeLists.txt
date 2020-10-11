cmake_minimum_required(VERSION 3.1.0)
project(logger)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

add_library(logger SHARED
        Logger_global.h
        Logger.cpp
        Logger.h
        Logger.ui)

target_link_libraries(logger PUBLIC Qt5::Core Qt5::Widgets)
target_compile_definitions(logger PUBLIC LOGGER_LIBRARY)

install(TARGETS logger
        LIBRARY DESTINATION lib/carpi)
