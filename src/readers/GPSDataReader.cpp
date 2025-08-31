#include "readers/GPSDataReader.h"
#include <fstream>
#include <iostream>

std::unique_ptr<Data> GPSDataReader::readNext() {
    if (!fileStream.is_open()) {
        return nullptr;
    }
    
    std::string line;
    while (std::getline(fileStream, line)) {
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
            
            lastData = std::make_unique<GPSData>(timestamp, latitude, longitude, height);
            return std::make_unique<GPSData>(timestamp, latitude, longitude, height);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing GPS line: " << line << " - " << e.what() << std::endl;
            continue;
        }
    }
    
    return nullptr;
}

