#pragma once
#include <string>
#include <vector>
#include <memory>
#include "core/Data.h"

class DataReader {
public:
    virtual ~DataReader() = default;
    
    virtual bool loadAllData(const std::string& filepath);
    
    virtual Data* getDataAt(size_t index) const;
    
    virtual size_t getDataCount() const { return allData.size(); }
    
    virtual void clearData();
    
    
    static std::vector<std::string> splitLine(const std::string& line, char delimiter = ',');
    
protected:
    static double parseDouble(const std::string& str);
    
    virtual std::unique_ptr<Data> parseLine(const std::string& line) = 0;
    
    std::vector<std::unique_ptr<Data>> allData;
};