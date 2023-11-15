cmake_minimum_required(VERSION 3.16)

set(CMAKE_OSX_ARCHITECTURES "x86_64")

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_NAME "ImageViewerX")
PROJECT(${PROJECT_NAME} LANGUAGES CXX)

set(qt_bundles Quick Widgets)
find_package(Qt6 REQUIRED COMPONENTS ${qt_bundles})
message("Qt Version: ${Qt6_VERSION}")

set(OpenCV_DIR /Users/Hilton/Desktop/setup/build_opencv)
find_package(OpenCV REQUIRED)
MESSAGE("OpenCV version : ${OpenCV_VERSION}")
# Add the paths to the include directories/to the header files
include_directories( ${OpenCV_INCLUDE_DIRS} )
link_directories(${OpenCV_LIB_DIR})

qt_standard_project_setup()
add_subdirectory(BasicSetup)
