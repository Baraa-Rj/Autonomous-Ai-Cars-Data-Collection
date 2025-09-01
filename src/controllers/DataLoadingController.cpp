#include "controllers/DataLoadingController.h"
#include "management/DataManager.h"

DataLoadingController::DataLoadingController(DataManager* dataManager, QObject* parent)
    : QObject(parent)
    , dataManager(dataManager)
{
    if (dataManager) {
        connect(dataManager, &DataManager::dataLoaded, 
                this, &DataLoadingController::onDataLoaded);
        connect(dataManager, &DataManager::dataLoadingProgress, 
                this, &DataLoadingController::onDataLoadingProgress);
        connect(dataManager, &DataManager::dataLoadingError, 
                this, &DataLoadingController::onDataLoadingError);
    }
}

bool DataLoadingController::isLoading() const {
    return dataManager ? dataManager->isLoading() : false;
}

void DataLoadingController::loadData(const QString& dataDirectory) {
    if (!dataManager || dataDirectory.isEmpty()) return;
    
    emit loadingStarted();
    dataManager->loadAllSensorDataAsync(dataDirectory.toStdString());
}

void DataLoadingController::cancelLoading() {
    if (dataManager) {
        dataManager->cancelLoading();
    }
}

void DataLoadingController::onDataLoaded() {
    emit loadingFinished();
}

void DataLoadingController::onDataLoadingProgress(int percentage) {
    emit loadingProgress(percentage);
}

void DataLoadingController::onDataLoadingError(const QString& error) {
    emit loadingError(error);
}