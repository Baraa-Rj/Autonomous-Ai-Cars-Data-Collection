#include "readers/DataReader.h"
#include <sstream>
#include <stdexcept>

std::vector<std::string> DataReader::splitLine(const std::string& line, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

double DataReader::parseDouble(const std::string& str) {
    try {
        return std::stod(str);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to parse double: " + str);
    }
}