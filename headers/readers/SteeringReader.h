#pragma once

#include "AbstractDataReader.h"
#include "../data/SteeringData.h"

class SteeringReader : public AbstractDataReader{
    public:
    SteeringReader(std::string path);
    ~SteeringReader();
    std::list<Data> getDataAt(std::chrono::system_clock::time_point time) const;
    void loadData(const std::string& path) override;
};