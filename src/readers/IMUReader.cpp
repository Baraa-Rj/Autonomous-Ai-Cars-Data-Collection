#include "headers/readers/IMUReader.h"
#include <fstream>
#include <sstream>

IMUReader::IMUReader(std::string path) : AbstractDataReader(path) {}

IMUReader::~IMUReader() {}

std::list<Data> IMUReader::getDataAt(std::chrono::system_clock::time_point time) const {
    return AbstractDataReader::getDataAt(time);
}

void IMUReader::loadData(const std::string& filePath) {
    items.clear();
    std::ifstream file(filePath);
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string tsStr, axStr, ayStr, azStr, gxStr, gyStr, gzStr;
        if (!std::getline(iss, tsStr, ',')) continue;
        if (!std::getline(iss, axStr, ',')) continue;
        if (!std::getline(iss, ayStr, ',')) continue;
        if (!std::getline(iss, azStr, ',')) continue;
        if (!std::getline(iss, gxStr, ',')) continue;
        if (!std::getline(iss, gyStr, ',')) continue;
        if (!std::getline(iss, gzStr, ',')) continue;
        auto tp = std::chrono::system_clock::time_point{std::chrono::seconds(std::stoll(tsStr))};
        std::vector<float> acc{std::stof(axStr), std::stof(ayStr), std::stof(azStr)};
        std::vector<float> gyr{std::stof(gxStr), std::stof(gyStr), std::stof(gzStr)};
        IMUData data(tp, acc, gyr);
        items.push_back(data);
    }
}