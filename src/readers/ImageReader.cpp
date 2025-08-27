#include "headers/readers/ImageReader.h"
#include <fstream>
#include <sstream>

ImageReader::ImageReader(std::string path) : AbstractDataReader(path) {}

ImageReader::~ImageReader() {}

std::list<Data> ImageReader::getDataAt(std::chrono::system_clock::time_point time) const {
    return AbstractDataReader::getDataAt(time);
}

void ImageReader::loadData(const std::string& filePath) {
    items.clear();
    std::ifstream file(filePath);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string tsStr, pathStr, posStr;
        if (!std::getline(iss, tsStr, ',')) continue;
        if (!std::getline(iss, pathStr, ',')) continue;
        if (!std::getline(iss, posStr, ',')) posStr = "FRONT";
        auto tp = std::chrono::system_clock::time_point{std::chrono::seconds(std::stoll(tsStr))};
        ImageData::CameraPosition pos = ImageData::CameraPosition::FRONT;
        if (posStr == "REAR") pos = ImageData::CameraPosition::REAR;
        else if (posStr == "LEFT") pos = ImageData::CameraPosition::LEFT;
        else if (posStr == "RIGHT") pos = ImageData::CameraPosition::RIGHT;
        ImageData data(tp, pathStr, pos);
        items.push_back(data);
    }
}