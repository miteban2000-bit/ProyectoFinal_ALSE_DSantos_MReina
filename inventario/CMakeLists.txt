cmake_minimum_required(VERSION 3.10)
project(InventoryManagerApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ✅ Activar AUTOMOC, AUTOUIC y AUTORCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_compile_options(-Wall -Wextra -Wpedantic)

find_package(Qt5 5.9 REQUIRED COMPONENTS Core Widgets Sql PrintSupport)

set(HEADERS
    include/component.h
    include/databasemanager.h
    include/inventorymanager.h
    include/reportgenerator.h
    include/mainwindow.h
)

set(SOURCES
    src/main.cpp
    src/component.cpp
    src/databasemanager.cpp
    src/inventorymanager.cpp
    src/reportgenerator.cpp
    src/mainwindow.cpp
)

add_executable(inventory ${SOURCES} ${HEADERS})

target_include_directories(inventory PRIVATE include)

target_link_libraries(inventory
    PRIVATE
        Qt5::Core
        Qt5::Widgets
        Qt5::Sql
        Qt5::PrintSupport
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_link_options(-s)
endif()
