#include "readers/IMUDataReader.h"
#include <fstream>
#include <iostream>

std::unique_ptr<Data> IMUDataReader::readNext() {
    if (!fileStream.is_open()) {
        return nullptr;
    }
    
    std::string line;
    while (std::getline(fileStream, line)) {
        if (line.empty()) continue;
        
        try {
            auto data = parseLine(line);
            if (data) {
                lastData = std::make_unique<IMUData>(*static_cast<IMUData*>(data.get()));
                return std::move(data);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing IMU line: " << line << " - " << e.what() << std::endl;
            continue;
        }
    }
    return nullptr;
}

std::unique_ptr<Data> IMUDataReader::parseLine(const std::string& line) {
    auto tokens = splitLine(line);
    if (tokens.size() != 13) {
        throw std::runtime_error("Invalid IMU line format: expected 13 columns, got " + std::to_string(tokens.size()));
    }
    
    double timestamp = parseDouble(tokens[0]);
    double x_acc = parseDouble(tokens[1]);
    double y_acc = parseDouble(tokens[2]);
    double z_acc = parseDouble(tokens[3]);
    double pitch = parseDouble(tokens[4]);
    double roll = parseDouble(tokens[5]);
    double yaw = parseDouble(tokens[6]);
    double x_gyro = parseDouble(tokens[7]);
    double y_gyro = parseDouble(tokens[8]);
    double z_gyro = parseDouble(tokens[9]);
    double x_mag = parseDouble(tokens[10]);
    double y_mag = parseDouble(tokens[11]);
    double z_mag = parseDouble(tokens[12]);
    
    return std::make_unique<IMUData>(timestamp, x_acc, y_acc, z_acc,
                                   pitch, roll, yaw,
                                   x_gyro, y_gyro, z_gyro,
                                   x_mag, y_mag, z_mag);
}

