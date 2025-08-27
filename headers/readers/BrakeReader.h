#pragma once

#include "AbstractDataReader.h"
#include "../data/BrakeData.h"

class BrakeReader : public AbstractDataReader{
    public:
    BrakeReader(std::string path);
    ~BrakeReader();
    std::list<Data> getDataAt(std::chrono::system_clock::time_point time) const;
    void loadData(const std::string& path) override;
};