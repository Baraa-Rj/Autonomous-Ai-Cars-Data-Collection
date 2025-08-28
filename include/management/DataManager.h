#pragma once
#include "management/SensorDataStore.h"
#include "management/ClockManager.h"
#include "readers/DataReaderFactory.h"
#include <string>
#include <memory>
#include <QObject>

class DataManager : public QObject {
    Q_OBJECT

public:
    explicit DataManager(QObject* parent = nullptr);
    
    bool loadAllSensorData(const std::string& dataDirectory);
    void clearData();
    
    // Getters for current data at specific timestamp
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
    
    // Access to clock manager for time operations
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
    
    bool loadCSVData(const std::string& filePath, SensorType type);
    bool loadImageData(const std::string& dirPath, const std::string& cameraName);
};