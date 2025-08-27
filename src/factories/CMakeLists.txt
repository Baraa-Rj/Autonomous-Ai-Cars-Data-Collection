# Factories Components Library
# Handles factory pattern implementations (SOLID-compliant)

# Factories component sources
set(FACTORIES_SOURCES
    ReaderFactory.cpp
)

# Factories component headers
set(FACTORIES_HEADERS
    ${CMAKE_SOURCE_DIR}/headers/factories/ReaderFactory.h
)

# Create factories components library
add_library(FactoriesComponents STATIC
    ${FACTORIES_SOURCES}
    ${FACTORIES_HEADERS}
)

# Link required libraries
target_link_libraries(FactoriesComponents 
    PUBLIC
    DataComponents
    Qt6::Core
)

# Include directories
target_include_directories(FactoriesComponents 
    PUBLIC
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/headers/factories
    ${CMAKE_SOURCE_DIR}/headers/data
    ${CMAKE_SOURCE_DIR}/headers/readers
)