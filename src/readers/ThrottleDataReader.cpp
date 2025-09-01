#include "readers/ThrottleDataReader.h"
#include <fstream>
#include <iostream>

std::unique_ptr<Data> ThrottleDataReader::readNext() {
    if (!fileStream.is_open()) {
        return nullptr;
    }
    
    std::string line;
    while (std::getline(fileStream, line)) {
        if (line.empty()) continue;
        
        try {
            auto data = parseLine(line);
            if (data) {
                lastData = std::make_unique<ThrottleData>(*static_cast<ThrottleData*>(data.get()));
                return std::move(data);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing Throttle line: " << line << " - " << e.what() << std::endl;
            continue;
        }
    }
    return nullptr;
}

std::unique_ptr<Data> ThrottleDataReader::parseLine(const std::string& line) {
    auto tokens = splitLine(line);
    if (tokens.size() != 2) {
        throw std::runtime_error("Invalid Throttle line format: expected 2 columns, got " + std::to_string(tokens.size()));
    }
    
    double timestamp = parseDouble(tokens[0]);
    double data_value = parseDouble(tokens[1]);
    
    return std::make_unique<ThrottleData>(timestamp, data_value);
}

