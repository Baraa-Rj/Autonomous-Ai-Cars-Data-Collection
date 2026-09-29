#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>
#include <string>
#include <limits>
#include <thread>
#include <atomic>
#include <functional>
#include <chrono>


class ClockManager {
public:
    ClockManager();
    ~ClockManager();
    
    void updateRange(double timestamp);
    void resetRange();
    double getMinTimestamp() const { return minTimestamp; }
    double getMaxTimestamp() const { return maxTimestamp; }
    double getDuration() const;
    bool hasValidRange() const;
    
    void setCurrentTimestamp(double timestamp);
    double getCurrentTimestamp() const { return currentTimestamp; }
    void advanceTime(double deltaSeconds);
    double clampToRange(double timestamp) const;
    
    void setProgressFromSlider(int sliderValue);
    int getSliderFromProgress() const;
    double getProgressPercentage() const; 
    void setProgressPercentage(double percentage);
    
    bool isValidTimestamp(double timestamp) const;
    std::string formatElapsedTime(double timestamp) const;
    std::string formatDuration() const;
    
    
    void startTiming(std::function<void()> callback, int intervalMs = 33);
    void stopTiming();
    void setTimingInterval(int intervalMs);
    bool isTimingActive() const { return isRunning.load(); }
    void setPlaybackSpeed(double speed) { playbackSpeed = speed; }
    double getPlaybackSpeed() const { return playbackSpeed; }
    
    static constexpr int SLIDER_MAX = 10000;  

private:
    double minTimestamp;
    double maxTimestamp;
    double currentTimestamp;
    bool hasRange;  // false until the first updateRange(); 0.0 is a valid timestamp
    
    std::atomic<bool> isRunning;
    std::thread timingThread;
    std::function<void()> updateCallback;
    int intervalMs;
    double playbackSpeed;
    
    void timingLoop();
    std::string formatTime(int totalSeconds) const;
};

