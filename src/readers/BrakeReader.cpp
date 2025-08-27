#include "headers/readers/BrakeReader.h"
#include <fstream>
#include <sstream>

BrakeReader::BrakeReader(std::string path) : AbstractDataReader(path) {}

BrakeReader::~BrakeReader() {}

std::list<Data> BrakeReader::getDataAt(std::chrono::system_clock::time_point time) const {
    return AbstractDataReader::getDataAt(time);
}

void BrakeReader::loadData(const std::string& filePath) {
    items.clear();
    std::ifstream file(filePath);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string tsStr, pressureStr;
        if (!std::getline(iss, tsStr, ',')) continue;
        if (!std::getline(iss, pressureStr, ',')) continue;
        auto tp = std::chrono::system_clock::time_point{std::chrono::seconds(std::stoll(tsStr))};
        float pressure = std::stof(pressureStr);
        BrakeData data(tp, pressure);
        items.push_back(data);
    }
}