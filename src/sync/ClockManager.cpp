#include "sync/ClockManager.h"
#include <thread>
#include <chrono>

ClockManager::ClockManager(QObject* parent) 
    : QObject(parent), fps(60), running(false) {}

ClockManager::~ClockManager() {
    stop();
}

void ClockManager::start() {
    if (running) return;
    
    running = true;
    currentTime = std::chrono::system_clock::now();
    
    // Start timing thread
    timingThread = std::thread([this]() {
        auto frameInterval = std::chrono::microseconds(1000000 / fps);
        auto nextFrameTime = std::chrono::steady_clock::now();
        
        while (running) {
            nextFrameTime += frameInterval;
            
            // Update current time and emit tick
            this->tick();
            
            // Sleep until next frame
            std::this_thread::sleep_until(nextFrameTime);
        }
    });
}

void ClockManager::stop() {
    running = false;
    if (timingThread.joinable()) {
        timingThread.join();
    }
}

std::chrono::system_clock::time_point ClockManager::tick() {
    currentTime = std::chrono::system_clock::now();
    // Emit signal if you need Qt signal/slot mechanism
    // emit timeUpdated();
    return currentTime;
}

void ClockManager::setFps(int fps) {
    this->fps = fps;
    // If running, restart with new FPS
    if (running) {
        stop();
        start();
    }
}

int ClockManager::getFps() const {
    return fps;
}

void ClockManager::setCurrentTime(std::chrono::system_clock::time_point time) {
    currentTime = time;
}

std::chrono::system_clock::time_point ClockManager::getCurrentTime() const {
    return currentTime;
}