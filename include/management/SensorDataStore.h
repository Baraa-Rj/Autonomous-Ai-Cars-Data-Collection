#pragma once
#include "readers/DataReaderFactory.h"
#include "core/Data.h"
#include <map>
#include <memory>
#include <vector>
#include <string>


class SensorDataStore {
public:

    void addData(DataType type, std::unique_ptr<Data> data);
    

    void addImageData(const std::string& cameraPosition, std::unique_ptr<Data> data);
    

    Data* getCurrentDataByType(DataType type) const;
    
    Data* getCurrentImageData(const std::string& cameraPosition) const;
    
   
    std::vector<Data*> getCurrentData() const;
    
 
    void clear();
    
   
    size_t getTotalDataPoints() const;
    
    Data* getCurrentGPS() const { return getCurrentDataByType(DataType::GPS); }
    Data* getCurrentIMU() const { return getCurrentDataByType(DataType::IMU); }
    Data* getCurrentSpeed() const { return getCurrentDataByType(DataType::SPEED); }
    Data* getCurrentBrake() const { return getCurrentDataByType(DataType::BRAKE); }
    Data* getCurrentThrottle() const { return getCurrentDataByType(DataType::THROTTLE); }
    Data* getCurrentSteering() const { return getCurrentDataByType(DataType::STEERING); }
    
    Data* getCurrentFrontImage() const { return getCurrentImageData("front"); }
    Data* getCurrentBackImage() const { return getCurrentImageData("back"); }
    Data* getCurrentLeftImage() const { return getCurrentImageData("left"); }
    Data* getCurrentRightImage() const { return getCurrentImageData("right"); }

private:
    std::map<DataType, std::unique_ptr<Data>> sensorData;
    std::map<std::string, std::unique_ptr<Data>> imageData;
};
