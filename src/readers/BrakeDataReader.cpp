#include "readers/BrakeDataReader.h"
#include <fstream>
#include <iostream>

std::unique_ptr<Data> BrakeDataReader::readNext() {
    if (!fileStream.is_open()) {
        return nullptr;
    }
    
    std::string line;
    while (std::getline(fileStream, line)) {
        if (line.empty()) continue;
        
        auto tokens = splitLine(line);
        if (tokens.size() != 2) {
            std::cerr << "Invalid Brake line: " << line << std::endl;
            continue;
        }
        
        try {
            double timestamp = parseDouble(tokens[0]);
            double data_value = parseDouble(tokens[1]);
            
            lastData = std::make_unique<BrakeData>(timestamp, data_value);
            return std::make_unique<BrakeData>(timestamp, data_value);
        } catch (const std::exception& e) {
            std::cerr << "Error parsing Brake line: " << line << " - " << e.what() << std::endl;
            continue;
        }
    }
    
    return nullptr;
}

