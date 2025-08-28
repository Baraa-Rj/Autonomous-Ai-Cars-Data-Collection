#pragma once
#include <vector>
#include "data/GPSData.h"
#include "data/IMUData.h"
#include "data/SpeedData.h"
#include "data/BrakeData.h"
#include "data/ThrottleData.h"
#include "data/SteeringData.h"
#include "data/ImageData.h"

class SensorDataStore {
public:
    std::vector<GPSData> gps_data;
    std::vector<IMUData> imu_data;
    std::vector<SpeedData> speed_data;
    std::vector<BrakeData> brake_data;
    std::vector<ThrottleData> throttle_data;
    std::vector<SteeringData> steering_data;
    
    std::vector<ImageData> front_images;
    std::vector<ImageData> back_images;
    std::vector<ImageData> left_images;
    std::vector<ImageData> right_images;
    
    void clear();
    size_t getTotalDataPoints() const;
};