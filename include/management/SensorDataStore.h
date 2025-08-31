#pragma once
#include "readers/DataReaderFactory.h"
#include "core/Data.h"
#include <map>
#include <memory>
#include <vector>
#include <string>

/**
 * Centralized data storage using map-based approach for memory efficiency.
 * Stores current data by DataType and provides efficient access methods.
 */
class SensorDataStore {
public:
    /**
     * Add data for a specific sensor type
     */
    void addData(DataType type, std::unique_ptr<Data> data);
    
    /**
     * Add data with camera position for image data
     */
    void addImageData(const std::string& cameraPosition, std::unique_ptr<Data> data);
    
    /**
     * Get current data by type
     */
    Data* getCurrentDataByType(DataType type) const;
    
    /**
     * Get current image data by camera position
     */
    Data* getCurrentImageData(const std::string& cameraPosition) const;
    
    /**
     * Get all current data as a list
     */
    std::vector<Data*> getCurrentData() const;
    
    /**
     * Clear all stored data
     */
    void clear();
    
    /**
     * Get total number of active data points
     */
    size_t getTotalDataPoints() const;
    
    // Convenience methods for specific sensor types
    Data* getCurrentGPS() const { return getCurrentDataByType(DataType::GPS); }
    Data* getCurrentIMU() const { return getCurrentDataByType(DataType::IMU); }
    Data* getCurrentSpeed() const { return getCurrentDataByType(DataType::SPEED); }
    Data* getCurrentBrake() const { return getCurrentDataByType(DataType::BRAKE); }
    Data* getCurrentThrottle() const { return getCurrentDataByType(DataType::THROTTLE); }
    Data* getCurrentSteering() const { return getCurrentDataByType(DataType::STEERING); }
    
    // Camera convenience methods
    Data* getCurrentFrontImage() const { return getCurrentImageData("front"); }
    Data* getCurrentBackImage() const { return getCurrentImageData("back"); }
    Data* getCurrentLeftImage() const { return getCurrentImageData("left"); }
    Data* getCurrentRightImage() const { return getCurrentImageData("right"); }

private:
    // Map from DataType to current sensor data
    std::map<DataType, std::unique_ptr<Data>> sensorData;
    
    // Map from camera position to current image data
    std::map<std::string, std::unique_ptr<Data>> imageData;
};
