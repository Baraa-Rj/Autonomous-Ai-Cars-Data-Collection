#include "readers/IMUDataReader.h"
#include <fstream>
#include <iostream>

std::vector<std::unique_ptr<Data>> IMUDataReader::readCSV(const std::string& filepath) {
    std::vector<std::unique_ptr<Data>> data;
    std::ifstream file(filepath);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open IMU file: " + filepath);
    }
    
    std::string line;
    std::getline(file, line); // Skip header: time_stamp,x_acc,y_acc,z_acc,pitch,roll,yaw,x_gyro,y_gyro,z_gyro,x_mag,y_mag,z_mag
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        auto tokens = splitLine(line);
        if (tokens.size() != 13) {
            std::cerr << "Invalid IMU line: " << line << std::endl;
            continue;
        }
        
        try {
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
            
            data.push_back(std::make_unique<IMUData>(timestamp, x_acc, y_acc, z_acc,
                                                   pitch, roll, yaw,
                                                   x_gyro, y_gyro, z_gyro,
                                                   x_mag, y_mag, z_mag));
        } catch (const std::exception& e) {
            std::cerr << "Error parsing IMU line: " << line << " - " << e.what() << std::endl;
        }
    }
    
    file.close();
    return data;
}