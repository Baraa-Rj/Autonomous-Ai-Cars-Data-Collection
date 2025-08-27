cmake_minimum_required(VERSION 3.16)
project(data-collection-phase)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6 (or Qt5 if you have that instead)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
# find_package(Qt5 REQUIRED COMPONENTS Core Widgets)  # Use this if you have Qt5

# Find OpenCV
find_package(OpenCV REQUIRED)

# Qt MOC processing
set(CMAKE_AUTOMOC ON)

# Create executable with a specific name
add_executable(QtOpenCVApp main.cpp)

# Link libraries
target_link_libraries(QtOpenCVApp 
    Qt6::Core 
    Qt6::Widgets
    ${OpenCV_LIBS}
)