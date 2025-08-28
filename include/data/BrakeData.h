#pragma once
#include "core/Data.h"

class BrakeData : public Data {
public:
    double data_value;
    
    BrakeData(double ts, double value) : Data(ts), data_value(value) {}
    
    std::string toString() const override;
};