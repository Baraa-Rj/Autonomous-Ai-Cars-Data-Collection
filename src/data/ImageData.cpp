#include "data/ImageData.h"
#include <sstream>

std::string ImageData::toString() const {
    std::ostringstream oss;
    oss << "Image: " << filepath << " (loaded: " << (loaded ? "yes" : "no") << ")";
    return oss.str();
}

void ImageData::loadImage() {
    if (!loaded) {
        image = cv::imread(filepath);
        loaded = !image.empty();
    }
}

void ImageData::releaseImage() {
    if (loaded) {
        image.release();
        loaded = false;
    }
}