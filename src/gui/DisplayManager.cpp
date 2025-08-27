#include "DisplayManager.h"

DisplayManager::DisplayManager(QWidget* parent)
    : QWidget(parent) {
    setWindowTitle("Data Collection Phase");
    resize(800, 600);
    dataStore = nullptr;
    fps = 30;
    currentTime = std::chrono::system_clock::now();
    clockManager = new ClockManager(this);
    clockManager->setFps(fps);
    clockManager->start();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() { this->update(); });
}



