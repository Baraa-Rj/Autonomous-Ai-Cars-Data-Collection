cmake_minimum_required(VERSION 3.16)
project(CarStatusVisualization VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(OpenCV REQUIRED)

# Enable Qt MOC (Meta Object Compiler) for Q_OBJECT classes
set(CMAKE_AUTOMOC ON)

set(SOURCES
    # Application
    src/app/main.cpp
    
    # Data Management
    src/management/DataManager.cpp
    src/management/ClockManager.cpp
    
    # GUI
    src/gui/MainWindow.cpp
    
    # Data Readers
    src/readers/DataReader.cpp
    src/readers/DataReaderFactory.cpp
    src/readers/GPSDataReader.cpp
    src/readers/IMUDataReader.cpp
    src/readers/SpeedDataReader.cpp
    src/readers/BrakeDataReader.cpp
    src/readers/ThrottleDataReader.cpp
    src/readers/SteeringDataReader.cpp
    src/readers/ImageDataReader.cpp
    
    # Data Types
    src/data/GPSData.cpp
    src/data/IMUData.cpp
    src/data/SpeedData.cpp
    src/data/BrakeData.cpp
    src/data/ThrottleData.cpp
    src/data/SteeringData.cpp
    src/data/ImageData.cpp
)

set(HEADERS
    # Core
    include/core/Data.h
    
    include/management/DataManager.h
    include/management/ClockManager.h
    include/management/SensorDataStore.h
    
    # GUI
    include/gui/MainWindow.h
    
    # Data Readers
    include/readers/DataReader.h
    include/readers/DataReaderFactory.h
    include/readers/GPSDataReader.h
    include/readers/IMUDataReader.h
    include/readers/SpeedDataReader.h
    include/readers/BrakeDataReader.h
    include/readers/ThrottleDataReader.h
    include/readers/SteeringDataReader.h
    include/readers/ImageDataReader.h
    
    # Data Types
    include/data/GPSData.h
    include/data/IMUData.h
    include/data/SpeedData.h
    include/data/BrakeData.h
    include/data/ThrottleData.h
    include/data/SteeringData.h
    include/data/ImageData.h
)

add_executable(CarStatusVisualization ${SOURCES} ${HEADERS})

target_include_directories(CarStatusVisualization PRIVATE include)
target_link_libraries(CarStatusVisualization Qt6::Core Qt6::Widgets ${OpenCV_LIBS})

# Copy sample data to build directory
file(COPY sample_data DESTINATION ${CMAKE_BINARY_DIR})