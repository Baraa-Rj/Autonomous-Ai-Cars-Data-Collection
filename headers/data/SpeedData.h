#pragma once

#include "Data.h"

class SpeedData : public Data {
protected:
    float speed;

public:
    SpeedData(std::chrono::system_clock::time_point timestamp, float speed);
    ~SpeedData();

    float getSpeed() const;

    void setSpeed(float speed);
};