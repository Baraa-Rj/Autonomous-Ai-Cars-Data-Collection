#include "sync/ClockManager.h"

ClockManager::ClockManager(QObject* parent) : QObject(parent) {}

ClockManager::~ClockManager() { stop(); }

void ClockManager::start() {
    stop();
    running = true;
    currentTime = std::chrono::system_clock::now();
    loopThread = std::thread([this]() {
        using namespace std::chrono;
        const double frameMs = 1000.0 / static_cast<double>(fps);
        while (running.load()) {
            auto t = tick();
            emit ticked(t);
            std::this_thread::sleep_for(duration<double, std::milli>(frameMs));
        }
    });
}

void ClockManager::stop() {
    if (running.exchange(false)) {
        if (loopThread.joinable()) loopThread.join();
    }
}
std::chrono::system_clock::time_point ClockManager::tick() {
    currentTime = std::chrono::system_clock::now();
    return currentTime;
}
void ClockManager::setFps(int fps) { this->fps = fps; }
int ClockManager::getFps() const { return fps; }
void ClockManager::setCurrentTime(std::chrono::system_clock::time_point time) { currentTime = time; }
std::chrono::system_clock::time_point ClockManager::getCurrentTime() const { return currentTime; }
