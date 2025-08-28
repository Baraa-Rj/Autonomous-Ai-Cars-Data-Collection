#pragma once
#include "core/Data.h"
#include <opencv2/opencv.hpp>
#include <mutex>
#include <future>

class ImageData : public Data {
private:
    mutable std::mutex imageMutex;
    std::future<void> loadingFuture;
    
public:
    std::string filepath;
    cv::Mat image;
    std::atomic<bool> loaded{false};
    std::atomic<bool> loading{false};
    
    ImageData(double ts, const std::string& path) 
        : Data(ts), filepath(path) {}
    
    // Copy constructor
    ImageData(const ImageData& other)
        : Data(other.timestamp), filepath(other.filepath), image(other.image.clone())
    {
        loaded.store(other.loaded.load());
        loading.store(false); // Don't copy loading state
    }
    
    // Move constructor
    ImageData(ImageData&& other) noexcept
        : Data(other.timestamp), filepath(std::move(other.filepath)), 
          image(std::move(other.image)), loadingFuture(std::move(other.loadingFuture))
    {
        loaded.store(other.loaded.exchange(false));
        loading.store(other.loading.exchange(false));
    }
    
    // Copy assignment
    ImageData& operator=(const ImageData& other) {
        if (this != &other) {
            Data::timestamp = other.timestamp;
            filepath = other.filepath;
            image = other.image.clone();
            loaded.store(other.loaded.load());
            loading.store(false); // Don't copy loading state
        }
        return *this;
    }
    
    // Move assignment
    ImageData& operator=(ImageData&& other) noexcept {
        if (this != &other) {
            Data::timestamp = other.timestamp;
            filepath = std::move(other.filepath);
            image = std::move(other.image);
            loadingFuture = std::move(other.loadingFuture);
            loaded.store(other.loaded.exchange(false));
            loading.store(other.loading.exchange(false));
        }
        return *this;
    }
    
    std::string toString() const override;
    void loadImage();
    void loadImageAsync();
    void releaseImage();
    bool isLoaded() const { return loaded.load(); }
    bool isLoading() const { return loading.load(); }
};