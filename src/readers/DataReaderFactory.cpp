#include "readers/DataReaderFactory.h"
#include "readers/GPSDataReader.h"
#include "readers/IMUDataReader.h"
#include "readers/SpeedDataReader.h"
#include "readers/BrakeDataReader.h"
#include "readers/ThrottleDataReader.h"
#include "readers/SteeringDataReader.h"
#include "readers/ImageDataReader.h"
#include <algorithm>

std::unique_ptr<DataReader> DataReaderFactory::createReader(SensorType type) {
    switch (type) {
        case SensorType::GPS:
            return std::make_unique<GPSDataReader>();
        case SensorType::IMU:
            return std::make_unique<IMUDataReader>();
        case SensorType::SPEED:
            return std::make_unique<SpeedDataReader>();
        case SensorType::BRAKE:
            return std::make_unique<BrakeDataReader>();
        case SensorType::THROTTLE:
            return std::make_unique<ThrottleDataReader>();
        case SensorType::STEERING:
            return std::make_unique<SteeringDataReader>();
        case SensorType::IMAGE:
            return std::make_unique<ImageDataReader>();
        default:
            return nullptr;
    }
}

SensorType DataReaderFactory::getSensorTypeFromFilename(const std::string& filename) {
    std::string lowerFilename = filename;
    std::transform(lowerFilename.begin(), lowerFilename.end(), lowerFilename.begin(), ::tolower);
    
    if (lowerFilename.find("gps") != std::string::npos) {
        return SensorType::GPS;
    } else if (lowerFilename.find("imu") != std::string::npos) {
        return SensorType::IMU;
    } else if (lowerFilename.find("speed") != std::string::npos) {
        return SensorType::SPEED;
    } else if (lowerFilename.find("brake") != std::string::npos) {
        return SensorType::BRAKE;
    } else if (lowerFilename.find("throttle") != std::string::npos) {
        return SensorType::THROTTLE;
    } else if (lowerFilename.find("steering") != std::string::npos) {
        return SensorType::STEERING;
    } else {
        return SensorType::IMAGE; // Default for directories
    }
}