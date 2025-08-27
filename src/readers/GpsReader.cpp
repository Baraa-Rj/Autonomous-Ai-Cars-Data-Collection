#include "headers/readers/GpsReader.h"
#include <fstream>
#include <sstream>

GpsReader::GpsReader(std::string path) : AbstractDataReader(path) {}

GpsReader::~GpsReader() {}

std::list<Data> GpsReader::getDataAt(std::chrono::system_clock::time_point time) const {
    return AbstractDataReader::getDataAt(time);
}

void GpsReader::loadData(const std::string& filePath) {
    items.clear();
    std::ifstream file(filePath);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string tsStr, latStr, lonStr, altStr;
        if (!std::getline(iss, tsStr, ',')) continue;
        if (!std::getline(iss, latStr, ',')) continue;
        if (!std::getline(iss, lonStr, ',')) continue;
        if (!std::getline(iss, altStr, ',')) continue;
        auto tp = std::chrono::system_clock::time_point{std::chrono::seconds(std::stoll(tsStr))};
        float lat = std::stof(latStr);
        float lon = std::stof(lonStr);
        float alt = std::stof(altStr);
        GpsData gps(tp, lat, lon, alt);
        items.push_back(gps);
    }
}