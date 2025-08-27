#pragma once

#include "AbstractDataReader.h"

class SpeedReader : public AbstractDataReader{
    public:
    SpeedReader(std::string path);
    ~SpeedReader();
    std::list<Data> getDataAt(std::DateTime time);
    void loadData(std::string path);
}