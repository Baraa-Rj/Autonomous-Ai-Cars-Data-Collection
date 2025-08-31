#include "data/ImageData.h"
#include <sstream>
#include <thread>
#include <iostream>
#include <chrono>

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
        if (tempImage.cols > 800 || tempImage.rows > 600) {
            cv::Mat resized;
            double scale = std::min(800.0 / tempImage.cols, 600.0 / tempImage.rows);
            cv::resize(tempImage, resized, cv::Size(), scale, scale, cv::INTER_AREA);
            image = resized.clone(); // Use clone() instead of move to ensure deep copy
        } else {
            image = tempImage.clone(); // Use clone() instead of move to ensure deep copy
        }
        loaded.store(true);
    } else {
        std::cerr << "Failed to load image: " << filepath << std::endl;
    }
}

void ImageData::loadImageAsync() {
    // Disable async loading for now - load synchronously to avoid threading issues
    if (loaded.load()) return;
    loadImage();
}

void ImageData::releaseImage() {
    std::lock_guard<std::mutex> lock(imageMutex);
    if (loaded.load()) {
        image.release();
        loaded.store(false);
    }
}