#include "headers/data/ImageData.h"

ImageData::ImageData(std::DateTime timestamp, std::string path, cameraPosition position) : Data(timestamp), path(path), position(position) {}

ImageData::~ImageData() {}

std::string ImageData::getPath() const {
    return path;
}