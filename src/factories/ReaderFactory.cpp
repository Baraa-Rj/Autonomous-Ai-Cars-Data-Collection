#include "factories/ReaderFactory.h"
#include "readers/AbstractDataReader.h"
#include "readers/BrakeReader.h"
#include "readers/GpsReader.h"
#include "readers/ImageReader.h"
#include "readers/IMUReader.h"
#include "readers/SpeedReader.h"
#include "readers/SteeringReader.h"
#include "readers/ThrottleReader.h"

std::unique_ptr<AbstractDataReader> ReaderFactory::createReader(DataType type, const std::string& path) {
    switch (type) {
        case DataType::GPS:
            return std::make_unique<GpsReader>(path);
        case DataType::SPEED:
            return std::make_unique<SpeedReader>(path);
        case DataType::BRAKE:
            return std::make_unique<BrakeReader>(path);
        case DataType::THROTTLE:
            return std::make_unique<ThrottleReader>(path);
        case DataType::STEERING:
            return std::make_unique<SteeringReader>(path);
        case DataType::IMU:
            return std::make_unique<IMUReader>(path);
        case DataType::LEFT_IMAGE:
        case DataType::FRONT_IMAGE:
        case DataType::RIGHT_IMAGE:
        case DataType::BACK_IMAGE:
            return std::make_unique<ImageReader>(path);
        default:
            return nullptr;
    }
}