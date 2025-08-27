#pragma once

#include "AbstractDataReader.h"
#include "../data/SpeedData.h"

class SpeedReader : public AbstractDataReader{
    public:
    SpeedReader(std::string path);
    ~SpeedReader();
    std::list<Data> getDataAt(std::chrono::system_clock::time_point time) const;
    void loadData(const std::string& path) override;
};