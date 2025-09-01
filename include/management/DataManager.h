#pragma once
#include "management/SensorDataStore.h"
#include "management/ClockManager.h"
#include "readers/DataReaderFactory.h"
#include <string>
#include <memory>
#include <mutex>
#include <future>
#include <thread>
#include <atomic>
#include <QObject>
#include <shared_mutex>
#include <condition_variable>
#include <vector>
#include <map>
#include "data/GPSData.h"
#include "data/IMUData.h"
#include "data/SpeedData.h"
#include "data/BrakeData.h"
#include "data/ThrottleData.h"
#include "data/SteeringData.h"
#include "data/ImageData.h"

class DataManager : public QObject {
    Q_OBJECT

public:
    explicit DataManager(QObject* parent = nullptr);
    
    void loadAllSensorDataAsync(const std::string& dataDirectory);
    void cancelLoading();
    void clearData();
    
    // Get current sensor data (no timestamp needed as we only keep last frame)
    GPSData* getCurrentGPS() const;
    IMUData* getCurrentIMU() const;
    SpeedData* getCurrentSpeed() const;
    BrakeData* getCurrentBrake() const;
    ThrottleData* getCurrentThrottle() const;
    SteeringData* getCurrentSteering() const;
    
    ImageData* getCurrentFrontImage() const;
    ImageData* getCurrentBackImage() const;
    ImageData* getCurrentLeftImage() const;
    ImageData* getCurrentRightImage() const;
    
    // Update sensor data by reading next values from streams
    void updateSensorData(double targetTimestamp);
    
    // Initialize streaming readers
    bool initializeStreamingReaders(const std::string& dataDirectory);
    
    ClockManager& getClockManager() { return clockManager; }
    const ClockManager& getClockManager() const { return clockManager; }
    
    const SensorDataStore& getDataStore() const { return dataStore; }
    
    bool isLoading() const { return loadingFlag.load(); }

signals:
    void dataLoadingProgress(int percentage);
    void dataLoaded();
    void dataLoadingError(const QString& error);

private:
    SensorDataStore dataStore;
    ClockManager clockManager;
    
    mutable std::shared_mutex dataStoreMutex;
    mutable std::mutex clockManagerMutex;
    std::atomic<bool> loadingFlag{false};
    std::atomic<bool> cancelLoadingFlag{false};
    
    // Preloaded data readers for each sensor type
    std::map<DataType, std::unique_ptr<DataReader>> sensorReaders;
    std::map<std::string, std::unique_ptr<DataReader>> imageReaders;
    
    // Pre-loaded image file lists for each camera
    std::map<std::string, std::vector<std::pair<double, std::string>>> imageFilesByCamera;
    std::map<std::string, size_t> currentImageIndices;
    
    // Image streaming support
    void updateImageDataForTimestamp(double targetTimestamp);
    void preloadImageFileList(const std::string& cameraName, const std::string& dirPath);
    
    // Simplified helper functions
    Data* findClosestData(DataReader* reader, double targetTimestamp);
    std::unique_ptr<Data> cloneData(Data* data, DataType type);
};