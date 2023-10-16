cmake_minimum_required(VERSION 3.16)

set(CMAKE_OSX_ARCHITECTURES "x86_64")

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_NAME "BasicSetup")
PROJECT(${PROJECT_NAME} LANGUAGES CXX)

set(qt_bundles Quick Widgets)
find_package(Qt6 REQUIRED COMPONENTS ${qt_bundles})

qt_standard_project_setup()
add_subdirectory(BasicSetup)
