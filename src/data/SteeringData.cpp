#include "data/SteeringData.h"

SteeringData::SteeringData(std::chrono::system_clock::time_point timestamp, float angle) : Data(timestamp), angle(angle) {}

SteeringData::~SteeringData() {}

float SteeringData::getAngle() const {
    return angle;
}