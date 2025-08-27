#pragma once

#include "AbstractDataReader.h"

class ThrottleReader : public AbstractDataReader{
    public:
    ThrottleReader(std::string path);
    ~ThrottleReader();
    std::list<Data> getDataAt(std::DateTime time);
    void loadData(std::string path);
}