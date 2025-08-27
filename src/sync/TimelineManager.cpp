#include "sync/TimelineManager.h"
#include "readers/AbstractDataReader.h"

void TimelineManager::computeGlobalTimeline(const std::map<DataType, std::unique_ptr<AbstractDataReader>>& readers) {
    bool first = true;
    
    for (const auto& [type, reader] : readers) {
        auto data = reader->getAllData();
        if (data.empty()) continue;
        
        auto start = data.front()->getTimestamp();
        auto end = data.back()->getTimestamp();
        
        if (first) {
            globalStart = start;
            globalEnd = end;
            first = false;
        } else {
            updateTimeline(start, end);
        }
    }
    
    initialized = true;
}

std::chrono::system_clock::time_point TimelineManager::getGlobalStart() const {
    return globalStart;
}

std::chrono::system_clock::time_point TimelineManager::getGlobalEnd() const {
    return globalEnd;
}

void TimelineManager::updateTimeline(std::chrono::system_clock::time_point start, 
                                   std::chrono::system_clock::time_point end) {
    if (start < globalStart) globalStart = start;
    if (end > globalEnd) globalEnd = end;
}