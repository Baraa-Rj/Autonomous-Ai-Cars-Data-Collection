#pragma once
#include "data/GPSData.h"
#include "data/IMUData.h"
#include "data/SpeedData.h"
#include "data/BrakeData.h"
#include "data/ThrottleData.h"
#include "data/SteeringData.h"
#include "data/ImageData.h"
#include <vector>

/**
 * Centralized data storage for all sensor types.
 * Contains vectors of sensor data organized by type for efficient access.
 */
struct SensorDataStore {
    // Sensor data vectors
    std::vector<GPSData> gps_data;
    std::vector<IMUData> imu_data;
    std::vector<SpeedData> speed_data;
    std::vector<BrakeData> brake_data;
    std::vector<ThrottleData> throttle_data;
    std::vector<SteeringData> steering_data;
    
    // Image data vectors (one per camera)
    std::vector<ImageData> front_images;
    std::vector<ImageData> back_images;
    std::vector<ImageData> left_images;
    std::vector<ImageData> right_images;
    
    /**
     * Clear all stored data
     */
    void clear() {
        gps_data.clear();
        imu_data.clear();
        speed_data.clear();
        brake_data.clear();
        throttle_data.clear();
        steering_data.clear();
        front_images.clear();
        back_images.clear();
        left_images.clear();
        right_images.clear();
    }
    
    /**
     * Get total number of data points across all sensors
     */
    size_t getTotalDataPoints() const {
        return gps_data.size() + imu_data.size() + speed_data.size() + 
               brake_data.size() + throttle_data.size() + steering_data.size() +
               front_images.size() + back_images.size() + 
               left_images.size() + right_images.size();
    }
};
