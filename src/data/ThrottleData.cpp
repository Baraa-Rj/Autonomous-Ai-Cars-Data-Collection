#include "headers/data/ThrottleData.h"

ThrottleData::ThrottleData(std::DateTime timestamp, float position) : Data(timestamp), position(position) {}

ThrottleData::~ThrottleData() {}

float ThrottleData::getPosition() const {
    return position;
}