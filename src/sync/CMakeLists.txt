# Sync Components Library  
# Handles timing and synchronization

# Sync component sources
set(SYNC_SOURCES
    ClockManager.cpp
    TimelineManager.cpp
)

# Sync component headers
set(SYNC_HEADERS
    ${CMAKE_SOURCE_DIR}/headers/sync/ClockManager.h
    ${CMAKE_SOURCE_DIR}/headers/sync/TimelineManager.h
)

# Create sync components library
add_library(SyncComponents STATIC
    ${SYNC_SOURCES}
    ${SYNC_HEADERS}
)

# Qt MOC for sync components (needed for ClockManager Q_OBJECT)
set_target_properties(SyncComponents PROPERTIES
    AUTOMOC ON
)

# Link required libraries
target_link_libraries(SyncComponents 
    PUBLIC
    DataComponents
    Qt6::Core
)

# Include directories
target_include_directories(SyncComponents 
    PUBLIC
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/headers/sync
    ${CMAKE_SOURCE_DIR}/headers/data
)