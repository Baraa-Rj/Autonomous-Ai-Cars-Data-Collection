#pragma once

#include "Data.h"

class SteeringData : public Data {
protected:
    float angle;

public:
    SteeringData(std::chrono::system_clock::time_point timestamp, float angle);
    ~SteeringData();

    float getAngle() const;

    void setAngle(float angle);
};