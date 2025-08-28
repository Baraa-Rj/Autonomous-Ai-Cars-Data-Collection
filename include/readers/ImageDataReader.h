#pragma once
#include "readers/DataReader.h"
#include "data/ImageData.h"

class ImageDataReader : public DataReader {
public:
    std::vector<std::unique_ptr<Data>> readCSV(const std::string& filepath) override;
    
    // Special method to read images from directory based on timestamp filenames
    std::vector<std::unique_ptr<Data>> readFromDirectory(const std::string& dirpath);
};