# Data Components Library
# Handles all data types and storage

# Data component sources
set(DATA_SOURCES
    Data.cpp
    DataStore.cpp
    BrakeData.cpp
    SpeedData.cpp
    GpsData.cpp
    ImageData.cpp
    ImageHandler.cpp
    IMUData.cpp
    SteeringData.cpp
    ThrottleData.cpp
)

# Data component headers
set(DATA_HEADERS
    ${CMAKE_SOURCE_DIR}/headers/data/Data.h
    ${CMAKE_SOURCE_DIR}/headers/data/DataStore.h
    ${CMAKE_SOURCE_DIR}/headers/data/BrakeData.h
    ${CMAKE_SOURCE_DIR}/headers/data/SpeedData.h
    ${CMAKE_SOURCE_DIR}/headers/data/GpsData.h
    ${CMAKE_SOURCE_DIR}/headers/data/ImageData.h
    ${CMAKE_SOURCE_DIR}/headers/data/ImageHandler.h
    ${CMAKE_SOURCE_DIR}/headers/data/IMUData.h
    ${CMAKE_SOURCE_DIR}/headers/data/SteeringData.h
    ${CMAKE_SOURCE_DIR}/headers/data/ThrottleData.h
)

# Create data components library
add_library(DataComponents STATIC
    ${DATA_SOURCES}
    ${DATA_HEADERS}
)

# Link required libraries
target_link_libraries(DataComponents 
    PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    ${OpenCV_LIBS}
)

# Include directories
target_include_directories(DataComponents 
    PUBLIC
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/headers/data
)