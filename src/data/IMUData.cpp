#include "headers/data/IMUData.h"

IMUData::IMUData(std::DateTime timestamp, std::vector<float> acceleration, std::vector<float> gyroscope) : Data(timestamp), acceleration(acceleration), gyroscope(gyroscope) {}

IMUData::~IMUData() {}

std::vector<float> IMUData::getAcceleration() const {
    return acceleration;
}

std::vector<float> IMUData::getGyroscope() const {