#pragma once

#include <string>
#include <memory>
#include <map>
#include "../data/DataStore.h"
#include "AbstractDataReader.h"
#include "../sync/TimelineManager.h"

class ReadersManager {
private:
    std::map<DataType, std::unique_ptr<AbstractDataReader>> readers;
    std::unique_ptr<TimelineManager> timelineManager;
    std::string baseDataPath;
    
public:
    ReadersManager(const std::string& dataPath = "sample_data");
    ~ReadersManager() = default;
    
    // Core responsibility: Managing reader instances
    void loadAllData();
    AbstractDataReader* getReader(DataType type);
    
    // Timeline access (delegates to TimelineManager)
    std::chrono::system_clock::time_point getGlobalStart() const;
    std::chrono::system_clock::time_point getGlobalEnd() const;
    TimelineManager* getTimelineManager() const;
    
private:
    void loadCSVData();
    void loadImageData();
    std::string getCSVPath(DataType type) const;
    std::string getImageDirPath(DataType type) const;
};