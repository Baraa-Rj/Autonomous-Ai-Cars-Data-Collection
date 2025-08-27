#include "gui/DataPresenter.h"
#include "readers/ReadersManager.h"
#include "data/DataStore.h"
#include "data/ImageHandler.h"
#include <QString>

DataPresenter::DataPresenter(ReadersManager* rm, DataStore* ds, UIComponents* ui)
    : readersManager(rm), dataStore(ds), uiComponents(ui) {}

void DataPresenter::updateSensorDisplay(std::chrono::system_clock::time_point currentTime) {
    if (!readersManager || !uiComponents) return;
    
    // Update sensor readings with placeholder values for now
    // TODO: Implement actual data retrieval when reader methods are fixed
    uiComponents->lblGpsLat->setText("GPS Lat: -");
    uiComponents->lblGpsLon->setText("GPS Lon: -");
    uiComponents->lblGpsAlt->setText("GPS Alt: -");
    uiComponents->lblSpeed->setText("Speed: -");
    uiComponents->lblBrake->setText("Brake: -");
    uiComponents->lblThrottle->setText("Throttle: -");
    uiComponents->lblSteering->setText("Steering: -");
    uiComponents->lblAccX->setText("Acc X: -");
    uiComponents->lblAccY->setText("Acc Y: -");
    uiComponents->lblAccZ->setText("Acc Z: -");
    uiComponents->lblGyrX->setText("Gyro X: -");
    uiComponents->lblGyrY->setText("Gyro Y: -");
    uiComponents->lblGyrZ->setText("Gyro Z: -");
}

void DataPresenter::updateCameraDisplay(std::chrono::system_clock::time_point currentTime) {
    if (!readersManager || !uiComponents) return;
    
    // TODO: Implement camera image updates when ImageReader methods are fixed
    // Placeholder for now - cameras show static text
}

void DataPresenter::updateTimeDisplay(std::chrono::system_clock::time_point currentTime) {
    if (!uiComponents) return;
    
    auto secs = std::chrono::duration<double>(currentTime.time_since_epoch()).count();
    uiComponents->lblTime->setText(QString("t: %1").arg(QString::number(secs, 'f', 3)));
}

QString DataPresenter::formatValue(double value, int precision) {
    return QString::number(value, 'f', precision);
}