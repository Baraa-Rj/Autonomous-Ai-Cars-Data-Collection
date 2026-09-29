#include "management/DataManager.h"
#include "readers/ImageDataReader.h"
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <thread>

DataManager::DataManager(QObject* parent) 
    : QObject(parent) {
}

bool DataManager::initializeStreamingReaders(const std::string& dataDirectory) {
    if (!std::filesystem::exists(dataDirectory)) {
        emit dataLoadingError("Data directory does not exist: " + QString::fromStdString(dataDirectory));
        return false;
    }
    
    std::vector<std::pair<std::string, DataType>> csvFiles = {
        {dataDirectory + "/gps.csv", DataType::GPS},
        {dataDirectory + "/imu.csv", DataType::IMU},
        {dataDirectory + "/speed.csv", DataType::SPEED},
        {dataDirectory + "/brake.csv", DataType::BRAKE},
        {dataDirectory + "/throttle.csv", DataType::THROTTLE},
        {dataDirectory + "/steering.csv", DataType::STEERING}
    };
    
    for (const auto& [filePath, sensorType] : csvFiles) {
        if (std::filesystem::exists(filePath)) {
            auto reader = DataReaderFactory::createReader(sensorType);
            if (reader && reader->loadAllData(filePath)) {
                sensorReaders[sensorType] = std::move(reader);
                
                auto& readerRef = sensorReaders[sensorType];
                for (size_t i = 0; i < readerRef->getDataCount(); ++i) {
                    Data* data = readerRef->getDataAt(i);
                    if (data) {
                        clockManager.updateRange(data->timestamp);
                    }
                }
            }
        }
    }
    
    std::vector<std::string> cameras = {"front", "back", "left", "right"};
    for (const std::string& camera : cameras) {
        std::string dirPath = dataDirectory + "/3d_images/" + camera;
        if (std::filesystem::exists(dirPath)) {
            auto reader = std::make_unique<ImageDataReader>();
            imageReaders[camera] = std::move(reader);
            preloadImageFileList(camera, dirPath);
        }
    }
    
    return true;
}

void DataManager::updateSensorData(double targetTimestamp) {
    std::unique_lock<std::shared_mutex> lock(dataStoreMutex);
    
    for (auto& [type, reader] : sensorReaders) {
        if (!reader || reader->getDataCount() == 0) continue;
        
        Data* bestData = findClosestData(reader.get(), targetTimestamp);
        if (bestData) {
            Data* currentData = dataStore.getCurrentDataByType(type);
            
            if (!currentData || currentData->timestamp != bestData->timestamp) {
                auto dataCopy = cloneData(bestData, type);
                if (dataCopy) {
                    dataStore.addData(type, std::move(dataCopy));
                }
            }
        }
    }
    
    updateImageDataForTimestamp(targetTimestamp);
}

void DataManager::preloadImageFileList(const std::string& cameraName, const std::string& dirPath) {
    if (!std::filesystem::exists(dirPath)) {
        return;
    }
    
    std::vector<std::pair<double, std::string>> imageFiles;
    
    for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            
            if (ext == ".jpeg" || ext == ".jpg") {
                try {
                    std::string filename = entry.path().stem().string();
                    double timestamp = std::stod(filename);
                    imageFiles.emplace_back(timestamp, entry.path().string());
                } catch (const std::exception& e) {
                    std::cerr << "Error parsing image timestamp: " << entry.path() << " - " << e.what() << std::endl;
                }
            }
        }
    }
    
    std::sort(imageFiles.begin(), imageFiles.end());
    
    imageFilesByCamera[cameraName] = std::move(imageFiles);
    
    std::cout << "Preloaded " << imageFilesByCamera[cameraName].size() << " images for " << cameraName << " camera" << std::endl;
}

void DataManager::updateImageDataForTimestamp(double targetTimestamp) {
    for (auto& [cameraName, imageFiles] : imageFilesByCamera) {
        if (imageFiles.empty()) continue;
        
        Data* currentImageData = dataStore.getCurrentImageData(cameraName);
        
        // First frame with timestamp >= target; searched on every update so seeking backward works.
        auto it = std::lower_bound(imageFiles.begin(), imageFiles.end(), targetTimestamp,
            [](const std::pair<double, std::string>& file, double target) {
                return file.first < target;
            });
        
        if (it != imageFiles.end()) {
            const auto& [timestamp, filepath] = *it;
            if (!currentImageData || currentImageData->timestamp != timestamp) {
                auto imageData = std::make_unique<ImageData>(timestamp, filepath);
                clockManager.updateRange(timestamp);
                dataStore.addImageData(cameraName, std::move(imageData));
            }
        }
    }
}

void DataManager::loadAllSensorDataAsync(const std::string& dataDirectory) {
    loadingFlag.store(true);
    cancelLoadingFlag.store(false);
    clearData();
    
    if (!initializeStreamingReaders(dataDirectory)) {
        loadingFlag.store(false);
        return;
    }
    
    std::thread([this, dataDirectory]() {
        try {
            emit dataLoadingProgress(50);
            
            
            updateSensorData(clockManager.getMinTimestamp());
            
            emit dataLoadingProgress(100);
            emit dataLoaded();
            
            size_t totalDataPoints = 0;
            for (const auto& [type, reader] : sensorReaders) {
                if (reader) {
                    totalDataPoints += reader->getDataCount();
                }
            }
            
            std::cout << "Preloaded " << totalDataPoints << " CSV data points. Time range: " 
                     << clockManager.getMinTimestamp() << " to " 
                     << clockManager.getMaxTimestamp() << std::endl;
            
        } catch (const std::exception& e) {
            emit dataLoadingError("Error loading preloaded data: " + QString::fromStdString(e.what()));
        }
        
        loadingFlag.store(false);
    }).detach();
}

void DataManager::cancelLoading() {
    cancelLoadingFlag.store(true);
}

void DataManager::clearData() {
    std::unique_lock<std::shared_mutex> lock(dataStoreMutex);
    dataStore.clear();
    
    for (auto& [type, reader] : sensorReaders) {
        if (reader) {
            reader->clearData();
        }
    }
    sensorReaders.clear();
    imageReaders.clear();
    
    imageFilesByCamera.clear();
    
    std::lock_guard<std::mutex> clockLock(clockManagerMutex);
    clockManager.resetRange();
}

Data* DataManager::findClosestData(DataReader* reader, double targetTimestamp) {
    if (!reader || reader->getDataCount() == 0) return nullptr;
    
    Data* bestData = nullptr;
    for (size_t i = 0; i < reader->getDataCount(); ++i) {
        Data* data = reader->getDataAt(i);
        if (data && data->timestamp <= targetTimestamp) {
            bestData = data;
        } else {
            break; 
        }
    }
    
    return bestData;
}

std::unique_ptr<Data> DataManager::cloneData(Data* data, DataType type) {
    if (!data) return nullptr;
    
    switch (type) {
        case DataType::GPS:
            return std::make_unique<GPSData>(*static_cast<GPSData*>(data));
        case DataType::IMU:
            return std::make_unique<IMUData>(*static_cast<IMUData*>(data));
        case DataType::SPEED:
            return std::make_unique<SpeedData>(*static_cast<SpeedData*>(data));
        case DataType::BRAKE:
            return std::make_unique<BrakeData>(*static_cast<BrakeData*>(data));
        case DataType::THROTTLE:
            return std::make_unique<ThrottleData>(*static_cast<ThrottleData*>(data));
        case DataType::STEERING:
            return std::make_unique<SteeringData>(*static_cast<SteeringData*>(data));
        default:
            return nullptr;
    }
}

GPSData* DataManager::getCurrentGPS() const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return dynamic_cast<GPSData*>(dataStore.getCurrentGPS());
}

IMUData* DataManager::getCurrentIMU() const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return dynamic_cast<IMUData*>(dataStore.getCurrentIMU());
}

SpeedData* DataManager::getCurrentSpeed() const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return dynamic_cast<SpeedData*>(dataStore.getCurrentSpeed());
}

BrakeData* DataManager::getCurrentBrake() const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return dynamic_cast<BrakeData*>(dataStore.getCurrentBrake());
}

ThrottleData* DataManager::getCurrentThrottle() const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return dynamic_cast<ThrottleData*>(dataStore.getCurrentThrottle());
}

SteeringData* DataManager::getCurrentSteering() const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return dynamic_cast<SteeringData*>(dataStore.getCurrentSteering());
}

ImageData* DataManager::getCurrentFrontImage() const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return dynamic_cast<ImageData*>(dataStore.getCurrentFrontImage());
}

ImageData* DataManager::getCurrentBackImage() const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return dynamic_cast<ImageData*>(dataStore.getCurrentBackImage());
}

ImageData* DataManager::getCurrentLeftImage() const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return dynamic_cast<ImageData*>(dataStore.getCurrentLeftImage());
}

ImageData* DataManager::getCurrentRightImage() const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return dynamic_cast<ImageData*>(dataStore.getCurrentRightImage());
}