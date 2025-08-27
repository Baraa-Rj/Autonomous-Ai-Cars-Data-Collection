#include "data/BrakeData.h"

BrakeData::BrakeData(std::chrono::system_clock::time_point timestamp, float pressure) : Data(timestamp), pressure(pressure) {}

BrakeData::~BrakeData() {}

float BrakeData::getPressure() const {
    return pressure;
}

void BrakeData::setPressure(float pressure) {
    this->pressure = pressure;
}