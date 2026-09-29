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
    
    void updateSensorData(double targetTimestamp);
    
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
    
    std::map<DataType, std::unique_ptr<DataReader>> sensorReaders;
    std::map<std::string, std::unique_ptr<DataReader>> imageReaders;
    
    std::map<std::string, std::vector<std::pair<double, std::string>>> imageFilesByCamera;
    
    void updateImageDataForTimestamp(double targetTimestamp);
    void preloadImageFileList(const std::string& cameraName, const std::string& dirPath);
    
    Data* findClosestData(DataReader* reader, double targetTimestamp);
    std::unique_ptr<Data> cloneData(Data* data, DataType type);
};