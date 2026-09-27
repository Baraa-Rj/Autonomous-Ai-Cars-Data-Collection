#pragma once

#include "Data.h"

template<typename T, DataType TYPE>
class SingleValueData : public Data {
protected:
    T value;

public:
    SingleValueData(std::chrono::system_clock::time_point timestamp, T val)
        : Data(timestamp), value(val) {}
    
    virtual ~SingleValueData() = default;
    
    T getValue() const { return value; }
    void setValue(T val) { value = val; }
    
    DataType getType() const override { return TYPE; }
};