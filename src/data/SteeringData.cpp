#include "data/SteeringData.h"
#include "data/DataStore.h"

SteeringData::SteeringData(std::chrono::system_clock::time_point timestamp, float angle) : Data(timestamp), angle(angle) {}

SteeringData::~SteeringData() {}

float SteeringData::getAngle() const {
    return angle;
}

DataType SteeringData::getType() const {
    return DataType::STEERING;
}