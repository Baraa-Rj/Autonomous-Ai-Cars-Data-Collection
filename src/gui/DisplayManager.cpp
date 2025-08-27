#include "DisplayManager.h"
#include "readers/ReadersManager.h"
#include "sync/TimelineManager.h"
#include <opencv2/opencv.hpp>

DisplayManager::DisplayManager(QWidget* parent)
    : QWidget(parent), dataStore(nullptr), readersManager(nullptr), timelineManager(nullptr) {
    setWindowTitle("Data Collection Phase");
    resize(800, 600);
    dataStore = new DataStore();
    currentTime = std::chrono::system_clock::now();

    // Build UI using dedicated UIBuilder
    uiComponents = UIBuilder::buildUI(this);
    
    // Create data presenter to handle data display logic
    dataPresenter = std::make_unique<DataPresenter>(readersManager, dataStore, &uiComponents);
}

void DisplayManager::displayFrame(std::list<std::shared_ptr<Data>> dataItems) {
    for (const auto& data : dataItems) {
        dataStore->addData(data->getType(), data);
    }
    updateDisplay(currentTime);
}

bool DisplayManager::renderData(std::list<std::shared_ptr<Data>> dataItems) {
    displayFrame(dataItems);
    return true;
}

void DisplayManager::updateDisplay(std::chrono::system_clock::time_point time) {
    currentTime = time;
    
    if (dataPresenter) {
        dataPresenter->updateSensorDisplay(currentTime);
        dataPresenter->updateCameraDisplay(currentTime);
        dataPresenter->updateTimeDisplay(currentTime);
    }
    
    this->update();
}

void DisplayManager::setDataStore(DataStore* ds) {
    dataStore = ds;
    // Update data presenter with new dataStore
    if (dataPresenter) {
        dataPresenter = std::make_unique<DataPresenter>(readersManager, dataStore, &uiComponents);
    }
}

DataStore* DisplayManager::getDataStore() {
    return dataStore;
}

void DisplayManager::setFps(int f) {
    fps = f;
}

int DisplayManager::getFps() const {
    return fps;
}

void DisplayManager::setReadersManager(ReadersManager* rm) {
    readersManager = rm;
    // Update data presenter with new readersManager
    if (dataStore) {
        dataPresenter = std::make_unique<DataPresenter>(readersManager, dataStore, &uiComponents);
    }
}

void DisplayManager::setTimelineManager(TimelineManager* tm) {
    timelineManager = tm;
    if (timelineManager && timelineManager->isInitialized()) {
        currentTime = timelineManager->getGlobalStart();
    }
}