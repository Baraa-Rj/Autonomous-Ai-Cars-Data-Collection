#include "management/DataManager.h"
#include "readers/ImageDataReader.h"
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <QApplication>

DataManager::DataManager(QObject* parent) 
    : QObject(parent) {
}

bool DataManager::loadAllSensorData(const std::string& dataDirectory) {
    if (!std::filesystem::exists(dataDirectory)) {
        emit dataLoadingError("Data directory does not exist: " + QString::fromStdString(dataDirectory));
        return false;
    }
    
    clearData();
    
    try {
        // Load CSV files
        emit dataLoadingProgress(10);
        if (!loadCSVData(dataDirectory + "/gps.csv", SensorType::GPS)) return false;
        
        emit dataLoadingProgress(20);
        if (!loadCSVData(dataDirectory + "/imu.csv", SensorType::IMU)) return false;
        
        emit dataLoadingProgress(30);
        if (!loadCSVData(dataDirectory + "/speed.csv", SensorType::SPEED)) return false;
        
        emit dataLoadingProgress(40);
        if (!loadCSVData(dataDirectory + "/brake.csv", SensorType::BRAKE)) return false;
        
        emit dataLoadingProgress(50);
        if (!loadCSVData(dataDirectory + "/throttle.csv", SensorType::THROTTLE)) return false;
        
        emit dataLoadingProgress(60);
        if (!loadCSVData(dataDirectory + "/steering.csv", SensorType::STEERING)) return false;
        
        // Load image data
        emit dataLoadingProgress(70);
        if (!loadImageData(dataDirectory + "/3d_images/front", "front")) return false;
        
        emit dataLoadingProgress(80);
        if (!loadImageData(dataDirectory + "/3d_images/back", "back")) return false;
        
        emit dataLoadingProgress(85);
        if (!loadImageData(dataDirectory + "/3d_images/left", "left")) return false;
        
        emit dataLoadingProgress(95);
        if (!loadImageData(dataDirectory + "/3d_images/right", "right")) return false;
        
        emit dataLoadingProgress(100);
        emit dataLoaded();
        
        std::cout << "Data loading completed. Time range: " << clockManager.getMinTimestamp() << " to " << clockManager.getMaxTimestamp() << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        emit dataLoadingError("Error loading data: " + QString::fromStdString(e.what()));
        return false;
    }
}

bool DataManager::loadCSVData(const std::string& filePath, SensorType type) {
    if (!std::filesystem::exists(filePath)) {
        std::cout << "Warning: File not found: " << filePath << std::endl;
        return true; // Continue loading other files
    }
    
    auto reader = DataReaderFactory::createReader(type);
    if (!reader) {
        emit dataLoadingError("Failed to create reader for sensor type");
        return false;
    }
    
    try {
        auto rawData = reader->readCSV(filePath);
        
        // Convert and store data based on type, updating time range as we go
        switch (type) {
            case SensorType::GPS:
                for (auto& data : rawData) {
                    auto gpsData = dynamic_cast<GPSData*>(data.get());
                    if (gpsData) {
                        clockManager.updateRange(gpsData->timestamp);
                        dataStore.gps_data.emplace_back(*gpsData);
                    }
                }
                break;
            case SensorType::IMU:
                for (auto& data : rawData) {
                    auto imuData = dynamic_cast<IMUData*>(data.get());
                    if (imuData) {
                        clockManager.updateRange(imuData->timestamp);
                        dataStore.imu_data.emplace_back(*imuData);
                    }
                }
                break;
            case SensorType::SPEED:
                for (auto& data : rawData) {
                    auto speedData = dynamic_cast<SpeedData*>(data.get());
                    if (speedData) {
                        clockManager.updateRange(speedData->timestamp);
                        dataStore.speed_data.emplace_back(*speedData);
                    }
                }
                break;
            case SensorType::BRAKE:
                for (auto& data : rawData) {
                    auto brakeData = dynamic_cast<BrakeData*>(data.get());
                    if (brakeData) {
                        clockManager.updateRange(brakeData->timestamp);
                        dataStore.brake_data.emplace_back(*brakeData);
                    }
                }
                break;
            case SensorType::THROTTLE:
                for (auto& data : rawData) {
                    auto throttleData = dynamic_cast<ThrottleData*>(data.get());
                    if (throttleData) {
                        clockManager.updateRange(throttleData->timestamp);
                        dataStore.throttle_data.emplace_back(*throttleData);
                    }
                }
                break;
            case SensorType::STEERING:
                for (auto& data : rawData) {
                    auto steeringData = dynamic_cast<SteeringData*>(data.get());
                    if (steeringData) {
                        clockManager.updateRange(steeringData->timestamp);
                        dataStore.steering_data.emplace_back(*steeringData);
                    }
                }
                break;
            default:
                break;
        }
        
        QApplication::processEvents(); // Keep UI responsive
        return true;
        
    } catch (const std::exception& e) {
        emit dataLoadingError("Error loading " + QString::fromStdString(filePath) + ": " + QString::fromStdString(e.what()));
        return false;
    }
}

bool DataManager::loadImageData(const std::string& dirPath, const std::string& cameraName) {
    if (!std::filesystem::exists(dirPath)) {
        std::cout << "Warning: Image directory not found: " << dirPath << std::endl;
        return true; // Continue loading other directories
    }
    
    auto imageReader = std::make_unique<ImageDataReader>();
    
    try {
        auto rawData = imageReader->readFromDirectory(dirPath);
        std::cout << "Loading " << rawData.size() << " images from " << cameraName << " camera" << std::endl;
        
        // Store image metadata only (lazy loading - images loaded on demand)
        if (cameraName == "front") {
            for (auto& data : rawData) {
                auto imageData = dynamic_cast<ImageData*>(data.get());
                if (imageData) {
                    clockManager.updateRange(imageData->timestamp);
                    dataStore.front_images.emplace_back(*imageData);
                }
            }
        } else if (cameraName == "back") {
            for (auto& data : rawData) {
                auto imageData = dynamic_cast<ImageData*>(data.get());
                if (imageData) {
                    clockManager.updateRange(imageData->timestamp);
                    dataStore.back_images.emplace_back(*imageData);
                }
            }
        } else if (cameraName == "left") {
            for (auto& data : rawData) {
                auto imageData = dynamic_cast<ImageData*>(data.get());
                if (imageData) {
                    clockManager.updateRange(imageData->timestamp);
                    dataStore.left_images.emplace_back(*imageData);
                }
            }
        } else if (cameraName == "right") {
            for (auto& data : rawData) {
                auto imageData = dynamic_cast<ImageData*>(data.get());
                if (imageData) {
                    clockManager.updateRange(imageData->timestamp);
                    dataStore.right_images.emplace_back(*imageData);
                }
            }
        }
        
        QApplication::processEvents(); // Keep UI responsive
        return true;
        
    } catch (const std::exception& e) {
        emit dataLoadingError("Error loading images from " + QString::fromStdString(dirPath) + ": " + QString::fromStdString(e.what()));
        return false;
    }
}

void DataManager::clearData() {
    dataStore.clear();
    clockManager.resetRange();
}


// Getter implementations - now using ClockManager for data searching

GPSData* DataManager::getCurrentGPS(double timestamp) const {
    return clockManager.findClosestData(dataStore.gps_data, timestamp);
}

IMUData* DataManager::getCurrentIMU(double timestamp) const {
    return clockManager.findClosestData(dataStore.imu_data, timestamp);
}

SpeedData* DataManager::getCurrentSpeed(double timestamp) const {
    return clockManager.findClosestData(dataStore.speed_data, timestamp);
}

BrakeData* DataManager::getCurrentBrake(double timestamp) const {
    return clockManager.findClosestData(dataStore.brake_data, timestamp);
}

ThrottleData* DataManager::getCurrentThrottle(double timestamp) const {
    return clockManager.findClosestData(dataStore.throttle_data, timestamp);
}

SteeringData* DataManager::getCurrentSteering(double timestamp) const {
    return clockManager.findClosestData(dataStore.steering_data, timestamp);
}

ImageData* DataManager::getCurrentFrontImage(double timestamp) const {
    return clockManager.findClosestData(dataStore.front_images, timestamp);
}

ImageData* DataManager::getCurrentBackImage(double timestamp) const {
    return clockManager.findClosestData(dataStore.back_images, timestamp);
}

ImageData* DataManager::getCurrentLeftImage(double timestamp) const {
    return clockManager.findClosestData(dataStore.left_images, timestamp);
}

ImageData* DataManager::getCurrentRightImage(double timestamp) const {
    return clockManager.findClosestData(dataStore.right_images, timestamp);
}


// MOC file will be generated automatically