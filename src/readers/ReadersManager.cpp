#include "readers/ReadersManager.h"

AbstractDataReader* ReadersManager::createReader(DataType type, const std::string& path) {
    switch (type) {
        case DataType::BRAKE:
            return new BrakeReader(path);
        case DataType::GPS:
            return new GpsReader(path);
        case DataType::IMU:
            return new IMUReader(path);
        case DataType::STEERING:
            return new SteeringReader(path);
        case DataType::THROTTLE:
            return new ThrottleReader(path);
        case DataType::LEFT_IMAGE:
        case DataType::RIGHT_IMAGE:
        case DataType::FRONT_IMAGE:
        case DataType::BACK_IMAGE:
            return new ImageReader(path);
        default:
            return nullptr;
    }
}


