#include "DisplayManager.h"

DisplayManager::DisplayManager(QWidget* parent)
    : QWidget(parent) {
    setWindowTitle("Data Collection Phase");
    resize(800, 600);
    dataStore = new DataStore();
    fps = 30;
    currentTime = std::DateTime::now();
    clock = new ClockManager();
    clock->setFps(fps);
    clock->start();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DisplayManager::update);
}


