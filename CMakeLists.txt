cmake_minimum_required(VERSION 3.16)
project(CarStatusVisualization VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)
find_package(OpenCV REQUIRED)

# Enable Qt MOC (Meta Object Compiler) for Q_OBJECT classes
set(CMAKE_AUTOMOC ON)

set(SOURCES
    # Application
    src/app/Simulation.cpp
    
    # Data Management
    src/management/DataManager.cpp
    src/management/ClockManager.cpp
    src/management/SensorDataStore.cpp
    
    # GUI
    src/gui/MainWindow.cpp
    src/gui/GPSMapWidget.cpp
    
    # Controllers (SRP-compliant)
    src/controllers/PlaybackController.cpp
    src/controllers/DisplayController.cpp
    src/controllers/DataLoadingController.cpp
    
    # Utils
    src/utils/ImageProcessor.cpp
    
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
    include/gui/GPSMapWidget.h
    
    # Controllers (SRP-compliant)
    include/controllers/PlaybackController.h
    include/controllers/DisplayController.h
    include/controllers/DataLoadingController.h
    
    # Utils
    include/utils/ImageProcessor.h
    
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
target_link_libraries(CarStatusVisualization Qt6::Core Qt6::Widgets Qt6::Network ${OpenCV_LIBS})

# Copy sample data (if present; it is not in the repository) and map HTML to build directory
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/sample_data)
    file(COPY sample_data DESTINATION ${CMAKE_BINARY_DIR})
endif()
file(COPY map.html DESTINATION ${CMAKE_BINARY_DIR})
# Headless tests: non-GUI sources only, no display needed
enable_testing()

add_executable(test_data_manager
    tests/test_data_manager.cpp
    src/management/DataManager.cpp
    src/management/ClockManager.cpp
    src/management/SensorDataStore.cpp
    src/readers/DataReader.cpp
    src/readers/DataReaderFactory.cpp
    src/readers/GPSDataReader.cpp
    src/readers/IMUDataReader.cpp
    src/readers/SpeedDataReader.cpp
    src/readers/BrakeDataReader.cpp
    src/readers/ThrottleDataReader.cpp
    src/readers/SteeringDataReader.cpp
    src/readers/ImageDataReader.cpp
    src/data/GPSData.cpp
    src/data/IMUData.cpp
    src/data/SpeedData.cpp
    src/data/BrakeData.cpp
    src/data/ThrottleData.cpp
    src/data/SteeringData.cpp
    src/data/ImageData.cpp
    include/management/DataManager.h
)
target_include_directories(test_data_manager PRIVATE include)
target_link_libraries(test_data_manager Qt6::Core ${OpenCV_LIBS})
add_test(NAME data_manager COMMAND test_data_manager)
