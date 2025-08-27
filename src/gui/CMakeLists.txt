# GUI Components Library
# Handles UI construction and data presentation (SOLID-compliant)

# GUI component sources
set(GUI_SOURCES
    DisplayManager.cpp
    UIBuilder.cpp
    DataPresenter.cpp
)

# GUI component headers
set(GUI_HEADERS
    ${CMAKE_SOURCE_DIR}/headers/gui/DisplayManager.h
    ${CMAKE_SOURCE_DIR}/headers/gui/UIBuilder.h
    ${CMAKE_SOURCE_DIR}/headers/gui/DataPresenter.h
)

# Create GUI components library
add_library(GuiComponents STATIC
    ${GUI_SOURCES}
    ${GUI_HEADERS}
)

# Qt MOC for GUI components (needed for DisplayManager Q_OBJECT)
set_target_properties(GuiComponents PROPERTIES
    AUTOMOC ON
)

# Link required libraries
target_link_libraries(GuiComponents 
    PUBLIC
    DataComponents
    Qt6::Core
    Qt6::Widgets
    ${OpenCV_LIBS}
)

# Include directories
target_include_directories(GuiComponents 
    PUBLIC
    ${CMAKE_SOURCE_DIR}/headers
    ${CMAKE_SOURCE_DIR}/headers/gui
    ${CMAKE_SOURCE_DIR}/headers/data
    ${CMAKE_SOURCE_DIR}/headers/readers
    ${CMAKE_SOURCE_DIR}/headers/sync
)