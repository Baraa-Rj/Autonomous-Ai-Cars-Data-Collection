#pragma once
#include "core/Data.h"

class SpeedData : public Data {
public:
    double data_value;
    
    SpeedData(double ts, double value) : Data(ts), data_value(value) {}
    
    std::string toString() const override;
};