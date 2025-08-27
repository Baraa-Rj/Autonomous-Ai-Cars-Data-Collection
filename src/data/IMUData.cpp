#include "data/IMUData.h"
#include "data/DataStore.h"

IMUData::IMUData(std::chrono::system_clock::time_point timestamp, std::vector<float> acceleration, std::vector<float> gyroscope)
    : Data(timestamp), acceleration(std::move(acceleration)), gyroscope(std::move(gyroscope)) {}

IMUData::~IMUData() {}

std::vector<float> IMUData::getAcceleration() const {
    return acceleration;
}

std::vector<float> IMUData::getGyroscope() const {
    return gyroscope;
}

DataType IMUData::getType() const {
    return DataType::IMU;
}