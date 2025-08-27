#pragma once

#include <chrono>
#include <map>
#include <memory>
#include "../data/DataStore.h"

class AbstractDataReader;

class TimelineManager {
private:
    std::chrono::system_clock::time_point globalStart;
    std::chrono::system_clock::time_point globalEnd;
    bool initialized{false};
    
public:
    TimelineManager() = default;
    
    void computeGlobalTimeline(const std::map<DataType, std::unique_ptr<AbstractDataReader>>& readers);
    
    std::chrono::system_clock::time_point getGlobalStart() const;
    std::chrono::system_clock::time_point getGlobalEnd() const;
    
    bool isInitialized() const { return initialized; }
    
private:
    void updateTimeline(std::chrono::system_clock::time_point start, 
                       std::chrono::system_clock::time_point end);
};