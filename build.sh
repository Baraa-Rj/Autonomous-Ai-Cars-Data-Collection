#!/bin/bash

echo "Building Car Status Visualization..."

# Create build directory
mkdir -p build
cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake ..

if [ $? -eq 0 ]; then
    echo "Building project..."
    make -j$(nproc)
    
    if [ $? -eq 0 ]; then
        echo "Build successful!"
        echo "Run './build/CarStatusVisualization' to start the application"
    else
        echo "Build failed!"
        exit 1
    fi
else
    echo "CMake configuration failed!"
    exit 1
fi