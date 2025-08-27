#include "readers/ThrottleReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

ThrottleReader::ThrottleReader(std::string path) : AbstractDataReader(path) {}

ThrottleReader::~ThrottleReader() {}

std::list<Data> ThrottleReader::getDataAt(std::chrono::system_clock::time_point time) const {
    return AbstractDataReader::getDataAt(time);
}

void ThrottleReader::loadData(const std::string& filePath) {
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
        std::string tsStr, posStr;
        if (!std::getline(iss, tsStr, ',')) continue;
        if (!std::getline(iss, posStr, ',')) continue;
        trim(tsStr); trim(posStr);
        if (first && (tsStr == "time_stamp" || tsStr == "timestamp")) { first = false; continue; }
        first = false;
        try {
            double ts = std::stod(tsStr);
            auto tp = std::chrono::time_point<std::chrono::system_clock>(
                std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<double>(ts))
            );
            float position = static_cast<float>(std::stod(posStr));
            ThrottleData data(tp, position);
            items.push_back(data);
            records.push_back(data);
        } catch (...) { continue; }
    }
}

std::optional<ThrottleData> ThrottleReader::latestAt(std::chrono::system_clock::time_point t) const {
    if (records.empty()) return std::nullopt;
    size_t l = 0, r = records.size();
    while (l < r) {
        size_t m = (l + r) / 2;
        if (records[m].getTimestamp() <= t) l = m + 1; else r = m;
    }
    if (l == 0) return std::nullopt;
    return records[l - 1];
}