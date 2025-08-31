#include "readers/DataReaderFactory.h"
#include "readers/GPSDataReader.h"
#include "readers/IMUDataReader.h"
#include "readers/SpeedDataReader.h"
#include "readers/BrakeDataReader.h"
#include "readers/ThrottleDataReader.h"
#include "readers/SteeringDataReader.h"
#include "readers/ImageDataReader.h"
#include <algorithm>

std::unique_ptr<DataReader> DataReaderFactory::createReader(DataType type) {
    switch (type) {
        case DataType::GPS:
            return std::make_unique<GPSDataReader>();
        case DataType::IMU:
            return std::make_unique<IMUDataReader>();
        case DataType::SPEED:
            return std::make_unique<SpeedDataReader>();
        case DataType::BRAKE:
            return std::make_unique<BrakeDataReader>();
        case DataType::THROTTLE:
            return std::make_unique<ThrottleDataReader>();
        case DataType::STEERING:
            return std::make_unique<SteeringDataReader>();
        case DataType::IMAGE:
            return std::make_unique<ImageDataReader>();
        default:
            return nullptr;
    }
}

DataType DataReaderFactory::getDataTypeFromFilename(const std::string& filename) {
    std::string lowerFilename = filename;
    std::transform(lowerFilename.begin(), lowerFilename.end(), lowerFilename.begin(), ::tolower);
    
    if (lowerFilename.find("gps") != std::string::npos) {
        return DataType::GPS;
    } else if (lowerFilename.find("imu") != std::string::npos) {
        return DataType::IMU;
    } else if (lowerFilename.find("speed") != std::string::npos) {
        return DataType::SPEED;
    } else if (lowerFilename.find("brake") != std::string::npos) {
        return DataType::BRAKE;
    } else if (lowerFilename.find("throttle") != std::string::npos) {
        return DataType::THROTTLE;
    } else if (lowerFilename.find("steering") != std::string::npos) {
        return DataType::STEERING;
    } else {
        return DataType::IMAGE; 
    }
}