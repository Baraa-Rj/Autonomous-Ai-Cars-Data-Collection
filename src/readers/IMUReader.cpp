#include "readers/IMUReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

IMUReader::IMUReader(std::string path) : AbstractDataReader(path) {}

IMUReader::~IMUReader() {}

std::list<std::shared_ptr<Data>> IMUReader::getDataAt(std::chrono::system_clock::time_point time) const {
    return AbstractDataReader::getDataAt(time);
}

void IMUReader::loadData(const std::string& filePath) {
    items.clear();
    records.clear();
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
        std::string tsStr, axStr, ayStr, azStr, gxStr, gyStr, gzStr;
        if (!std::getline(iss, tsStr, ',')) continue;
        if (!std::getline(iss, axStr, ',')) continue;
        if (!std::getline(iss, ayStr, ',')) continue;
        if (!std::getline(iss, azStr, ',')) continue;
        if (!std::getline(iss, gxStr, ',')) continue;
        if (!std::getline(iss, gyStr, ',')) continue;
        if (!std::getline(iss, gzStr, ',')) continue;
        trim(tsStr); trim(axStr); trim(ayStr); trim(azStr); trim(gxStr); trim(gyStr); trim(gzStr);
        if (first && (tsStr == "time_stamp" || tsStr == "timestamp")) { first = false; continue; }
        first = false;
        try {
            double ts = std::stod(tsStr);
            auto tp = std::chrono::time_point<std::chrono::system_clock>(
                std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<double>(ts))
            );
            std::vector<float> acc{static_cast<float>(std::stod(axStr)), static_cast<float>(std::stod(ayStr)), static_cast<float>(std::stod(azStr))};
            std::vector<float> gyr{static_cast<float>(std::stod(gxStr)), static_cast<float>(std::stod(gyStr)), static_cast<float>(std::stod(gzStr))};
            auto data = std::make_shared<IMUData>(tp, acc, gyr);
            items.push_back(data);
            records.push_back(*data);
        } catch (...) { continue; }
    }
}

std::optional<IMUData> IMUReader::latestAt(std::chrono::system_clock::time_point t) const {
    if (records.empty()) return std::nullopt;
    size_t l = 0, r = records.size();
    while (l < r) {
        size_t m = (l + r) / 2;
        if (records[m].getTimestamp() <= t) l = m + 1; else r = m;
    }
    if (l == 0) return std::nullopt;
    return records[l - 1];
}