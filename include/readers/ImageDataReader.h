#pragma once
#include "readers/DataReader.h"
#include "data/ImageData.h"

class ImageDataReader : public DataReader {
public:
    std::vector<std::unique_ptr<Data>> readFromDirectory(const std::string& dirpath);
    
protected:
    std::unique_ptr<Data> parseLine(const std::string& line) override;
};