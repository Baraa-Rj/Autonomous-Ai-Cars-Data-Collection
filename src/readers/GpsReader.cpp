#include "readers/GpsReader.h"
#include <fstream>
#include <sstream>
#include<iostream>
#include <cctype>
#include <algorithm>
GpsReader::GpsReader(std::string path) : AbstractDataReader(path) {}

GpsReader::~GpsReader() {}

std::list<Data> GpsReader::getDataAt(std::chrono::system_clock::time_point time) const {
    return AbstractDataReader::getDataAt(time);
}

void GpsReader::loadData(const std::string& filePath) {
    items.clear();
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::perror(("Failed to open " + filePath).c_str());
        return;
    }
    auto trim = [](std::string& s) {
        auto isws = [](int c){ return std::isspace(c); };
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [&](char c){ return !isws(c); }));
        s.erase(std::find_if(s.rbegin(), s.rend(), [&](char c){ return !isws(c); }).base(), s.end());
    };
    std::string line;
    size_t parsed = 0;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string tsStr, latStr, lonStr, altStr;
        if (!std::getline(iss, tsStr, ',')) continue;
        if (!std::getline(iss, latStr, ',')) continue;
        if (!std::getline(iss, lonStr, ',')) continue;
        if (!std::getline(iss, altStr, ',')) continue;
        trim(tsStr); trim(latStr); trim(lonStr); trim(altStr);
        if (tsStr.empty() || !std::isdigit(static_cast<unsigned char>(tsStr[0]))) continue;
        try {
            long long ts = std::stoll(tsStr);
            std::chrono::system_clock::time_point tp = (ts > 1000000000000LL)
                ? std::chrono::system_clock::time_point{std::chrono::milliseconds(ts)}
                : std::chrono::system_clock::time_point{std::chrono::seconds(ts)};
            GpsData gps(tp, std::stof(latStr), std::stof(lonStr), std::stof(altStr));
            items.push_back(gps);
            ++parsed;
        } catch (...) {
            continue;
        }
    }
    std::cout << "GpsReader parsed rows: " << parsed << std::endl;
}
