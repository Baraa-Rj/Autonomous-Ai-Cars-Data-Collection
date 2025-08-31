#pragma once
#include "core/Data.h"
#include <opencv2/opencv.hpp>
#include <mutex>
#include <atomic>

class ImageData : public Data {
private:
    mutable std::mutex imageMutex;
    
public:
    std::string filepath;
    cv::Mat image;
    std::atomic<bool> loaded{false};
    std::atomic<bool> loading{false};
    
    ImageData(double ts, const std::string& path) 
        : Data(ts), filepath(path) {}
    
    ImageData(const ImageData& other)
        : Data(other.timestamp), filepath(other.filepath), image(other.image.clone())
    {
        loaded.store(other.loaded.load());
        loading.store(false); 
    }
    
    std::string toString() const override;
    void loadImage();
    void loadImageAsync();
    void releaseImage();
    bool isLoaded() const { return loaded.load(); }
    bool isLoading() const { return loading.load(); }
    
    // Thread-safe image access
    cv::Mat getImage() const {
        std::lock_guard<std::mutex> lock(imageMutex);
        return image.clone(); // Always return a deep copy for thread safety
    }
    
    bool isEmpty() const {
        std::lock_guard<std::mutex> lock(imageMutex);
        return image.empty();
    }
    
    double lastAccessTime{0.0};
    void updateAccessTime();
    bool shouldCleanup(double currentTime, double maxAge = 10.0) const;
};