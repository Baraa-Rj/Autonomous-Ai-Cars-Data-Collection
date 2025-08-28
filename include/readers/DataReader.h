#pragma once
#include <string>
#include <vector>
#include <memory>
#include "core/Data.h"

// Abstract base class for all data readers
class DataReader {
public:
    virtual ~DataReader() = default;
    
    // Pure virtual function to read CSV file and return data objects
    virtual std::vector<std::unique_ptr<Data>> readCSV(const std::string& filepath) = 0;
    
    // Helper function to split CSV line by delimiter
    static std::vector<std::string> splitLine(const std::string& line, char delimiter = ',');
    
protected:
    // Helper function to convert string to double with error handling
    static double parseDouble(const std::string& str);
};