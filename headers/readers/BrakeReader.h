#pragma once

#include "AbstractDataReader.h"

class BrakeReader : public AbstractDataReader{
    public:
    BrakeReader(std::string path);
    ~BrakeReader();
    std::list<Data> getDataAt(std::DateTime time);
    void loadData(std::string path);
}