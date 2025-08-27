#include "headers/readers/SpeedReader.h"
#include <fstream>
#include <sstream>

SpeedReader::SpeedReader(std::string path) : AbstractDataReader(path) {}

SpeedReader::~SpeedReader() {}

std::list<Data> SpeedReader::getDataAt(std::chrono::system_clock::time_point time) const {
    return AbstractDataReader::getDataAt(time);
}

void SpeedReader::loadData(const std::string& filePath) {
    items.clear();
    std::ifstream file(filePath);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string tsStr, speedStr;
        if (!std::getline(iss, tsStr, ',')) continue;
        if (!std::getline(iss, speedStr, ',')) continue;
        auto tp = std::chrono::system_clock::time_point{std::chrono::seconds(std::stoll(tsStr))};
        float speed = std::stof(speedStr);
        SpeedData data(tp, speed);
        items.push_back(data);
    }
}