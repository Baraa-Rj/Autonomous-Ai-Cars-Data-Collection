# Car Status Visualization

A real-time C++ Qt application for visualizing synchronized vehicle sensor data and camera feeds from multiple sources including GPS, IMU, speed, brake, throttle, steering, and 4-camera setup.

## Features

- **Real-time Sensor Display**: Shows GPS, IMU, speed, brake, throttle, and steering data
- **Multi-Camera Feeds**: Displays synchronized camera feeds from front, back, left, and right positions  
- **Time-synchronized Playback**: All sensor data and camera feeds are synchronized by timestamp
- **Interactive Controls**: Play/pause, timeline scrubbing, and progress tracking
- **Factory Pattern Data Loading**: Extensible CSV readers for different sensor types
- **OpenCV Integration**: Efficient image loading and display

## Architecture

The application follows a clean object-oriented design with:

- **Abstract Data Classes**: Base `Data` class with specialized sensors (GPSData, IMUData, etc.)
- **Factory Pattern**: DataReaderFactory creates appropriate CSV readers for each sensor type
- **Data Management**: DataManager handles loading and synchronization of all sensor streams
- **Qt GUI**: MainWindow provides real-time visualization with sensor panels and camera feeds

## Requirements

- **Qt 6** (Core, Widgets)
- **OpenCV 4.x**
- **C++17 compiler**
- **CMake 3.16+**

### Ubuntu/Debian Installation
```bash
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev-tools libopencv-dev cmake build-essential
```

### macOS Installation
```bash
brew install qt opencv cmake
```

## Building

1. **Clone and navigate to project**:
   ```bash
   git clone <repository-url>
   cd Last-Edition
   ```

2. **Create build directory**:
   ```bash
   mkdir build && cd build
   ```

3. **Configure with CMake**:
   ```bash
   cmake ..
   ```

4. **Build the project**:
   ```bash
   make -j$(nproc)
   ```

5. **Run the application**:
   ```bash
   ./CarStatusVisualization
   ```

## Usage

1. **Launch Application**: Run the executable to open the main window
2. **Load Data**: Click "Load Data" and select your data directory containing:
   - CSV files: `gps.csv`, `imu.csv`, `speed.csv`, `brake.csv`, `throttle.csv`, `steering.csv`
   - Image directories: `3d_images/front/`, `3d_images/back/`, `3d_images/left/`, `3d_images/right/`
3. **Control Playback**: Use play/pause button and timeline slider to navigate through data
4. **Monitor Sensors**: View real-time sensor readings in the left panel
5. **Watch Cameras**: Observe synchronized camera feeds in the 2x2 grid on the right

## Data Format

### CSV Files
Each sensor CSV file contains timestamp and sensor-specific data:

- **GPS**: `time_stamp,latitude,longitude,height`
- **IMU**: `time_stamp,x_acc,y_acc,z_acc,pitch,roll,yaw,x_gyro,y_gyro,z_gyro,x_mag,y_mag,z_mag`
- **Speed/Brake/Throttle/Steering**: `time_stamp,data_value`

### Images
Images are stored in directories with filenames as Unix timestamps:
```
3d_images/
├── front/1684926118.639806.jpeg
├── back/1684926118.706407.jpeg
├── left/1684926118.773480.jpeg
└── right/1684926118.841911.jpeg
```

## Project Structure

```
├── CMakeLists.txt
├── README.md
├── include/
│   ├── Data.h                    # Abstract base class
│   ├── *Data.h                   # Sensor data classes
│   ├── *DataReader.h             # CSV reader classes
│   ├── DataReaderFactory.h       # Factory pattern
│   ├── DataManager.h             # Data coordination
│   ├── SensorDataStore.h         # Data storage
│   └── MainWindow.h              # GUI interface
├── src/
│   ├── main.cpp
│   ├── *Data.cpp                 # Data class implementations  
│   ├── *DataReader.cpp           # Reader implementations
│   ├── DataManager.cpp           # Data management logic
│   └── MainWindow.cpp            # GUI implementation
└── sample_data/                  # Example data files
```

## Extending the Application

### Adding New Sensors
1. Create new data class inheriting from `Data`
2. Implement corresponding `DataReader` subclass  
3. Add to `DataReaderFactory` and `SensorDataStore`
4. Update GUI to display the new sensor

### Custom Data Formats
Modify the appropriate `DataReader` class to handle different CSV formats or data sources.

## Troubleshooting

- **Qt not found**: Ensure Qt6 is installed and CMAKE_PREFIX_PATH includes Qt installation
- **OpenCV errors**: Verify OpenCV 4.x is installed with development headers
- **Data loading fails**: Check CSV file formats match expected headers
- **Images not displaying**: Ensure image files exist and have correct timestamp-based filenames

## License

[Add your license information here]