#include "data/ImageData.h"
#include <sstream>
#include <thread>
#include <iostream>

std::string ImageData::toString() const {
    std::ostringstream oss;
    oss << "Image: " << filepath << " (loaded: " << (loaded.load() ? "yes" : "no") << ")";
    return oss.str();
}

void ImageData::loadImage() {
    if (loaded.load()) return;
    
    std::lock_guard<std::mutex> lock(imageMutex);
    if (loaded.load()) return; 
    
    cv::Mat tempImage = cv::imread(filepath);
    if (!tempImage.empty()) {
        image = std::move(tempImage);
        loaded.store(true);
    } else {
        std::cerr << "Failed to load image: " << filepath << std::endl;
    }
}

void ImageData::loadImageAsync() {
    if (loaded.load() || loading.load()) return;
    
    loading.store(true);
    loadingFuture = std::async(std::launch::async, [this]() {
        loadImage();
        loading.store(false);
    });
}

void ImageData::releaseImage() {
    std::lock_guard<std::mutex> lock(imageMutex);
    if (loaded.load()) {
        image.release();
        loaded.store(false);
    }
}