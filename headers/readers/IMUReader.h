#pragma once

#include "AbstractDataReader.h"
#include "../data/IMUData.h"
#include <vector>
#include <optional>

class IMUReader : public AbstractDataReader{
    public:
    IMUReader(std::string path);
    ~IMUReader();
    std::list<std::shared_ptr<Data>> getDataAt(std::chrono::system_clock::time_point time) const override;
    void loadData(const std::string& path) override;
    std::optional<IMUData> latestAt(std::chrono::system_clock::time_point t) const;
private:
    std::vector<IMUData> records;
};