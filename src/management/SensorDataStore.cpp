#include "management/SensorDataStore.h"
#include <algorithm>

void SensorDataStore::addData(DataType type, std::unique_ptr<Data> data) {
    if (data) {
        sensorData[type] = std::move(data);
    }
}

void SensorDataStore::addImageData(const std::string& cameraPosition, std::unique_ptr<Data> data) {
    if (data) {
        imageData[cameraPosition] = std::move(data);
    }
}

Data* SensorDataStore::getCurrentDataByType(DataType type) const {
    auto it = sensorData.find(type);
    return (it != sensorData.end()) ? it->second.get() : nullptr;
}

Data* SensorDataStore::getCurrentImageData(const std::string& cameraPosition) const {
    auto it = imageData.find(cameraPosition);
    return (it != imageData.end()) ? it->second.get() : nullptr;
}

std::vector<Data*> SensorDataStore::getCurrentData() const {
    std::vector<Data*> allData;
    
    for (const auto& [type, data] : sensorData) {
        if (data) {
            allData.push_back(data.get());
        }
    }
    
    for (const auto& [position, data] : imageData) {
        if (data) {
            allData.push_back(data.get());
        }
    }
    
    std::sort(allData.begin(), allData.end(), 
              [](const Data* a, const Data* b) {
                  return a->timestamp < b->timestamp;
              });
    
    return allData;
}

void SensorDataStore::clear() {
    sensorData.clear();
    imageData.clear();
}

size_t SensorDataStore::getTotalDataPoints() const {
    return sensorData.size() + imageData.size();
}