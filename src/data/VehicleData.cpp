#include "headers/data/VehicleData.h"

VehicleData::VehicleData(std::DateTime timestamp, float speed, float acceleration, float steeringAngle, float yawRate) : Data(timestamp), speed(speed), acceleration(acceleration), steeringAngle(steeringAngle), yawRate(yawRate) {}

VehicleData::~VehicleData() {}

float VehicleData::getSpeed() const {
    return speed;
}