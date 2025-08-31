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
        
        auto tokens = splitLine(line);
        if (tokens.size() != 2) {
            std::cerr << "Invalid Throttle line: " << line << std::endl;
            continue;
        }
        
        try {
            double timestamp = parseDouble(tokens[0]);
            double data_value = parseDouble(tokens[1]);
            
            lastData = std::make_unique<ThrottleData>(timestamp, data_value);
            return std::make_unique<ThrottleData>(timestamp, data_value);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing Throttle line: " << line << " - " << e.what() << std::endl;
            continue;
        }
    }
    
    return nullptr;
}

