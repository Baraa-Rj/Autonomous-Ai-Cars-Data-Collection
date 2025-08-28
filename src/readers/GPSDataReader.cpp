#include "readers/GPSDataReader.h"
#include <fstream>
#include <iostream>

std::vector<std::unique_ptr<Data>> GPSDataReader::readCSV(const std::string& filepath) {
    std::vector<std::unique_ptr<Data>> data;
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open GPS file: " + filepath);
    }
    
    std::string line;
    std::getline(file, line); // Skip header: time_stamp,latitude,longitude,height
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        auto tokens = splitLine(line);
        if (tokens.size() != 4) {
            std::cerr << "Invalid GPS line: " << line << std::endl;
            continue;
        }
        
        try {
            double timestamp = parseDouble(tokens[0]);
            double latitude = parseDouble(tokens[1]);
            double longitude = parseDouble(tokens[2]);
            double height = parseDouble(tokens[3]);
            
            data.push_back(std::make_unique<GPSData>(timestamp, latitude, longitude, height));
        } catch (const std::exception& e) {
            std::cerr << "Error parsing GPS line: " << line << " - " << e.what() << std::endl;
        }
    }
    
    file.close();
    return data;
}