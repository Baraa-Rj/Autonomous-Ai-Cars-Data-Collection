#include "headers/data/ImageData.h"

ImageData::ImageData(std::chrono::system_clock::time_point timestamp, std::string path, CameraPosition position)
    : Data(timestamp), path(std::move(path)), position(position) {}

ImageData::~ImageData() {}

std::string ImageData::getPath() const {
    return path;
}