cmake_minimum_required(VERSION 3.12)
project(tootbrush)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(
    Qt5 REQUIRED COMPONENTS
    Widgets
    Gui
    Core
    Network
)

set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/request.cpp
)

set(HEADERS
    include/mainwindow.h
    include/request.h
    include/mastodon.h
)

add_executable(
    tootbrush
    ${SOURCES}
    ${HEADERS}
)

target_compile_options(
    tootbrush PUBLIC
    -std=c++11
    -Wall
    -Wextra
    -pedantic-errors
)

target_link_libraries(
    tootbrush
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    Qt5::Network
)
