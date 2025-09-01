#pragma once
#include <string>
#include <vector>
#include <memory>
#include "core/Data.h"

// Abstract base class for all data readers
class DataReader {
public:
    virtual ~DataReader() = default;
    
    // New: Load all data from CSV file into memory
    virtual bool loadAllData(const std::string& filepath);
    
    // New: Get data point at specific index
    virtual Data* getDataAt(size_t index) const;
    
    // New: Get total number of loaded data points
    virtual size_t getDataCount() const { return allData.size(); }
    
    // New: Clear all loaded data
    virtual void clearData();
    
    
    // Helper function to split CSV line by delimiter
    static std::vector<std::string> splitLine(const std::string& line, char delimiter = ',');
    
protected:
    // Helper function to convert string to double with error handling
    static double parseDouble(const std::string& str);
    
    // Parse a single CSV line into a Data object
    virtual std::unique_ptr<Data> parseLine(const std::string& line) = 0;
    
    // Storage for preloaded data
    std::vector<std::unique_ptr<Data>> allData;
};