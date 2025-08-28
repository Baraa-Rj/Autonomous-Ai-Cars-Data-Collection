#include "readers/ImageDataReader.h"
#include <filesystem>
#include <iostream>
#include <algorithm>

std::vector<std::unique_ptr<Data>> ImageDataReader::readCSV(const std::string& filepath) {
    // Images don't have CSV files, this method is not used for images
    return std::vector<std::unique_ptr<Data>>();
}

std::vector<std::unique_ptr<Data>> ImageDataReader::readFromDirectory(const std::string& dirpath) {
    std::vector<std::unique_ptr<Data>> data;
    
    if (!std::filesystem::exists(dirpath)) {
        std::cerr << "Image directory does not exist: " << dirpath << std::endl;
        return data;
    }
    
    std::vector<std::filesystem::path> imageFiles;
    
    // Collect all JPEG files
    for (const auto& entry : std::filesystem::directory_iterator(dirpath)) {
        if (entry.is_regular_file()) {
            std::string ext = entry.path().extension().string();
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            
            if (ext == ".jpeg" || ext == ".jpg") {
                imageFiles.push_back(entry.path());
            }
        }
    }
    
    // Sort files by name (which are timestamps)
    std::sort(imageFiles.begin(), imageFiles.end());
    
    // Create ImageData objects
    for (const auto& imagePath : imageFiles) {
        try {
            std::string filename = imagePath.stem().string(); // Get filename without extension
            double timestamp = std::stod(filename); // Filename is the timestamp
            
            data.push_back(std::make_unique<ImageData>(timestamp, imagePath.string()));
        } catch (const std::exception& e) {
            std::cerr << "Error parsing image timestamp: " << imagePath << " - " << e.what() << std::endl;
        }
    }
    
    return data;
}