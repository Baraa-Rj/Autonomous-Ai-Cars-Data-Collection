cmake_minimum_required(VERSION 3.16)
project(data-collection-phase)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

find_package(OpenCV REQUIRED)

set(CMAKE_AUTOMOC ON)

add_executable(QtOpenCVApp
    Simulation.cpp
    headers/gui/DisplayManager.h
    src/gui/DisplayManager.cpp
)

target_include_directories(QtOpenCVApp PRIVATE
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/headers/gui
)

target_link_libraries(QtOpenCVApp 
    Qt6::Core 
    Qt6::Widgets
    ${OpenCV_LIBS}
)
