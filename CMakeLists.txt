cmake_minimum_required(VERSION 3.16)
project(data-collection-phase)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

find_package(OpenCV REQUIRED)

set(CMAKE_AUTOMOC ON)

add_executable(QtOpenCVApp
    Simulation.cpp
    # GUI
    headers/gui/DisplayManager.h
    src/gui/DisplayManager.cpp
    # Sync
    headers/sync/ClockManager.h
    src/sync/ClockManager.cpp
    # Data
    src/data/Data.cpp
    src/data/BrakeData.cpp
    src/data/SpeedData.cpp
    src/data/GpsData.cpp
    src/data/ImageData.cpp
    src/data/ImageHandler.cpp
    src/data/IMUData.cpp
    src/data/SteeringData.cpp
    src/data/ThrottleData.cpp
    # Readers
    src/readers/AbstractDataReaders.cpp
    src/readers/BrakeReader.cpp
    src/readers/GpsReader.cpp
    src/readers/ImageReader.cpp
    src/readers/IMUReader.cpp
    src/readers/ReadersManager.cpp
    src/readers/SpeedReader.cpp
    src/readers/SteeringReader.cpp
    src/readers/ThrottleReader.cpp
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
