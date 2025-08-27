#pragma once

#include <string>
#include <map>
#include <memory>
#include "../data/DataStore.h"

class AbstractDataReader;

class DataLoaderService {
private:
    std::string baseDataPath;
    
public:
    explicit DataLoaderService(const std::string& dataPath);
    
    void loadCSVData(std::map<DataType, std::unique_ptr<AbstractDataReader>>& readers);
    void loadImageData(std::map<DataType, std::unique_ptr<AbstractDataReader>>& readers);
    
private:
    std::string getCSVPath(DataType type) const;
    std::string getImageDirPath(DataType type) const;
};