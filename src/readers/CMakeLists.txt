# Readers Components Library
# Handles data reading from files and streams

# Readers component sources
set(READERS_SOURCES
    AbstractDataReaders.cpp
    BrakeReader.cpp
    GpsReader.cpp
    ImageReader.cpp
    IMUReader.cpp
    ReadersManager.cpp
    SpeedReader.cpp
    SteeringReader.cpp
    ThrottleReader.cpp
)

# Readers component headers
set(READERS_HEADERS
    ${CMAKE_SOURCE_DIR}/headers/readers/AbstractDataReader.h
    ${CMAKE_SOURCE_DIR}/headers/readers/BrakeReader.h
    ${CMAKE_SOURCE_DIR}/headers/readers/GpsReader.h
    ${CMAKE_SOURCE_DIR}/headers/readers/ImageReader.h
    ${CMAKE_SOURCE_DIR}/headers/readers/IMUReader.h
    ${CMAKE_SOURCE_DIR}/headers/readers/ReadersManager.h
    ${CMAKE_SOURCE_DIR}/headers/readers/SpeedReader.h
    ${CMAKE_SOURCE_DIR}/headers/readers/SteeringReader.h
    ${CMAKE_SOURCE_DIR}/headers/readers/ThrottleReader.h
)

# Create readers components library
add_library(ReadersComponents STATIC
    ${READERS_SOURCES}
    ${READERS_HEADERS}
)

# Link required libraries
target_link_libraries(ReadersComponents 
    PUBLIC
    DataComponents
    SyncComponents
    Qt6::Core
    ${OpenCV_LIBS}
)

# Include directories
target_include_directories(ReadersComponents 
    PUBLIC
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/headers/readers
    ${CMAKE_SOURCE_DIR}/headers/data
    ${CMAKE_SOURCE_DIR}/headers/sync
    ${CMAKE_SOURCE_DIR}/headers/services
)