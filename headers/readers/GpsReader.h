#pragma once

#include "AbstractDataReader.h"
#include "../data/GpsData.h"

class GpsReader : public AbstractDataReader{
    public:
    GpsReader(std::string path);
    ~GpsReader();
    std::list<Data> getDataAt(std::chrono::system_clock::time_point time) const;
    void loadData(const std::string& path) override;
};