cmake_minimum_required(VERSION 3.16)
project(data-collection-phase VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)
find_package(OpenCV REQUIRED)

# Enable Qt MOC (Meta Object Compiler)
set(CMAKE_AUTOMOC ON)

# Set global include directories
include_directories(${CMAKE_SOURCE_DIR}/headers)

# Add subdirectories (each with their own CMakeLists.txt)
add_subdirectory(src/data)
add_subdirectory(src/gui)  
add_subdirectory(src/readers)
add_subdirectory(src/sync)
add_subdirectory(src/services)
add_subdirectory(src/factories)

# Main executable
add_executable(QtOpenCVApp
    Simulation.cpp
    headers/Simulation.h
    src/Simulation.cpp
)

# Link all component libraries to main executable
target_link_libraries(QtOpenCVApp 
    PRIVATE
    DataComponents
    GuiComponents
    ReadersComponents
    SyncComponents
    ServicesComponents
    FactoriesComponents
    Qt6::Core 
    Qt6::Gui
    Qt6::Widgets
    ${OpenCV_LIBS}
)

# Include directories for main executable
target_include_directories(QtOpenCVApp PRIVATE
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/headers/gui
    ${CMAKE_SOURCE_DIR}/headers/data
    ${CMAKE_SOURCE_DIR}/headers/readers
    ${CMAKE_SOURCE_DIR}/headers/sync
    ${CMAKE_SOURCE_DIR}/headers/services
    ${CMAKE_SOURCE_DIR}/headers/factories
)