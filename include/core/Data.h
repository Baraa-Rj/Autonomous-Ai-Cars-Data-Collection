#pragma once
#include <string>

class Data {
public:
    double timestamp;
    
    Data(double ts) : timestamp(ts) {}
    virtual ~Data() = default;
    
    virtual std::string toString() const = 0;
};