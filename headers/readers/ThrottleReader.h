#pragma once

#include "AbstractDataReader.h"
#include "../data/ThorttleData.h"

class ThrottleReader : public AbstractDataReader{
    public:
    ThrottleReader(std::string path);
    ~ThrottleReader();
    std::list<Data> getDataAt(std::chrono::system_clock::time_point time) const;
    void loadData(const std::string& path) override;
};