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
    bool loadAllSensorData(const std::string& dataDirectory);
    void cancelLoading();
    void clearData();
    
    GPSData* getCurrentGPS(double timestamp) const;
    IMUData* getCurrentIMU(double timestamp) const;
    SpeedData* getCurrentSpeed(double timestamp) const;
    BrakeData* getCurrentBrake(double timestamp) const;
    ThrottleData* getCurrentThrottle(double timestamp) const;
    SteeringData* getCurrentSteering(double timestamp) const;
    
    ImageData* getCurrentFrontImage(double timestamp) const;
    ImageData* getCurrentBackImage(double timestamp) const;
    ImageData* getCurrentLeftImage(double timestamp) const;
    ImageData* getCurrentRightImage(double timestamp) const;
    
    ClockManager& getClockManager() { return clockManager; }
    const ClockManager& getClockManager() const { return clockManager; }
    
    const SensorDataStore& getDataStore() const { return dataStore; }

signals:
    void dataLoadingProgress(int percentage);
    void dataLoaded();
    void dataLoadingError(const QString& error);

private:
    SensorDataStore dataStore;
    ClockManager clockManager;
    
    mutable std::shared_mutex dataStoreMutex;
    mutable std::mutex clockManagerMutex;
    std::atomic<bool> isLoading{false};
    std::atomic<bool> cancelLoadingFlag{false};
    
    bool loadCSVData(const std::string& filePath, SensorType type);
    bool loadImageData(const std::string& dirPath, const std::string& cameraName);
    
    void loadCSVDataAsync(const std::string& filePath, SensorType type, std::atomic<int>& completedTasks, int totalTasks);
    void loadImageDataAsync(const std::string& dirPath, const std::string& cameraName, std::atomic<int>& completedTasks, int totalTasks);
};