#pragma once
#include "DataReader.h"
#include <memory>
#include <string>

enum class DataType {
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
    static std::unique_ptr<DataReader> createReader(DataType type);
    static DataType getDataTypeFromFilename(const std::string& filename);
};