#include "readers/ReadersManager.h"
#include <QDir>
#include <QFileInfo>
#include <iostream>

ReadersManager::ReadersManager(const std::string& dataPath) : baseDataPath(dataPath) {
    loadAllData();
}

void ReadersManager::loadAllData() {
    std::cout << "Loading data from: " << baseDataPath << std::endl;
    
    loadCSVData();
    loadImageData();
    computeGlobalTimeline();
    
    std::cout << "Data loading completed" << std::endl;
}

void ReadersManager::loadCSVData() {
    std::string base = baseDataPath;
    
    // Create and load CSV readers
    std::pair<DataType, std::string> csvFiles[] = {
        {DataType::GPS, base + "/gps.csv"},
        {DataType::SPEED, base + "/speed.csv"},
        {DataType::BRAKE, base + "/brake.csv"},
        {DataType::THROTTLE, base + "/throttle.csv"},
        {DataType::STEERING, base + "/steering.csv"},
        {DataType::IMU, base + "/imu.csv"}
    };
    
    for (const auto& [type, path] : csvFiles) {
        if (QFileInfo(QString::fromStdString(path)).exists()) {
            auto reader = createReader(type, path);
            if (reader) {
                reader->loadData(path);
                readers[type] = std::unique_ptr<AbstractDataReader>(reader);
                std::cout << "Loaded " << path << std::endl;
            }
        }
    }
}

void ReadersManager::loadImageData() {
    // ImageReader handles directory-based loading
    std::string imageBase = baseDataPath + "/3d_images";
    
    std::pair<DataType, std::string> imageDirs[] = {
        {DataType::LEFT_IMAGE, imageBase + "/left"},
        {DataType::FRONT_IMAGE, imageBase + "/front"},
        {DataType::RIGHT_IMAGE, imageBase + "/right"},
        {DataType::BACK_IMAGE, imageBase + "/back"}
    };
    
    for (const auto& [type, dir] : imageDirs) {
        if (QDir(QString::fromStdString(dir)).exists()) {
            auto reader = new ImageReader(dir);
            reader->loadData(dir);
            readers[type] = std::unique_ptr<AbstractDataReader>(reader);
            std::cout << "Loaded images from " << dir << std::endl;
        }
    }
}

void ReadersManager::computeGlobalTimeline() {
    bool init = false;
    
    for (const auto& [type, reader] : readers) {
        auto data = reader->getAllData();
        if (data.empty()) continue;
        
        auto start = data.front().getTimestamp();
        auto end = data.back().getTimestamp();
        
        if (!init) {
            globalStart = start;
            globalEnd = end;
            init = true;
        } else {
            if (start < globalStart) globalStart = start;
            if (end > globalEnd) globalEnd = end;
        }
    }
}

AbstractDataReader* ReadersManager::getReader(DataType type) {
    auto it = readers.find(type);
    return (it != readers.end()) ? it->second.get() : nullptr;
}

AbstractDataReader* ReadersManager::createReader(DataType type, const std::string& path) {
    switch (type) {
        case DataType::BRAKE:
            return new BrakeReader(path);
        case DataType::GPS:
            return new GpsReader(path);
        case DataType::IMU:
            return new IMUReader(path);
        case DataType::STEERING:
            return new SteeringReader(path);
        case DataType::THROTTLE:
            return new ThrottleReader(path);
        case DataType::LEFT_IMAGE:
        case DataType::RIGHT_IMAGE:
        case DataType::FRONT_IMAGE:
        case DataType::BACK_IMAGE:
            return new ImageReader(path);
        default:
            return nullptr;
    }
}

std::chrono::system_clock::time_point ReadersManager::getGlobalStart() const {
    return globalStart;
}

std::chrono::system_clock::time_point ReadersManager::getGlobalEnd() const {
    return globalEnd;
}


