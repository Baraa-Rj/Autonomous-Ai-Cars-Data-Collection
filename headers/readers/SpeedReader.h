#pragma once

#include "AbstractDataReader.h"
#include "../data/SpeedData.h"
#include <vector>
#include <optional>

class SpeedReader : public AbstractDataReader{
    public:
    SpeedReader(std::string path);
    ~SpeedReader();
    std::list<std::shared_ptr<Data>> getDataAt(std::chrono::system_clock::time_point time) const override;
    void loadData(const std::string& path) override;
    std::optional<SpeedData> latestAt(std::chrono::system_clock::time_point t) const;
private:
    std::vector<SpeedData> records;
};