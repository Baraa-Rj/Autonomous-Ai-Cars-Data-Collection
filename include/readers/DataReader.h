#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include "core/Data.h"

// Abstract base class for all data readers
class DataReader {
public:
    virtual ~DataReader() = default;
    
    // Initialize streaming read from CSV file
    virtual bool initializeStream(const std::string& filepath);
    
    // Read next data point from stream, returns nullptr if no more data
    virtual std::unique_ptr<Data> readNext() = 0;
    
    // Close stream and cleanup
    virtual void closeStream();
    
    // Helper function to split CSV line by delimiter
    static std::vector<std::string> splitLine(const std::string& line, char delimiter = ',');
    
protected:
    // Helper function to convert string to double with error handling
    static double parseDouble(const std::string& str);
    
    // Stream and state management
    std::ifstream fileStream;
    std::string currentFilepath;
    std::unique_ptr<Data> lastData;
    bool headerSkipped = false;
};