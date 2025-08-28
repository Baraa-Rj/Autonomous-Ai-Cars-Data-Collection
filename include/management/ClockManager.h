#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include <cmath>
#include <string>

/**
 * Centralized time management system for the Car Status Visualization application.
 * Handles timestamp ranges, current playback position, timeline calculations, and data searching.
 */
class ClockManager {
public:
    ClockManager();
    
    // Range Management
    void updateRange(double timestamp);
    void resetRange();
    double getMinTimestamp() const { return minTimestamp; }
    double getMaxTimestamp() const { return maxTimestamp; }
    double getDuration() const;
    bool hasValidRange() const;
    
    // Current Position Management
    void setCurrentTimestamp(double timestamp);
    double getCurrentTimestamp() const { return currentTimestamp; }
    void advanceTime(double deltaSeconds);
    double clampToRange(double timestamp) const;
    
    // Timeline/Slider Conversion (0-10000 scale as used in MainWindow)
    void setProgressFromSlider(int sliderValue);
    int getSliderFromProgress() const;
    double getProgressPercentage() const; // 0.0 to 1.0
    void setProgressPercentage(double percentage);
    
    // Utility Functions
    bool isValidTimestamp(double timestamp) const;
    std::string formatElapsedTime(double timestamp) const;
    std::string formatDuration() const;
    
    // Template function for finding closest data (moved from DataManager)
    template<typename T>
    T* findClosestData(const std::vector<T>& dataVector, double timestamp) const;
    
    // Constants
    static constexpr double TOLERANCE = 0.01; // 10ms tolerance for closest data matching
    static constexpr int SLIDER_MAX = 10000;  // Maximum slider value used in UI

private:
    double minTimestamp;
    double maxTimestamp;
    double currentTimestamp;
    
    // Helper functions
    std::string formatTime(int totalSeconds) const;
};

// Template implementation
template<typename T>
T* ClockManager::findClosestData(const std::vector<T>& dataVector, double timestamp) const {
    if (dataVector.empty()) return nullptr;
    
    // Binary search for closest timestamp
    auto it = std::lower_bound(dataVector.begin(), dataVector.end(), timestamp,
                              [](const T& data, double ts) { return data.timestamp < ts; });
    
    if (it == dataVector.end()) {
        // Return last element if timestamp is beyond all data
        return const_cast<T*>(&dataVector.back());
    } else if (it == dataVector.begin()) {
        // Return first element if timestamp is before all data
        return const_cast<T*>(&dataVector.front());
    } else {
        // Check which is closer: current or previous
        auto prev = it - 1;
        double currentDiff = std::abs(it->timestamp - timestamp);
        double prevDiff = std::abs(prev->timestamp - timestamp);
        
        // Use tolerance to prefer more recent data if very close
        if (currentDiff < prevDiff || (std::abs(currentDiff - prevDiff) < TOLERANCE && it->timestamp >= timestamp)) {
            return const_cast<T*>(&(*it));
        } else {
            return const_cast<T*>(&(*prev));
        }
    }
}