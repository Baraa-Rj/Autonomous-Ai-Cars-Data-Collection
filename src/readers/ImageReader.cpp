#include "readers/ImageReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

ImageReader::ImageReader(std::string path) : AbstractDataReader(path) {}

ImageReader::~ImageReader() {}

std::list<Data> ImageReader::getDataAt(std::chrono::system_clock::time_point time) const {
    return AbstractDataReader::getDataAt(time);
}

void ImageReader::loadData(const std::string& filePath) {
    items.clear();
    std::ifstream file(filePath);
    if (!file.is_open()) return;
    auto trim = [](std::string& s){
        auto isws = [](int c){ return std::isspace(c); };
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](char c){ return !isws(c); }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [&](char c){ return !isws(c); }).base(), s.end());
    };
    std::string line;
    bool first = true;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string tsStr, pathStr, posStr;
        if (!std::getline(iss, tsStr, ',')) continue;
        if (!std::getline(iss, pathStr, ',')) continue;
        if (!std::getline(iss, posStr, ',')) posStr = "FRONT";
        trim(tsStr); trim(pathStr); trim(posStr);
        if (first && (tsStr == "time_stamp" || tsStr == "timestamp")) { first = false; continue; }
        first = false;
        try {
            double ts = std::stod(tsStr);
            auto tp = std::chrono::time_point<std::chrono::system_clock>(
                std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<double>(ts))
            );
            ImageData::CameraPosition pos = ImageData::CameraPosition::FRONT;
            if (posStr == "REAR") pos = ImageData::CameraPosition::REAR;
            else if (posStr == "LEFT") pos = ImageData::CameraPosition::LEFT;
            else if (posStr == "RIGHT") pos = ImageData::CameraPosition::RIGHT;
            ImageData data(tp, pathStr, pos);
            items.push_back(data);
        } catch (...) { continue; }
    }
}