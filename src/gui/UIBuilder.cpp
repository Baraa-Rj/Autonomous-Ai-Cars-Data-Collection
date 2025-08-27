#include "gui/UIBuilder.h"
#include <Qt>

UIComponents UIBuilder::buildUI(QWidget* parent) {
    UIComponents components;
    
    auto* mainLayout = new QHBoxLayout(parent);
    
    // Create sensor panel and camera panel
    auto* sensorPanel = createSensorPanel(parent, components);
    auto* cameraPanel = createCameraPanel(parent, components);
    
    mainLayout->addWidget(sensorPanel, 0);
    mainLayout->addWidget(cameraPanel, 1);
    parent->setLayout(mainLayout);
    
    return components;
}

QWidget* UIBuilder::createSensorPanel(QWidget* parent, UIComponents& components) {
    auto* sensorWidget = new QWidget(parent);
    auto* sensorLayout = new QVBoxLayout(sensorWidget);
    
    initializeSensorLabels(sensorWidget, components);
    
    // Add labels to layout
    sensorLayout->addWidget(components.lblGpsLat);
    sensorLayout->addWidget(components.lblGpsLon);
    sensorLayout->addWidget(components.lblGpsAlt);
    sensorLayout->addWidget(components.lblSpeed);
    sensorLayout->addWidget(components.lblBrake);
    sensorLayout->addWidget(components.lblThrottle);
    sensorLayout->addWidget(components.lblSteering);
    sensorLayout->addWidget(components.lblAccX);
    sensorLayout->addWidget(components.lblAccY);
    sensorLayout->addWidget(components.lblAccZ);
    sensorLayout->addWidget(components.lblGyrX);
    sensorLayout->addWidget(components.lblGyrY);
    sensorLayout->addWidget(components.lblGyrZ);
    sensorLayout->addWidget(components.lblTime);
    sensorLayout->addStretch();
    
    return sensorWidget;
}

QWidget* UIBuilder::createCameraPanel(QWidget* parent, UIComponents& components) {
    auto* cameraWidget = new QWidget(parent);
    auto* cameraLayout = new QVBoxLayout(cameraWidget);
    
    initializeCameraLabels(cameraWidget, components);
    
    auto* grid = new QGridLayout();
    grid->addWidget(components.lblCamLeft, 0, 0);
    grid->addWidget(components.lblCamFront, 0, 1);
    grid->addWidget(components.lblCamRight, 1, 0);
    grid->addWidget(components.lblCamBack, 1, 1);
    
    cameraLayout->addLayout(grid);
    
    return cameraWidget;
}

void UIBuilder::initializeSensorLabels(QWidget* parent, UIComponents& components) {
    components.lblGpsLat = new QLabel("GPS Lat: -", parent);
    components.lblGpsLon = new QLabel("GPS Lon: -", parent);
    components.lblGpsAlt = new QLabel("GPS Alt: -", parent);
    components.lblSpeed = new QLabel("Speed: -", parent);
    components.lblBrake = new QLabel("Brake: -", parent);
    components.lblThrottle = new QLabel("Throttle: -", parent);
    components.lblSteering = new QLabel("Steering: -", parent);
    components.lblAccX = new QLabel("Acc X: -", parent);
    components.lblAccY = new QLabel("Acc Y: -", parent);
    components.lblAccZ = new QLabel("Acc Z: -", parent);
    components.lblGyrX = new QLabel("Gyro X: -", parent);
    components.lblGyrY = new QLabel("Gyro Y: -", parent);
    components.lblGyrZ = new QLabel("Gyro Z: -", parent);
    components.lblTime = new QLabel("t: -", parent);
}

void UIBuilder::initializeCameraLabels(QWidget* parent, UIComponents& components) {
    components.lblCamLeft = new QLabel("Left", parent);
    components.lblCamFront = new QLabel("Front", parent);
    components.lblCamRight = new QLabel("Right", parent);
    components.lblCamBack = new QLabel("Back", parent);
    
    components.lblCamLeft->setAlignment(Qt::AlignCenter);
    components.lblCamFront->setAlignment(Qt::AlignCenter);
    components.lblCamRight->setAlignment(Qt::AlignCenter);
    components.lblCamBack->setAlignment(Qt::AlignCenter);
    
    components.lblCamLeft->setMinimumSize(320, 180);
    components.lblCamFront->setMinimumSize(320, 180);
    components.lblCamRight->setMinimumSize(320, 180);
    components.lblCamBack->setMinimumSize(320, 180);
}