#include "readers/ThrottleDataReader.h"
#include <fstream>
#include <iostream>


std::unique_ptr<Data> ThrottleDataReader::parseLine(const std::string& line) {
    auto tokens = splitLine(line);
    if (tokens.size() != 2) {
        throw std::runtime_error("Invalid Throttle line format: expected 2 columns, got " + std::to_string(tokens.size()));
    }
    
    double timestamp = parseDouble(tokens[0]);
    double data_value = parseDouble(tokens[1]);
    
    return std::make_unique<ThrottleData>(timestamp, data_value);
}

