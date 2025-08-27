#include "headers/sync/ClockManager.h"

ClockManager::ClockManager() {}

ClockManager::~ClockManager() {}

void ClockManager::start() {
    currentTime = std::DateTime::now();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ClockManager::tick);
}
std::DateTime ClockManager::tick() {
    return currentTime;
}
void ClockManager::setFps(int fps) {
    this->fps = fps;
}
int ClockManager::getFps() {
    return fps;
}
void ClockManager::setCurrentTime(std::DateTime time) {
    currentTime = time;
}
