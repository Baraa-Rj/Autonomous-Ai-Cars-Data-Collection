#include "readers/ImageReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <regex>

ImageReader::ImageReader(std::string path) : AbstractDataReader(path) {}

ImageReader::~ImageReader() {}

std::list<Data> ImageReader::getDataAt(std::chrono::system_clock::time_point time) const {
    return AbstractDataReader::getDataAt(time);
}

static std::chrono::system_clock::time_point tp_from_seconds_double(double secs) {
    return std::chrono::time_point<std::chrono::system_clock>(
        std::chrono::duration_cast<std::chrono::system_clock::duration>(
            std::chrono::duration<double>(secs))
    );
}

void ImageReader::loadData(const std::string& filePath) {
    items.clear();

    namespace fs = std::filesystem;
    std::error_code ec;
    if (fs::is_directory(filePath, ec) && !ec) {
        std::regex re("^([0-9]+(?:\\.[0-9]+)?)");
        std::vector<ImageData> temp;
        for (const auto& entry : fs::directory_iterator(filePath)) {
            if (!entry.is_regular_file()) continue;
            const auto ext = entry.path().extension().string();
            std::string lowerExt = ext; std::transform(lowerExt.begin(), lowerExt.end(), lowerExt.begin(), ::tolower);
            if (lowerExt != ".jpg" && lowerExt != ".jpeg" && lowerExt != ".png") continue;
            const std::string name = entry.path().stem().string();
            std::smatch m;
            if (!std::regex_search(name, m, re)) continue;
            try {
                double secs = std::stod(m[1].str());
                auto tp = tp_from_seconds_double(secs);
                ImageData data(tp, entry.path().string(), ImageData::CameraPosition::FRONT);
                temp.push_back(data);
            } catch (...) { continue; }
        }
        std::sort(temp.begin(), temp.end(), [](const ImageData& a, const ImageData& b){
            return a.getTimestamp() < b.getTimestamp();
        });
        for (const auto& d : temp) items.push_back(d);
        return;
    }

    }
