#include "readers/SpeedDataReader.h"
#include <fstream>
#include <iostream>


std::unique_ptr<Data> SpeedDataReader::parseLine(const std::string& line) {
    auto tokens = splitLine(line);
    if (tokens.size() != 2) {
        throw std::runtime_error("Invalid Speed line format: expected 2 columns, got " + std::to_string(tokens.size()));
    }
    
    double timestamp = parseDouble(tokens[0]);
    double data_value = parseDouble(tokens[1]);
    
    return std::make_unique<SpeedData>(timestamp, data_value);
}

