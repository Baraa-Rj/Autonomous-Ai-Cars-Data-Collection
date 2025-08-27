#pragma once

#include "AbstractDataReader.h"

class GpsReader : public AbstractDataReader{
    public:
    GpsReader(std::string path);
    ~GpsReader();
    std::list<Data> getDataAt(std::DateTime time);
    void loadData(std::string path);
}