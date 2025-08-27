#pragma once

#include "AbstractDataReader.h"

class ImageReader : public AbstractDataReader{
    public:
    ImageReader(std::string path);
    ~ImageReader();
    std::list<Data> getDataAt(std::DateTime time);
    void loadData(std::string path);
}