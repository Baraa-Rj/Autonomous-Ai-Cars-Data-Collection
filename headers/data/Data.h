#pragma once

#include <chrono>

class Data {
protected:
    std::chrono::system_clock::time_point timestamp;

public:
    explicit Data(std::chrono::system_clock::time_point timestamp);
    virtual ~Data();

    std::chrono::system_clock::time_point getTimestamp() const;
};