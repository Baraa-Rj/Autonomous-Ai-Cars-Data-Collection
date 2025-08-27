#include "data/ImageData.h"
#include "data/DataStore.h"

ImageData::ImageData(std::chrono::system_clock::time_point timestamp, std::string path, CameraPosition position)
    : Data(timestamp), path(std::move(path)), position(position) {}

ImageData::~ImageData() {}

std::string ImageData::getPath() const {
    return path;
}

DataType ImageData::getType() const {
    switch (position) {
        case CameraPosition::FRONT: return DataType::FRONT_IMAGE;
        case CameraPosition::REAR: return DataType::BACK_IMAGE;
        case CameraPosition::LEFT: return DataType::LEFT_IMAGE;
        case CameraPosition::RIGHT: return DataType::RIGHT_IMAGE;
        default: return DataType::FRONT_IMAGE;
    }
}