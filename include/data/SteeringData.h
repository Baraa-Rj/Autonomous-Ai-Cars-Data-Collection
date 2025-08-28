#pragma once
#include "core/Data.h"

class SteeringData : public Data {
public:
    double data_value;
    
    SteeringData(double ts, double value) : Data(ts), data_value(value) {}
    
    std::string toString() const override;
};