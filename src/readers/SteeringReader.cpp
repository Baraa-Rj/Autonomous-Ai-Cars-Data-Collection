#include "headers/readers/SteeringReader.h"
#include <fstream>
#include <sstream>

SteeringReader::SteeringReader(std::string path) : AbstractDataReader(path) {}

SteeringReader::~SteeringReader() {}

std::list<Data> SteeringReader::getDataAt(std::chrono::system_clock::time_point time) const {
    return AbstractDataReader::getDataAt(time);
}

void SteeringReader::loadData(const std::string& filePath) {
    items.clear();
    std::ifstream file(filePath);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string tsStr, angleStr;
        if (!std::getline(iss, tsStr, ',')) continue;
        if (!std::getline(iss, angleStr, ',')) continue;
        auto tp = std::chrono::system_clock::time_point{std::chrono::seconds(std::stoll(tsStr))};
        float angle = std::stof(angleStr);
        SteeringData data(tp, angle);
        items.push_back(data);
    }
}