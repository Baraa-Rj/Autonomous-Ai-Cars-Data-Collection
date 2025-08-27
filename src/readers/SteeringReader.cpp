#include "readers/SteeringReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

SteeringReader::SteeringReader(std::string path) : AbstractDataReader(path) {}

SteeringReader::~SteeringReader() {}

std::list<Data> SteeringReader::getDataAt(std::chrono::system_clock::time_point time) const {
    return AbstractDataReader::getDataAt(time);
}

void SteeringReader::loadData(const std::string& filePath) {
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
        std::string tsStr, angleStr;
        if (!std::getline(iss, tsStr, ',')) continue;
        if (!std::getline(iss, angleStr, ',')) continue;
        trim(tsStr); trim(angleStr);
        if (first && (tsStr == "time_stamp" || tsStr == "timestamp")) { first = false; continue; }
        first = false;
        try {
            double ts = std::stod(tsStr);
            auto tp = std::chrono::time_point<std::chrono::system_clock>(
                std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<double>(ts))
            );
            float angle = static_cast<float>(std::stod(angleStr));
            SteeringData data(tp, angle);
            items.push_back(data);
        } catch (...) { continue; }
    }
}