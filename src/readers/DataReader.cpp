#include "readers/DataReader.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

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