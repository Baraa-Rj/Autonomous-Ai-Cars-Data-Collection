#pragma once

#include "AbstractDataReader.h"
#include "../data/ThrottleData.h"
#include <vector>
#include <optional>

class ThrottleReader : public AbstractDataReader{
    public:
    ThrottleReader(std::string path);
    ~ThrottleReader();
    std::list<std::shared_ptr<Data>> getDataAt(std::chrono::system_clock::time_point time) const override;
    void loadData(const std::string& path) override;
    std::optional<ThrottleData> latestAt(std::chrono::system_clock::time_point t) const;
private:
    std::vector<ThrottleData> records;
};