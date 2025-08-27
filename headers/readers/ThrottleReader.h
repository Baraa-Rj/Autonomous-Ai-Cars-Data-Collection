#pragma once

#include "AbstractDataReader.h"
#include "../data/ThorttleData.h"
#include <vector>
#include <optional>

class ThrottleReader : public AbstractDataReader{
    public:
    ThrottleReader(std::string path);
    ~ThrottleReader();
    std::list<Data> getDataAt(std::chrono::system_clock::time_point time) const;
    void loadData(const std::string& path) override;
    std::optional<ThrottleData> latestAt(std::chrono::system_clock::time_point t) const;
private:
    std::vector<ThrottleData> records;
};