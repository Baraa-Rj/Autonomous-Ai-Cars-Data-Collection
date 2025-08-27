# Services Components Library
# Handles service layer functionality (SOLID-compliant)

# Services component sources
set(SERVICES_SOURCES
    DataLoaderService.cpp
)

# Services component headers
set(SERVICES_HEADERS
    ${CMAKE_SOURCE_DIR}/headers/services/DataLoaderService.h
)

# Create services components library
add_library(ServicesComponents STATIC
    ${SERVICES_SOURCES}
    ${SERVICES_HEADERS}
)

# Link required libraries
target_link_libraries(ServicesComponents 
    PUBLIC
    DataComponents
    ReadersComponents
    Qt6::Core
    ${OpenCV_LIBS}
)

# Include directories
target_include_directories(ServicesComponents 
    PUBLIC
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/headers/services
    ${CMAKE_SOURCE_DIR}/headers/data
    ${CMAKE_SOURCE_DIR}/headers/readers
)