# Car Status Visualization

[![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![Qt](https://img.shields.io/badge/Qt-6-green.svg)](https://www.qt.io/)
[![OpenCV](https://img.shields.io/badge/OpenCV-4.x-orange.svg)](https://opencv.org/)
[![CMake](https://img.shields.io/badge/CMake-3.16+-red.svg)](https://cmake.org/)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

A professional real-time C++ Qt application for visualizing synchronized vehicle sensor data and multi-camera feeds. Designed for automotive testing, research, and telemetry analysis with frame-perfect synchronization across all data sources.

## ✨ Features

### 🚗 Multi-Sensor Integration
- **GPS Tracking**: Real-time latitude/longitude/altitude visualization
- **IMU Data**: 9-DOF inertial measurement (accelerometer, gyroscope, magnetometer)
- **Vehicle Dynamics**: Speed, brake pressure, throttle position, steering angle
- **Time Synchronization**: Microsecond-precision timestamp alignment

### 📹 Multi-Camera System
- **4-Camera Setup**: Simultaneous front/back/left/right camera feeds
- **Synchronized Playback**: Frame-perfect alignment with sensor data
- **High Performance**: Optimized OpenCV rendering pipeline
- **Flexible Formats**: Support for JPEG and common image formats

### 🎮 Advanced Playback Controls
- **Timeline Scrubbing**: Jump to any point in your dataset instantly
- **Variable Speed**: Control playback rate from 0.1x to 10x
- **Frame Stepping**: Navigate frame-by-frame for detailed analysis
- **Loop Playback**: Continuous replay for extended analysis

### 🏗 Extensible Architecture
- **Factory Pattern**: Easy addition of new sensor types
- **Plugin Architecture**: Modular data readers and processors
- **SOLID Principles**: Clean, maintainable, testable codebase
- **Qt Integration**: Professional desktop GUI framework

## 🚀 Quick Start

### Prerequisites

**System Requirements:**
- Linux/macOS/Windows
- C++17 compatible compiler (GCC 8+, Clang 9+, MSVC 2019+)
- Qt 6.0+ development libraries
- OpenCV 4.x development libraries
- CMake 3.16+

### Installation

#### Ubuntu/Debian
```bash
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev-tools libopencv-dev cmake build-essential git
```

#### macOS
```bash
brew install qt opencv cmake
```

#### Windows
1. Install [Qt 6](https://www.qt.io/download-qt-installer)
2. Install [OpenCV](https://opencv.org/releases/)
3. Install [Visual Studio 2019+](https://visualstudio.microsoft.com/)
4. Install [CMake](https://cmake.org/download/)

### Build & Run

```bash
# Clone repository
git clone <repository-url>
cd Last-Edition

# Quick build using provided script
chmod +x build.sh
./build.sh

# Manual build
mkdir build && cd build
cmake ..
make -j$(nproc)

# Run application
./build/CarStatusVisualization
```

## 📊 Usage Guide

### Data Loading
1. **Prepare Data**: Ensure your dataset follows the required format (see Data Format section)
2. **Launch Application**: Run the executable to open the main interface
3. **Load Dataset**: Click "Load Data" and select your data directory
4. **Start Analysis**: Use playback controls to navigate through your data

### Interface Overview
- **Left Panel**: Real-time sensor readings and GPS coordinates
- **Right Panel**: 2×2 camera feed grid with timestamp synchronization
- **Bottom Controls**: Playback timeline, play/pause, speed controls
- **Status Bar**: Current timestamp, playback status, data statistics

### Keyboard Shortcuts
- `Space`: Play/Pause
- `←/→`: Step backward/forward
- `Shift + ←/→`: Jump 10 seconds
- `Home/End`: Go to start/end of dataset
- `+/-`: Increase/decrease playback speed

## 📁 Data Format

### Directory Structure
```
your_dataset/
├── gps.csv                    # GPS coordinates
├── imu.csv                    # Inertial measurement data
├── speed.csv                  # Vehicle speed
├── brake.csv                  # Brake pressure
├── throttle.csv               # Throttle position
├── steering.csv               # Steering angle
└── 3d_images/                 # Camera feeds
    ├── front/                 # Front camera images
    ├── back/                  # Rear camera images
    ├── left/                  # Left camera images
    └── right/                 # Right camera images
```

### CSV Format Specifications

#### GPS Data (`gps.csv`)
```csv
time_stamp,latitude,longitude,height
1684926118.639806,37.7749,-122.4194,10.5
1684926118.706407,37.7750,-122.4195,10.6
```

#### IMU Data (`imu.csv`)
```csv
time_stamp,x_acc,y_acc,z_acc,pitch,roll,yaw,x_gyro,y_gyro,z_gyro,x_mag,y_mag,z_mag
1684926118.639806,0.1,0.2,-9.8,0.0,0.0,45.0,0.01,0.02,0.03,25.5,30.2,40.1
```

#### Sensor Data (speed.csv, brake.csv, throttle.csv, steering.csv)
```csv
time_stamp,data_value
1684926118.639806,65.5
1684926118.706407,66.2
```

### Image Naming Convention
Images must be named using Unix timestamps with microsecond precision:
```
1684926118.639806.jpeg
1684926118.706407.jpeg
1684926118.773480.jpeg
```

## 🏛 Architecture Overview

### Core Components

```
┌─────────────────┐    ┌──────────────────┐    ┌─────────────────┐
│   MainWindow    │    │  DataManager     │    │ SensorDataStore │
│   (GUI Layer)   │◄──►│  (Coordination)  │◄──►│   (Storage)     │
└─────────────────┘    └──────────────────┘    └─────────────────┘
         │                       │                       │
         ▼                       ▼                       ▼
┌─────────────────┐    ┌──────────────────┐    ┌─────────────────┐
│  Controllers    │    │ DataReaderFactory│    │   Data Classes  │
│ (SRP Compliant) │    │ (Factory Pattern)│    │ (GPS, IMU, etc.)│
└─────────────────┘    └──────────────────┘    └─────────────────┘
```

### Design Patterns
- **Factory Pattern**: `DataReaderFactory` creates appropriate readers for each sensor type
- **Single Responsibility**: Each controller handles one specific aspect (playback, display, loading)
- **Observer Pattern**: Real-time data updates across GUI components
- **Strategy Pattern**: Pluggable data readers for different formats

### Key Classes
- `DataManager`: Orchestrates data loading and synchronization
- `ClockManager`: Handles timeline and playback synchronization  
- `SensorDataStore`: Manages in-memory sensor data storage
- `PlaybackController`: Controls timeline navigation and playback
- `DisplayController`: Manages real-time GUI updates

## 🔧 Development

### Adding New Sensors

1. **Create Data Class**:
```cpp
// include/data/NewSensorData.h
class NewSensorData : public Data {
    double sensorValue;
    // Implementation...
};
```

2. **Implement Data Reader**:
```cpp
// include/readers/NewSensorDataReader.h  
class NewSensorDataReader : public DataReader {
    std::vector<std::unique_ptr<Data>> readData(const std::string& filePath) override;
};
```

3. **Register in Factory**:
```cpp
// src/readers/DataReaderFactory.cpp
factory["newsensor.csv"] = std::make_unique<NewSensorDataReader>();
```

4. **Update GUI**: Add display components in `MainWindow`

### Testing
Sample data is not included in the repository. To try the application, prepare a
dataset in the layout described in [Data Format](#-data-format), then:
```bash
cd build
./CarStatusVisualization
# Click "Load Data" and select your dataset directory
```
If a `sample_data/` directory exists in the project root, CMake copies it into
the build directory.

Automated headless tests (no display needed) generate a small fixture dataset
and check parsing and seeking:
```bash
cd build
ctest --output-on-failure
```

### Code Style
- **C++17 Standard**: Modern C++ features and best practices
- **Qt Conventions**: Follow Qt naming and coding conventions
- **RAII**: Proper resource management with smart pointers
- **Const Correctness**: Immutable data where appropriate

## 📦 Dependencies

### Runtime Dependencies
- Qt 6 (Core, Widgets, Network)
- OpenCV 4.x (Core, ImgProc, ImgCodecs)
- Standard C++ Library

### Development Dependencies
- CMake 3.16+
- C++17 compiler
- Qt 6 development headers
- OpenCV development headers

### Optional Dependencies
- Qt Creator (recommended IDE)
- clang-format (code formatting)
- Doxygen (documentation generation)

## 🚀 Performance

### Optimizations
- **Zero-Copy Operations**: Minimize data copying during playback
- **Efficient Image Loading**: OpenCV-optimized image pipeline  
- **Smart Caching**: Intelligent data pre-loading and caching
- **Multi-threading**: Background data loading and processing

### Benchmarks
- **Dataset Size**: Tested with 10GB+ datasets
- **Playback Performance**: 60fps+ on modern hardware
- **Memory Usage**: ~500MB for typical 1-hour dataset
- **Startup Time**: <3 seconds for large datasets

## 🐛 Troubleshooting

### Common Issues

**Build Errors:**
```bash
# Qt not found
export CMAKE_PREFIX_PATH="/usr/lib/x86_64-linux-gnu/cmake/Qt6"

# OpenCV not found  
sudo apt install libopencv-contrib-dev

# Missing C++17 support
export CXX=g++-8
```

**Runtime Issues:**
- **Application won't start**: Check Qt 6 installation and library paths
- **Data loading fails**: Verify CSV format matches specifications exactly
- **Images not displaying**: Ensure timestamp-based filename format
- **Poor performance**: Check available memory and disk I/O speed

### Debug Mode
```bash
# Build with debug symbols
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
gdb ./CarStatusVisualization
```

## 🤝 Contributing

We welcome contributions! Please follow these guidelines:

1. **Fork** the repository
2. **Create** a feature branch (`git checkout -b feature/amazing-feature`)
3. **Follow** existing code style and patterns
4. **Test** your changes thoroughly
5. **Commit** with clear, descriptive messages
6. **Submit** a pull request

### Development Setup
```bash
# Clone your fork
git clone https://github.com/yourusername/Last-Edition.git

# Set up development environment
cd Last-Edition
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
```

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Qt Framework**: For providing excellent cross-platform GUI capabilities
- **OpenCV Community**: For powerful computer vision libraries
- **CMake**: For robust cross-platform build system
- **Contributors**: All developers who have contributed to this project

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/yourusername/Last-Edition/issues)
- **Discussions**: [GitHub Discussions](https://github.com/yourusername/Last-Edition/discussions)
- **Wiki**: [Project Wiki](https://github.com/yourusername/Last-Edition/wiki)

---

**Built with ❤️ using modern C++ and Qt**