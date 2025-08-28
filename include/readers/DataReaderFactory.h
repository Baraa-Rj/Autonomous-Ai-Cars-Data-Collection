#pragma once
#include "DataReader.h"
#include <memory>
#include <string>

enum class SensorType {
    GPS,
    IMU, 
    SPEED,
    BRAKE,
    THROTTLE,
    STEERING,
    IMAGE
};

class DataReaderFactory {
public:
    static std::unique_ptr<DataReader> createReader(SensorType type);
    static SensorType getSensorTypeFromFilename(const std::string& filename);
};