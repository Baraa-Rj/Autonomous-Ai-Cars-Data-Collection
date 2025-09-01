#include "readers/ImageDataReader.h"
#include <filesystem>
#include <iostream>
#include <algorithm>



std::vector<std::unique_ptr<Data>> ImageDataReader::readFromDirectory(const std::string& dirpath) {
    std::vector<std::unique_ptr<Data>> data;
    
    if (!std::filesystem::exists(dirpath)) {
        std::cerr << "Image directory does not exist: " << dirpath << std::endl;
        return data;
    }
    
    std::vector<std::filesystem::path> imageFiles;
    
    for (const auto& entry : std::filesystem::directory_iterator(dirpath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            
            if (ext == ".jpeg" || ext == ".jpg") {
                imageFiles.push_back(entry.path());
            }
        }
    }
    
    std::sort(imageFiles.begin(), imageFiles.end());
    
    for (const auto& imagePath : imageFiles) {
        try {
            std::string filename = imagePath.stem().string(); 
            double timestamp = std::stod(filename);
            
            data.push_back(std::make_unique<ImageData>(timestamp, imagePath.string()));
        } catch (const std::exception& e) {
            std::cerr << "Error parsing image timestamp: " << imagePath << " - " << e.what() << std::endl;
        }
    }
    
    return data;
}

std::unique_ptr<Data> ImageDataReader::parseLine(const std::string& line) {
    // ImageDataReader doesn't use CSV parsing - images are loaded from directory
    throw std::runtime_error("ImageDataReader doesn't support CSV line parsing. Use readFromDirectory() instead.");
}