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
        
        try {
            auto data = parseLine(line);
            if (data) {
                lastData = std::make_unique<GPSData>(*static_cast<GPSData*>(data.get()));
                return std::move(data);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing GPS line: " << line << " - " << e.what() << std::endl;
            continue;
        }
    }
    return nullptr;
}

std::unique_ptr<Data> GPSDataReader::parseLine(const std::string& line) {
    auto tokens = splitLine(line);
    if (tokens.size() != 4) {
        throw std::runtime_error("Invalid GPS line format: expected 4 columns, got " + std::to_string(tokens.size()));
    }
    
    double timestamp = parseDouble(tokens[0]);
    double latitude = parseDouble(tokens[1]);
    double longitude = parseDouble(tokens[2]);
    double height = parseDouble(tokens[3]);
    
    return std::make_unique<GPSData>(timestamp, latitude, longitude, height);
}

