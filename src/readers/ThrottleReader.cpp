#include "headers/readers/ThrottleReader.h"
#include <fstream>
#include <sstream>

ThrottleReader::ThrottleReader(std::string path) : AbstractDataReader(path) {}

ThrottleReader::~ThrottleReader() {}

std::list<Data> ThrottleReader::getDataAt(std::chrono::system_clock::time_point time) const {
    return AbstractDataReader::getDataAt(time);
}

void ThrottleReader::loadData(const std::string& filePath) {
    items.clear();
    std::ifstream file(filePath);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string tsStr, posStr;
        if (!std::getline(iss, tsStr, ',')) continue;
        if (!std::getline(iss, posStr, ',')) continue;
        auto tp = std::chrono::system_clock::time_point{std::chrono::seconds(std::stoll(tsStr))};
        float position = std::stof(posStr);
        ThrottleData data(tp, position);
        items.push_back(data);
    }
}