#pragma once

#include "Data.h"

class BrakeData : public Data {
protected:
    float pressure;

public:
    BrakeData(std::chrono::system_clock::time_point timestamp, float pressure);
    ~BrakeData();

    float getPressure() const;

    void setPressure(float pressure);
    void print() const;
};