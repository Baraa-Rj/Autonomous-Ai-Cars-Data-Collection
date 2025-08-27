#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTimer>
#include <chrono>
#include "data/DataStore.h"
#include "data/Data.h"

class ReadersManager;
class GpsReader;
class SpeedReader;
class BrakeReader;
class ThrottleReader;
class SteeringReader;
class IMUReader;

class DisplayManager : public QWidget {
    Q_OBJECT
    
private:
    DataStore* dataStore;
    ReadersManager* readersManager;
    std::chrono::system_clock::time_point currentTime;
    std::chrono::system_clock::time_point globalStart;
    std::chrono::system_clock::time_point globalEnd;
    int fps{30};
    
    // UI Elements
    QLabel* lblGpsLat;
    QLabel* lblGpsLon;
    QLabel* lblGpsAlt;
    QLabel* lblSpeed;
    QLabel* lblBrake;
    QLabel* lblThrottle;
    QLabel* lblSteering;
    QLabel* lblAccX;
    QLabel* lblAccY;
    QLabel* lblAccZ;
    QLabel* lblGyrX;
    QLabel* lblGyrY;
    QLabel* lblGyrZ;
    QLabel* lblTime;
    QLabel* lblCamLeft;
    QLabel* lblCamFront;
    QLabel* lblCamRight;
    QLabel* lblCamBack;
    
    // Reader pointers for optimization
    GpsReader* gpsReader;
    SpeedReader* speedReader;
    BrakeReader* brakeReader;
    ThrottleReader* throttleReader;
    SteeringReader* steeringReader;
    IMUReader* imuReader;
    
public:
    explicit DisplayManager(QWidget* parent = nullptr);
    ~DisplayManager() override = default;
    
    void displayFrame(std::list<std::shared_ptr<Data>> dataItems);
    bool renderData(std::list<std::shared_ptr<Data>> dataItems);
    
    void setDataStore(DataStore* dataStore);
    DataStore* getDataStore();
    
    void setFps(int fps);
    int getFps() const;
    
    void setReadersManager(ReadersManager* rm);
    void initializeTimeline();
    
private:
    void buildUi();
    void connectUi();
    void updateCameras(std::chrono::system_clock::time_point t);
    void updateSidebar(std::chrono::system_clock::time_point t);
    void computeGlobalTimeline();
    
public slots:
    void updateDisplay(std::chrono::system_clock::time_point time);
};


