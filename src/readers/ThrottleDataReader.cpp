#include "readers/ThrottleDataReader.h"
#include <fstream>
#include <iostream>

std::vector<std::unique_ptr<Data>> ThrottleDataReader::readCSV(const std::string& filepath) {
    std::vector<std::unique_ptr<Data>> data;
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open Throttle file: " + filepath);
    }
    
    std::string line;
    std::getline(file, line); // Skip header: time_stamp,data_value
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        auto tokens = splitLine(line);
        if (tokens.size() != 2) {
            std::cerr << "Invalid Throttle line: " << line << std::endl;
            continue;
        }
        
        try {
            double timestamp = parseDouble(tokens[0]);
            double data_value = parseDouble(tokens[1]);
            
            data.push_back(std::make_unique<ThrottleData>(timestamp, data_value));
        } catch (const std::exception& e) {
            std::cerr << "Error parsing Throttle line: " << line << " - " << e.what() << std::endl;
        }
    }
    
    file.close();
    return data;
}