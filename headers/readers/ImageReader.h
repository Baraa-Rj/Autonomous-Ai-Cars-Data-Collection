#pragma once

#include "AbstractDataReader.h"
#include "../data/ImageData.h"
#include "../data/ImageHandler.h"

class ImageReader : public AbstractDataReader{
    public:
    ImageReader(std::string path);
    ~ImageReader();
    std::list<std::shared_ptr<Data>> getDataAt(std::chrono::system_clock::time_point time) const override;
    void loadData(const std::string& path) override;
};