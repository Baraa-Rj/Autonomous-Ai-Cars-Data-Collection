#include "readers/SpeedDataReader.h"
#include <fstream>
#include <iostream>

std::vector<std::unique_ptr<Data>> SpeedDataReader::readCSV(const std::string& filepath) {
    std::vector<std::unique_ptr<Data>> data;
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open Speed file: " + filepath);
    }
    
    std::string line;
    std::getline(file, line); // Skip header: time_stamp,data_value
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        auto tokens = splitLine(line);
        if (tokens.size() != 2) {
            std::cerr << "Invalid Speed line: " << line << std::endl;
            continue;
        }
        
        try {
            double timestamp = parseDouble(tokens[0]);
            double data_value = parseDouble(tokens[1]);
            
            data.push_back(std::make_unique<SpeedData>(timestamp, data_value));
        } catch (const std::exception& e) {
            std::cerr << "Error parsing Speed line: " << line << " - " << e.what() << std::endl;
        }
    }
    
    file.close();
    return data;
}