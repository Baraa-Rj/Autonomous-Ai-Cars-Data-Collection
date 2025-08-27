#include "headers/data/BrakeData.h"

BrakeData::BrakeData(std::DateTime timestamp, float pressure) : Data(timestamp), pressure(pressure) {}

BrakeData::~BrakeData() {}

float BrakeData::getPressure() const {
    return pressure;
}

void BrakeData::setPressure(float pressure) {
    this->pressure = pressure;
}