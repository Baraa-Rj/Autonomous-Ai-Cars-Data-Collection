#include "controllers/DisplayController.h"
#include "management/DataManager.h"
#include "utils/ImageProcessor.h"

DisplayController::DisplayController(DataManager* dataManager, QObject* parent)
    : QObject(parent)
    , dataManager(dataManager)
    , gpsLabel(nullptr)
    , imuLabel(nullptr)
    , speedLabel(nullptr)
    , brakeLabel(nullptr)
    , throttleLabel(nullptr)
    , steeringLabel(nullptr)
    , frontImageLabel(nullptr)
    , backImageLabel(nullptr)
    , leftImageLabel(nullptr)
    , rightImageLabel(nullptr)
{
}

void DisplayController::setSensorLabels(QLabel* gps, QLabel* imu, QLabel* speed,
                                       QLabel* brake, QLabel* throttle, QLabel* steering) {
    gpsLabel = gps;
    imuLabel = imu;
    speedLabel = speed;
    brakeLabel = brake;
    throttleLabel = throttle;
    steeringLabel = steering;
}

void DisplayController::setImageLabels(QLabel* front, QLabel* back, QLabel* left, QLabel* right) {
    frontImageLabel = front;
    backImageLabel = back;
    leftImageLabel = left;
    rightImageLabel = right;
}

void DisplayController::updateDisplays(double timestamp) {
    updateSensorDisplays(timestamp);
    updateImageDisplays(timestamp);
}

void DisplayController::updateSensorDisplays(double timestamp) {
    if (!dataManager) return;
    
    // Update sensor data for current timestamp
    dataManager->updateSensorData(timestamp);
    
    // Update GPS display
    auto gps = dataManager->getCurrentGPS();
    if (gps && gpsLabel) {
        gpsLabel->setText(QString::fromStdString(gps->toString()));
    }
    
    // Update IMU display
    auto imu = dataManager->getCurrentIMU();
    if (imu && imuLabel) {
        imuLabel->setText(QString::fromStdString(imu->toString()));
    }
    
    // Update speed display
    auto speed = dataManager->getCurrentSpeed();
    if (speed && speedLabel) {
        speedLabel->setText(QString::fromStdString(speed->toString()));
    }
    
    // Update brake display
    auto brake = dataManager->getCurrentBrake();
    if (brake && brakeLabel) {
        brakeLabel->setText(QString::fromStdString(brake->toString()));
    }
    
    // Update throttle display
    auto throttle = dataManager->getCurrentThrottle();
    if (throttle && throttleLabel) {
        throttleLabel->setText(QString::fromStdString(throttle->toString()));
    }
    
    // Update steering display
    auto steering = dataManager->getCurrentSteering();
    if (steering && steeringLabel) {
        steeringLabel->setText(QString::fromStdString(steering->toString()));
    }
}

void DisplayController::updateImageDisplays(double timestamp) {
    if (!dataManager) return;
    
    // Update image displays using ImageProcessor utility
    ImageProcessor::displayImage(dataManager->getCurrentFrontImage(), frontImageLabel);
    ImageProcessor::displayImage(dataManager->getCurrentBackImage(), backImageLabel);
    ImageProcessor::displayImage(dataManager->getCurrentLeftImage(), leftImageLabel);
    ImageProcessor::displayImage(dataManager->getCurrentRightImage(), rightImageLabel);
}