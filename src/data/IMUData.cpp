#include "headers/data/IMUData.h"

IMUData::IMUData(std::chrono::system_clock::time_point timestamp, std::vector<float> acceleration, std::vector<float> gyroscope)
    : Data(timestamp), acceleration(std::move(acceleration)), gyroscope(std::move(gyroscope)) {}

IMUData::~IMUData() {}

std::vector<float> IMUData::getAcceleration() const {
    return acceleration;
}

std::vector<float> IMUData::getGyroscope() const {