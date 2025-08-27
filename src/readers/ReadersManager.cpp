#include "readers/ReadersManager.h"

ReadersManager::ReadersManager() {}

ReadersManager::~ReadersManager() {}

AbstractDataReader* ReadersManager::createReader(DataType type, std::string path) {
    switch (type) {
        case DataType::BRAKE:
            return new BrakeReader(path);
        case DataType::GPS:
            return new GpsReader(path);
        case DataType::FRONT_IMAGE:
        case DataType::BACK_IMAGE:
        case DataType::LEFT_IMAGE:
        case DataType::RIGHT_IMAGE:
            return new ImageReader(path);
        case DataType::IMU:
            return new IMUReader(path);
        case DataType::STEERING:
            return new SteeringReader(path);
        case DataType::THROTTLE:
            return new ThrottleReader(path);
        default:
            return nullptr;
    }
}