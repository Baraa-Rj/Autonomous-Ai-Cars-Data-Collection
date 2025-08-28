#include "management/DataManager.h"
#include "readers/ImageDataReader.h"
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <future>
#include <thread>
#include <QApplication>
#include <QRunnable>
#include <QThreadPool>

// Custom QRunnable for data loading tasks
class DataLoadingTask : public QRunnable {
public:
    DataLoadingTask(DataManager* manager, const std::string& path, SensorType type, 
                   std::atomic<int>& completed, int total)
        : dataManager(manager), filePath(path), sensorType(type), 
          completedTasks(completed), totalTasks(total) {}
    
    void run() override {
        dataManager->loadCSVDataAsync(filePath, sensorType, completedTasks, totalTasks);
    }
    
private:
    DataManager* dataManager;
    std::string filePath;
    SensorType sensorType;
    std::atomic<int>& completedTasks;
    int totalTasks;
};

class ImageLoadingTask : public QRunnable {
public:
    ImageLoadingTask(DataManager* manager, const std::string& path, const std::string& camera,
                    std::atomic<int>& completed, int total)
        : dataManager(manager), dirPath(path), cameraName(camera),
          completedTasks(completed), totalTasks(total) {}
    
    void run() override {
        dataManager->loadImageDataAsync(dirPath, cameraName, completedTasks, totalTasks);
    }
    
private:
    DataManager* dataManager;
    std::string dirPath;
    std::string cameraName;
    std::atomic<int>& completedTasks;
    int totalTasks;
};

DataManager::DataManager(QObject* parent) 
    : QObject(parent), threadPool(QThreadPool::globalInstance()) {
    threadPool->setMaxThreadCount(std::thread::hardware_concurrency());
}

void DataManager::loadAllSensorDataAsync(const std::string& dataDirectory) {
    if (!std::filesystem::exists(dataDirectory)) {
        emit dataLoadingError("Data directory does not exist: " + QString::fromStdString(dataDirectory));
        return;
    }
    
    if (isLoading.load()) {
        emit dataLoadingError("Loading is already in progress");
        return;
    }
    
    isLoading.store(true);
    cancelLoadingFlag.store(false);
    clearData();
    
    // Use a separate thread to coordinate the loading process
    std::thread([this, dataDirectory]() {
        try {
            // Total tasks: 6 CSV files + 4 image directories
            const int totalTasks = 10;
            std::atomic<int> completedTasks{0};
            
            // Submit CSV loading tasks
            std::vector<std::string> csvFiles = {
                dataDirectory + "/gps.csv",
                dataDirectory + "/imu.csv", 
                dataDirectory + "/speed.csv",
                dataDirectory + "/brake.csv",
                dataDirectory + "/throttle.csv",
                dataDirectory + "/steering.csv"
            };
            
            std::vector<SensorType> sensorTypes = {
                SensorType::GPS, SensorType::IMU, SensorType::SPEED,
                SensorType::BRAKE, SensorType::THROTTLE, SensorType::STEERING
            };
            
            // Submit CSV loading tasks to thread pool
            for (size_t i = 0; i < csvFiles.size(); ++i) {
                if (cancelLoadingFlag.load()) break;
                threadPool->start(new DataLoadingTask(this, csvFiles[i], sensorTypes[i], 
                                                    completedTasks, totalTasks));
            }
            
            // Submit image loading tasks
            std::vector<std::pair<std::string, std::string>> imageDirs = {
                {dataDirectory + "/3d_images/front", "front"},
                {dataDirectory + "/3d_images/back", "back"},
                {dataDirectory + "/3d_images/left", "left"},
                {dataDirectory + "/3d_images/right", "right"}
            };
            
            for (const auto& [dirPath, cameraName] : imageDirs) {
                if (cancelLoadingFlag.load()) break;
                threadPool->start(new ImageLoadingTask(this, dirPath, cameraName,
                                                     completedTasks, totalTasks));
            }
            
            // Monitor progress
            int lastProgress = 0;
            while (completedTasks.load() < totalTasks && !cancelLoadingFlag.load()) {
                int currentProgress = (completedTasks.load() * 100) / totalTasks;
                if (currentProgress != lastProgress) {
                    emit dataLoadingProgress(currentProgress);
                    lastProgress = currentProgress;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            
            // Wait for all tasks to complete
            threadPool->waitForDone();
            
            if (!cancelLoadingFlag.load()) {
                emit dataLoadingProgress(100);
                emit dataLoaded();
                std::cout << "Multithreaded data loading completed. Time range: " 
                         << clockManager.getMinTimestamp() << " to " 
                         << clockManager.getMaxTimestamp() << std::endl;
            }
            
        } catch (const std::exception& e) {
            emit dataLoadingError("Error loading data: " + QString::fromStdString(e.what()));
        }
        
        isLoading.store(false);
    }).detach();
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
        
        QApplication::processEvents(); 
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

void DataManager::cancelLoading() {
    cancelLoadingFlag.store(true);
}

void DataManager::clearData() {
    std::unique_lock<std::shared_mutex> lock(dataStoreMutex);
    dataStore.clear();
    
    std::lock_guard<std::mutex> clockLock(clockManagerMutex);
    clockManager.resetRange();
}


// Getter implementations - now using ClockManager for data searching

GPSData* DataManager::getCurrentGPS(double timestamp) const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return clockManager.findClosestData(dataStore.gps_data, timestamp);
}

IMUData* DataManager::getCurrentIMU(double timestamp) const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return clockManager.findClosestData(dataStore.imu_data, timestamp);
}

SpeedData* DataManager::getCurrentSpeed(double timestamp) const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return clockManager.findClosestData(dataStore.speed_data, timestamp);
}

BrakeData* DataManager::getCurrentBrake(double timestamp) const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return clockManager.findClosestData(dataStore.brake_data, timestamp);
}

ThrottleData* DataManager::getCurrentThrottle(double timestamp) const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return clockManager.findClosestData(dataStore.throttle_data, timestamp);
}

SteeringData* DataManager::getCurrentSteering(double timestamp) const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return clockManager.findClosestData(dataStore.steering_data, timestamp);
}

ImageData* DataManager::getCurrentFrontImage(double timestamp) const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return clockManager.findClosestData(dataStore.front_images, timestamp);
}

ImageData* DataManager::getCurrentBackImage(double timestamp) const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return clockManager.findClosestData(dataStore.back_images, timestamp);
}

ImageData* DataManager::getCurrentLeftImage(double timestamp) const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return clockManager.findClosestData(dataStore.left_images, timestamp);
}

ImageData* DataManager::getCurrentRightImage(double timestamp) const {
    std::shared_lock<std::shared_mutex> lock(dataStoreMutex);
    return clockManager.findClosestData(dataStore.right_images, timestamp);
}


void DataManager::loadCSVDataAsync(const std::string& filePath, SensorType type, 
                                   std::atomic<int>& completedTasks, int totalTasks) {
    if (cancelLoadingFlag.load()) {
        completedTasks.fetch_add(1);
        return;
    }
    
    if (!std::filesystem::exists(filePath)) {
        std::cout << "Warning: File not found: " << filePath << std::endl;
        completedTasks.fetch_add(1);
        return;
    }
    
    try {
        auto reader = DataReaderFactory::createReader(type);
        if (!reader) {
            std::cerr << "Failed to create reader for sensor type" << std::endl;
            completedTasks.fetch_add(1);
            return;
        }
        
        auto rawData = reader->readCSV(filePath);
        
        // Thread-safe data storage
        {
            std::unique_lock<std::shared_mutex> lock(dataStoreMutex);
            
            switch (type) {
                case SensorType::GPS:
                    for (auto& data : rawData) {
                        if (auto gpsData = dynamic_cast<GPSData*>(data.get())) {
                            {
                                std::lock_guard<std::mutex> clockLock(clockManagerMutex);
                                clockManager.updateRange(gpsData->timestamp);
                            }
                            dataStore.gps_data.emplace_back(*gpsData);
                        }
                    }
                    break;
                case SensorType::IMU:
                    for (auto& data : rawData) {
                        if (auto imuData = dynamic_cast<IMUData*>(data.get())) {
                            {
                                std::lock_guard<std::mutex> clockLock(clockManagerMutex);
                                clockManager.updateRange(imuData->timestamp);
                            }
                            dataStore.imu_data.emplace_back(*imuData);
                        }
                    }
                    break;
                case SensorType::SPEED:
                    for (auto& data : rawData) {
                        if (auto speedData = dynamic_cast<SpeedData*>(data.get())) {
                            {
                                std::lock_guard<std::mutex> clockLock(clockManagerMutex);
                                clockManager.updateRange(speedData->timestamp);
                            }
                            dataStore.speed_data.emplace_back(*speedData);
                        }
                    }
                    break;
                case SensorType::BRAKE:
                    for (auto& data : rawData) {
                        if (auto brakeData = dynamic_cast<BrakeData*>(data.get())) {
                            {
                                std::lock_guard<std::mutex> clockLock(clockManagerMutex);
                                clockManager.updateRange(brakeData->timestamp);
                            }
                            dataStore.brake_data.emplace_back(*brakeData);
                        }
                    }
                    break;
                case SensorType::THROTTLE:
                    for (auto& data : rawData) {
                        if (auto throttleData = dynamic_cast<ThrottleData*>(data.get())) {
                            {
                                std::lock_guard<std::mutex> clockLock(clockManagerMutex);
                                clockManager.updateRange(throttleData->timestamp);
                            }
                            dataStore.throttle_data.emplace_back(*throttleData);
                        }
                    }
                    break;
                case SensorType::STEERING:
                    for (auto& data : rawData) {
                        if (auto steeringData = dynamic_cast<SteeringData*>(data.get())) {
                            {
                                std::lock_guard<std::mutex> clockLock(clockManagerMutex);
                                clockManager.updateRange(steeringData->timestamp);
                            }
                            dataStore.steering_data.emplace_back(*steeringData);
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error loading " << filePath << ": " << e.what() << std::endl;
    }
    
    completedTasks.fetch_add(1);
}

void DataManager::loadImageDataAsync(const std::string& dirPath, const std::string& cameraName,
                                    std::atomic<int>& completedTasks, int totalTasks) {
    if (cancelLoadingFlag.load()) {
        completedTasks.fetch_add(1);
        return;
    }
    
    if (!std::filesystem::exists(dirPath)) {
        std::cout << "Warning: Image directory not found: " << dirPath << std::endl;
        completedTasks.fetch_add(1);
        return;
    }
    
    try {
        auto imageReader = std::make_unique<ImageDataReader>();
        auto rawData = imageReader->readFromDirectory(dirPath);
        
        std::cout << "Loading " << rawData.size() << " images from " << cameraName 
                  << " camera in parallel" << std::endl;
        
        // Thread-safe image data storage
        {
            std::unique_lock<std::shared_mutex> lock(dataStoreMutex);
            
            if (cameraName == "front") {
                for (auto& data : rawData) {
                    if (auto imageData = dynamic_cast<ImageData*>(data.get())) {
                        {
                            std::lock_guard<std::mutex> clockLock(clockManagerMutex);
                            clockManager.updateRange(imageData->timestamp);
                        }
                        dataStore.front_images.emplace_back(*imageData);
                    }
                }
            } else if (cameraName == "back") {
                for (auto& data : rawData) {
                    if (auto imageData = dynamic_cast<ImageData*>(data.get())) {
                        {
                            std::lock_guard<std::mutex> clockLock(clockManagerMutex);
                            clockManager.updateRange(imageData->timestamp);
                        }
                        dataStore.back_images.emplace_back(*imageData);
                    }
                }
            } else if (cameraName == "left") {
                for (auto& data : rawData) {
                    if (auto imageData = dynamic_cast<ImageData*>(data.get())) {
                        {
                            std::lock_guard<std::mutex> clockLock(clockManagerMutex);
                            clockManager.updateRange(imageData->timestamp);
                        }
                        dataStore.left_images.emplace_back(*imageData);
                    }
                }
            } else if (cameraName == "right") {
                for (auto& data : rawData) {
                    if (auto imageData = dynamic_cast<ImageData*>(data.get())) {
                        {
                            std::lock_guard<std::mutex> clockLock(clockManagerMutex);
                            clockManager.updateRange(imageData->timestamp);
                        }
                        dataStore.right_images.emplace_back(*imageData);
                    }
                }
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error loading images from " << dirPath << ": " << e.what() << std::endl;
    }
    
    completedTasks.fetch_add(1);
}

