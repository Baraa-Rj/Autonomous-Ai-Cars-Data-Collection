#pragma once

#include <chrono>
#include <QString>
#include "UIBuilder.h"

class ReadersManager;
class DataStore;

class DataPresenter {
private:
    ReadersManager* readersManager;
    DataStore* dataStore;
    UIComponents* uiComponents;
    
public:
    DataPresenter(ReadersManager* rm, DataStore* ds, UIComponents* ui);
    
    void updateSensorDisplay(std::chrono::system_clock::time_point currentTime);
    void updateCameraDisplay(std::chrono::system_clock::time_point currentTime);
    void updateTimeDisplay(std::chrono::system_clock::time_point currentTime);
    
private:
    QString formatValue(double value, int precision = 3);
};