#include "management/ClockManager.h"
#include <sstream>
#include <iomanip>

ClockManager::ClockManager() 
    : minTimestamp(0.0), maxTimestamp(0.0), currentTimestamp(0.0) {
}

void ClockManager::updateRange(double timestamp) {
    if (minTimestamp == 0.0 && maxTimestamp == 0.0) {
        minTimestamp = maxTimestamp = timestamp;
    } else {
        if (timestamp < minTimestamp) minTimestamp = timestamp;
        if (timestamp > maxTimestamp) maxTimestamp = timestamp;
    }
}

void ClockManager::resetRange() {
    minTimestamp = 0.0;
    maxTimestamp = 0.0;
    currentTimestamp = 0.0;
}

double ClockManager::getDuration() const {
    return hasValidRange() ? maxTimestamp - minTimestamp : 0.0;
}

bool ClockManager::hasValidRange() const {
    return maxTimestamp > minTimestamp;
}

void ClockManager::setCurrentTimestamp(double timestamp) {
    currentTimestamp = clampToRange(timestamp);
}

void ClockManager::advanceTime(double deltaSeconds) {
    setCurrentTimestamp(currentTimestamp + deltaSeconds);
}

double ClockManager::clampToRange(double timestamp) const {
    if (!hasValidRange()) return timestamp;
    
    if (timestamp < minTimestamp) return minTimestamp;
    if (timestamp > maxTimestamp) return maxTimestamp;
    return timestamp;
}

void ClockManager::setProgressFromSlider(int sliderValue) {
    if (!hasValidRange()) return;
    
    double percentage = static_cast<double>(sliderValue) / SLIDER_MAX;
    double targetTime = minTimestamp + (maxTimestamp - minTimestamp) * percentage;
    setCurrentTimestamp(targetTime);
}

int ClockManager::getSliderFromProgress() const {
    if (!hasValidRange()) return 0;
    
    double percentage = (currentTimestamp - minTimestamp) / (maxTimestamp - minTimestamp);
    return static_cast<int>(percentage * SLIDER_MAX);
}

double ClockManager::getProgressPercentage() const {
    if (!hasValidRange()) return 0.0;
    
    return (currentTimestamp - minTimestamp) / (maxTimestamp - minTimestamp);
}

void ClockManager::setProgressPercentage(double percentage) {
    if (!hasValidRange()) return;
    
    percentage = std::max(0.0, std::min(1.0, percentage)); 
    double targetTime = minTimestamp + (maxTimestamp - minTimestamp) * percentage;
    setCurrentTimestamp(targetTime);
}

bool ClockManager::isValidTimestamp(double timestamp) const {
    return hasValidRange() && timestamp >= minTimestamp && timestamp <= maxTimestamp;
}

std::string ClockManager::formatElapsedTime(double timestamp) const {
    if (!hasValidRange()) return "00:00";
    
    double elapsed = timestamp - minTimestamp;
    int totalSeconds = static_cast<int>(elapsed);
    return formatTime(totalSeconds);
}

std::string ClockManager::formatDuration() const {
    if (!hasValidRange()) return "00:00";
    
    int totalSeconds = static_cast<int>(getDuration());
    return formatTime(totalSeconds);
}

std::string ClockManager::formatTime(int totalSeconds) const {
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;
    
    std::ostringstream oss;
    if (hours > 0) {
        oss << std::setfill('0') << std::setw(2) << hours << ":"
            << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setfill('0') << std::setw(2) << seconds;
    } else {
        oss << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setfill('0') << std::setw(2) << seconds;
    }
    
    return oss.str();
}