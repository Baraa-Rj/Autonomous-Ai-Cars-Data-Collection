#include "headers/data/SteeringData.h"

SteeringData::SteeringData(std::DateTime timestamp, float angle) : Data(timestamp), angle(angle) {}

SteeringData::~SteeringData() {}

float SteeringData::getAngle() const {
    return angle;
}