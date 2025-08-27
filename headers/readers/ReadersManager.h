#pragma once

#include <string>
#include <memory>
#include <map>
#include "../data/DataStore.h"
#include "AbstractDataReader.h"
#include "BrakeReader.h"
#include "GpsReader.h"
#include "ImageReader.h"
#include "IMUReader.h"
#include "SpeedReader.h"
#include "SteeringReader.h"
#include "ThrottleReader.h"

class ReadersManager {
private:
    std::map<DataType, std::unique_ptr<AbstractDataReader>> readers;
    std::string baseDataPath;

public:
    ReadersManager(const std::string& dataPath = "sample_data");
    ~ReadersManager() = default;
    
    void loadAllData();
    AbstractDataReader* getReader(DataType type);
    AbstractDataReader* createReader(DataType type, const std::string& path);
    
    std::chrono::system_clock::time_point getGlobalStart() const;
    std::chrono::system_clock::time_point getGlobalEnd() const;
    
private:
    void loadCSVData();
    void loadImageData();
    void computeGlobalTimeline();
    
    std::chrono::system_clock::time_point globalStart;
    std::chrono::system_clock::time_point globalEnd;
};


