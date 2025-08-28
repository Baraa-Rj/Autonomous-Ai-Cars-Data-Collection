#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>
#include <string>
#include <limits>


class ClockManager {
public:
    ClockManager();
    
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
    
    template<typename T>
    T* findClosestData(const std::vector<T>& dataVector) const;
    
    static constexpr int SLIDER_MAX = 10000;  

private:
    double minTimestamp;
    double maxTimestamp;
    double currentTimestamp;
    
    std::string formatTime(int totalSeconds) const;
};

template<typename T>
T* ClockManager::findClosestData(const std::vector<T>& dataVector) const {
    if (dataVector.empty()) return nullptr;
    
    double currentTs = getCurrentTimestamp();
    
    auto it = std::lower_bound(dataVector.begin(), dataVector.end(), currentTs,
        [](const T& data, double timestamp) {
            return data.timestamp < timestamp;
        });
    
    if (it == dataVector.begin()) {
        return const_cast<T*>(&(*it));
    }
    
    --it;
    return const_cast<T*>(&(*it));
}
