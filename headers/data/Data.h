#pragma once

#include <chrono>

enum class DataType;

class Data {
protected:
    std::chrono::system_clock::time_point timestamp;

public:
    explicit Data(std::chrono::system_clock::time_point timestamp);
    virtual ~Data();

    std::chrono::system_clock::time_point getTimestamp() const;
    virtual DataType getType() const = 0;
};