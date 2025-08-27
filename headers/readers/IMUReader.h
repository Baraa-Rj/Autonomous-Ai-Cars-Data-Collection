#pragma once

#include "AbstractDataReader.h"

class IMUReader : public AbstractDataReader{
    public:
    IMUReader(std::string path);
    ~IMUReader();
    std::list<Data> getDataAt(std::DateTime time);
    void loadData(std::string path);
}