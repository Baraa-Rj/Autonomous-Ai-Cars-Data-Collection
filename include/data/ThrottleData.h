#pragma once
#include "core/Data.h"

class ThrottleData : public Data {
public:
    double data_value;
    
    ThrottleData(double ts, double value) : Data(ts), data_value(value) {}
    
    std::string toString() const override;
};