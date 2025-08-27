#pragma once

#include "Data.h"

class ThrottleData : public Data {
protected:
    float position;

public:
    ThrottleData(std::chrono::system_clock::time_point timestamp, float position);
    ~ThrottleData();
    float getPosition() const;

    void setPosition(float position);
};