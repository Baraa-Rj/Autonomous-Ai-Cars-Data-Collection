#pragma once

#include "AbstractDataReader.h"
#include "../data/GpsData.h"
#include <vector>
#include <optional>

class GpsReader : public AbstractDataReader{
    public:
    GpsReader(std::string path);
    ~GpsReader();
    std::list<std::shared_ptr<Data>> getDataAt(std::chrono::system_clock::time_point time) const override;
    void loadData(const std::string& path) override;

    std::optional<GpsData> latestAt(std::chrono::system_clock::time_point t) const;

private:
    std::vector<GpsData> records;
};