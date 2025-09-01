#pragma once
#include <QObject>
#include <QtWidgets/QLabel>

class DataManager;

// Single Responsibility: Handle display updates for sensor data and images
class DisplayController : public QObject {
    Q_OBJECT

public:
    explicit DisplayController(DataManager* dataManager, QObject* parent = nullptr);
    
    // Set UI labels for sensor data display
    void setSensorLabels(QLabel* gps, QLabel* imu, QLabel* speed, 
                        QLabel* brake, QLabel* throttle, QLabel* steering);
    
    // Set UI labels for image display  
    void setImageLabels(QLabel* front, QLabel* back, QLabel* left, QLabel* right);

public slots:
    void updateDisplays(double timestamp);

private:
    void updateSensorDisplays(double timestamp);
    void updateImageDisplays(double timestamp);

private:
    DataManager* dataManager;
    
    // Sensor display labels
    QLabel* gpsLabel;
    QLabel* imuLabel;
    QLabel* speedLabel;
    QLabel* brakeLabel;
    QLabel* throttleLabel;
    QLabel* steeringLabel;
    
    // Image display labels
    QLabel* frontImageLabel;
    QLabel* backImageLabel;
    QLabel* leftImageLabel;
    QLabel* rightImageLabel;
};