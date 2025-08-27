#pragma once

#include "AbstractDataReader.h"
#include "../data/SteeringData.h"
#include <vector>
#include <optional>

class SteeringReader : public AbstractDataReader{
    public:
    SteeringReader(std::string path);
    ~SteeringReader();
    std::list<Data> getDataAt(std::chrono::system_clock::time_point time) const;
    void loadData(const std::string& path) override;
    std::optional<SteeringData> latestAt(std::chrono::system_clock::time_point t) const;
private:
    std::vector<SteeringData> records;
};