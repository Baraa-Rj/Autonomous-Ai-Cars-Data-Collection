#pragma once

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>

struct UIComponents {
    // Sensor data labels
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
    
    // Camera labels
    QLabel* lblCamLeft;
    QLabel* lblCamFront;
    QLabel* lblCamRight;
    QLabel* lblCamBack;
};

class UIBuilder {
public:
    static UIComponents buildUI(QWidget* parent);
    
private:
    static QWidget* createSensorPanel(QWidget* parent, UIComponents& components);
    static QWidget* createCameraPanel(QWidget* parent, UIComponents& components);
    static void initializeSensorLabels(QWidget* parent, UIComponents& components);
    static void initializeCameraLabels(QWidget* parent, UIComponents& components);
};