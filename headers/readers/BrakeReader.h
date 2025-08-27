#pragma once

#include "AbstractDataReader.h"
#include "../data/BrakeData.h"
#include <vector>
#include <optional>

class BrakeReader : public AbstractDataReader{
    public:
    BrakeReader(std::string path);
    ~BrakeReader();
    std::list<std::shared_ptr<Data>> getDataAt(std::chrono::system_clock::time_point time) const override;
    void loadData(const std::string& path) override;
    std::optional<BrakeData> latestAt(std::chrono::system_clock::time_point t) const;
private:
    std::vector<BrakeData> records;
};