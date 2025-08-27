#include "data/SpeedData.h"
#include "data/DataStore.h"

SpeedData::SpeedData(std::chrono::system_clock::time_point timestamp, float speed)
    : Data(timestamp), speed(speed) {}

SpeedData::~SpeedData() {}

float SpeedData::getSpeed() const {
    return speed;
}

void SpeedData::setSpeed(float speed) {
    this->speed = speed;
}

DataType SpeedData::getType() const {
    return DataType::SPEED;
}


