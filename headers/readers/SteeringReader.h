#pragma once

#include "AbstractDataReader.h"

class SteeringReader : public AbstractDataReader{
    public:
    SteeringReader(std::string path);
    ~SteeringReader();
    std::list<Data> getDataAt(std::DateTime time);
    void loadData(std::string path);
}