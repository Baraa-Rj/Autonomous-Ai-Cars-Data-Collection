#include "headers/data/ThorttleData.h"

ThrottleData::ThrottleData(std::chrono::system_clock::time_point timestamp, float position) : Data(timestamp), position(position) {}

ThrottleData::~ThrottleData() {}

float ThrottleData::getPosition() const {
    return position;
}