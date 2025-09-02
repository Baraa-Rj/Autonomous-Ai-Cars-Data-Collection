#pragma once
#include <QObject>
#include <QtWidgets/QLabel>

class DataManager;

class DisplayController : public QObject {
    Q_OBJECT

public:
    explicit DisplayController(DataManager* dataManager, QObject* parent = nullptr);
    
    void setSensorLabels(QLabel* gps, QLabel* imu, QLabel* speed, 
                        QLabel* brake, QLabel* throttle, QLabel* steering);
    
    void setImageLabels(QLabel* front, QLabel* back, QLabel* left, QLabel* right);

public slots:
    void updateDisplays(double timestamp);

private:
    void updateSensorDisplays(double timestamp);
    void updateImageDisplays(double timestamp);

private:
    DataManager* dataManager;
    
    QLabel* gpsLabel;
    QLabel* imuLabel;
    QLabel* speedLabel;
    QLabel* brakeLabel;
    QLabel* throttleLabel;
    QLabel* steeringLabel;
    
    QLabel* frontImageLabel;
    QLabel* backImageLabel;
    QLabel* leftImageLabel;
    QLabel* rightImageLabel;
};