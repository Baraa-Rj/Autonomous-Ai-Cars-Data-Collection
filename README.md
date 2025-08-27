# Data Collection Phase - Vehicle Sensor Visualization

A Qt/C++ application for real-time vehicle sensor data collection and visualization. This project provides a comprehensive system for loading, processing, and displaying various vehicle sensor data including GPS, IMU, speed, steering, brake, throttle, and multi-camera feeds.

## Features

- **Multi-Sensor Data Loading**: Reads CSV files for GPS, IMU, speed, brake, steering, and throttle data
- **Image Processing**: Loads timestamped camera images from multiple camera positions
- **Real-time Visualization**: Qt-based GUI for displaying all sensor data in real-time
- **Synchronized Playback**: Thread-based clock system for precise timing control
- **Modular Architecture**: Clean separation between data loading, processing, and display components

## Architecture

### Core Components
- **Simulation**: Main orchestrator coordinating all system components
- **ClockManager**: Thread-based timing system with configurable FPS
- **DisplayManager**: Qt GUI for real-time sensor data visualization
- **ReadersManager**: Manages loading data from various sensor sources

### Data System
- **Data**: Abstract base class with timestamped sensor readings
- **DataStore**: Central storage for current sensor readings
- **Specialized Data Classes**: BrakeData, GpsData, IMUData, ImageData, SpeedData, SteeringData, ThrottleData

### Reader System
- **AbstractDataReader**: Base class for data loading with smart pointer management
- **CSV Readers**: Specialized readers for each sensor type
- **ImageReader**: Handles directory-based image sequence loading

## Sample Data Structure

```
sample_data/
├── 3d_images/
│   ├── back/     # Timestamped rear camera JPEGs
│   ├── front/    # Timestamped front camera JPEGs
│   ├── left/     # Timestamped left camera JPEGs
│   └── right/    # Timestamped right camera JPEGs
├── brake.csv     # Brake pressure data
├── gps.csv       # GPS coordinates and altitude
├── imu.csv       # Accelerometer and gyroscope data
├── speed.csv     # Vehicle speed data
├── steering.csv  # Steering angle data
└── throttle.csv  # Throttle position data
```

## Building and Running

### Prerequisites
- Qt6 (Core and Widgets modules)
- OpenCV
- CMake 3.16+
- C++17 compatible compiler

### Build Instructions

1. Clone the repository:
```bash
git clone https://github.com/Baraa-Rj/data-collection-phase.git
cd data-collection-phase
```

2. Create build directory and compile:
```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

3. Run the application:
```bash
./QtOpenCVApp
```

### GUI Interface

The application displays:
- **Left Panel**: Real-time sensor readings (GPS coordinates, speed, brake, throttle, steering, IMU data)
- **Right Panel**: Multi-camera view with synchronized image feeds
- **Time Display**: Current playback timestamp

## Recent Improvements

This project has been fully refactored to resolve all compilation issues:

- **Memory Management**: Standardized on `shared_ptr` for consistent ownership
- **Architecture**: Fixed abstract class instantiation issues
- **Qt Integration**: Resolved all Qt-specific compilation problems  
- **Reader System**: Updated all sensor readers with consistent signatures
- **Build System**: Optimized CMake configuration for reliable builds

## Development

The codebase follows modern C++ practices with:
- Smart pointer memory management
- RAII resource handling
- Modular component design
- Qt's signal-slot system for inter-component communication

## License

This project is part of a vehicle data collection and analysis system.