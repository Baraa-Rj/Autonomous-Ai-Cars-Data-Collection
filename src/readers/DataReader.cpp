#include "readers/DataReader.h"
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>

bool DataReader::initializeStream(const std::string& filepath) {
    closeStream();
    currentFilepath = filepath;
    fileStream.open(filepath);
    headerSkipped = false;
    
    if (!fileStream.is_open()) {
        std::cerr << "Cannot open file: " << filepath << std::endl;
        return false;
    }
    
    std::string headerLine;
    if (std::getline(fileStream, headerLine)) {
        headerSkipped = true;
    }
    
    return true;
}

void DataReader::closeStream() {
    if (fileStream.is_open()) {
        fileStream.close();
    }
    lastData.reset();
    headerSkipped = false;
}

std::vector<std::string> DataReader::splitLine(const std::string& line, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

double DataReader::parseDouble(const std::string& str) {
    try {
        return std::stod(str);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to parse double: " + str);
    }
}

bool DataReader::loadAllData(const std::string& filepath) {
    clearData();
    currentFilepath = filepath;
    
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filepath << std::endl;
        return false;
    }
    
    std::string line;
    // Skip header
    if (std::getline(file, line)) {
        // Header skipped
    }
    
    // Read all data lines
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        try {
            auto data = parseLine(line);
            if (data) {
                allData.push_back(std::move(data));
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing line in " << filepath << ": " << line << " - " << e.what() << std::endl;
            continue;
        }
    }
    
    file.close();
    
    // Sort data by timestamp for efficient access
    std::sort(allData.begin(), allData.end(), [](const std::unique_ptr<Data>& a, const std::unique_ptr<Data>& b) {
        return a->timestamp < b->timestamp;
    });
    
    std::cout << "Loaded " << allData.size() << " data points from " << filepath << std::endl;
    return !allData.empty();
}

Data* DataReader::getDataAt(size_t index) const {
    if (index >= allData.size()) {
        return nullptr;
    }
    return allData[index].get();
}

void DataReader::clearData() {
    allData.clear();
}