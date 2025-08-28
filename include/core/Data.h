#pragma once
#include <string>

// Abstract base class for all data types
class Data {
public:
    double timestamp;
    
    Data(double ts) : timestamp(ts) {}
    virtual ~Data() = default;
    
    // Pure virtual function to get data as string for display
    virtual std::string toString() const = 0;
};