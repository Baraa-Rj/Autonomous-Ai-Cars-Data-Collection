#pragma once

#include "Data.h"
#include <vector>

class IMUData : public Data {
protected:
    std::vector<float> acceleration;
    std::vector<float> gyroscope;

public:
    IMUData(std::chrono::system_clock::time_point timestamp, std::vector<float> acceleration, std::vector<float> gyroscope);
    ~IMUData();

    std::vector<float> getAcceleration() const;
    std::vector<float> getGyroscope() const;

    void setAcceleration(const std::vector<float>& acceleration);
    void setGyroscope(const std::vector<float>& gyroscope);
};