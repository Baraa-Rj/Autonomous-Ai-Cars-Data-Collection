#include "sync/ClockManager.h"

ClockManager::ClockManager(QObject* parent) : QObject(parent) {}

ClockManager::~ClockManager() { delete timer; }

void ClockManager::start() {
    currentTime = std::chrono::system_clock::now();
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() { this->tick(); });
}
std::chrono::system_clock::time_point ClockManager::tick() {
    currentTime = std::chrono::system_clock::now();
    return currentTime;
}
void ClockManager::setFps(int fps) {
    this->fps = fps;
}
int ClockManager::getFps() const {
    return fps;
}
void ClockManager::setCurrentTime(std::chrono::system_clock::time_point time) {
    currentTime = time;
}
std::chrono::system_clock::time_point ClockManager::getCurrentTime() const { return currentTime; }
