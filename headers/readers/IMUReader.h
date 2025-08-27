#pragma once

#include "AbstractDataReader.h"
#include "../data/IMUData.h"

class IMUReader : public AbstractDataReader{
    public:
    IMUReader(std::string path);
    ~IMUReader();
    std::list<Data> getDataAt(std::chrono::system_clock::time_point time) const;
    void loadData(const std::string& path) override;
};