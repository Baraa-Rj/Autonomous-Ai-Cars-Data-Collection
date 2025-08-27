#include "services/DataLoaderService.h"
#include "readers/AbstractDataReader.h"
#include "readers/BrakeReader.h"
#include "readers/GpsReader.h"
#include "readers/ImageReader.h"
#include "readers/IMUReader.h"
#include "readers/SpeedReader.h"
#include "readers/SteeringReader.h"
#include "readers/ThrottleReader.h"
#include <QDir>
#include <QFileInfo>
#include <iostream>

DataLoaderService::DataLoaderService(const std::string& dataPath) 
    : baseDataPath(dataPath) {}

void DataLoaderService::loadCSVData(std::map<DataType, std::unique_ptr<AbstractDataReader>>& readers) {
    std::pair<DataType, std::string> csvFiles[] = {
        {DataType::GPS, getCSVPath(DataType::GPS)},
        {DataType::SPEED, getCSVPath(DataType::SPEED)},
        {DataType::BRAKE, getCSVPath(DataType::BRAKE)},
        {DataType::THROTTLE, getCSVPath(DataType::THROTTLE)},
        {DataType::STEERING, getCSVPath(DataType::STEERING)},
        {DataType::IMU, getCSVPath(DataType::IMU)}
    };
    
    for (const auto& [type, path] : csvFiles) {
        if (QFileInfo(QString::fromStdString(path)).exists()) {
            std::unique_ptr<AbstractDataReader> reader;
            
            switch (type) {
                case DataType::GPS:
                    reader = std::make_unique<GpsReader>(path);
                    break;
                case DataType::SPEED:
                    reader = std::make_unique<SpeedReader>(path);
                    break;
                case DataType::BRAKE:
                    reader = std::make_unique<BrakeReader>(path);
                    break;
                case DataType::THROTTLE:
                    reader = std::make_unique<ThrottleReader>(path);
                    break;
                case DataType::STEERING:
                    reader = std::make_unique<SteeringReader>(path);
                    break;
                case DataType::IMU:
                    reader = std::make_unique<IMUReader>(path);
                    break;
                default:
                    continue;
            }
            
            if (reader) {
                reader->loadData(path);
                readers[type] = std::move(reader);
                std::cout << "Loaded " << path << std::endl;
            }
        }
    }
}

void DataLoaderService::loadImageData(std::map<DataType, std::unique_ptr<AbstractDataReader>>& readers) {
    std::pair<DataType, std::string> imageDirs[] = {
        {DataType::LEFT_IMAGE, getImageDirPath(DataType::LEFT_IMAGE)},
        {DataType::FRONT_IMAGE, getImageDirPath(DataType::FRONT_IMAGE)},
        {DataType::RIGHT_IMAGE, getImageDirPath(DataType::RIGHT_IMAGE)},
        {DataType::BACK_IMAGE, getImageDirPath(DataType::BACK_IMAGE)}
    };
    
    for (const auto& [type, dir] : imageDirs) {
        if (QDir(QString::fromStdString(dir)).exists()) {
            auto reader = std::make_unique<ImageReader>(dir);
            reader->loadData(dir);
            readers[type] = std::move(reader);
            std::cout << "Loaded images from " << dir << std::endl;
        }
    }
}

std::string DataLoaderService::getCSVPath(DataType type) const {
    switch (type) {
        case DataType::GPS: return baseDataPath + "/gps.csv";
        case DataType::SPEED: return baseDataPath + "/speed.csv";
        case DataType::BRAKE: return baseDataPath + "/brake.csv";
        case DataType::THROTTLE: return baseDataPath + "/throttle.csv";
        case DataType::STEERING: return baseDataPath + "/steering.csv";
        case DataType::IMU: return baseDataPath + "/imu.csv";
        default: return "";
    }
}

std::string DataLoaderService::getImageDirPath(DataType type) const {
    std::string imageBase = baseDataPath + "/3d_images";
    switch (type) {
        case DataType::LEFT_IMAGE: return imageBase + "/left";
        case DataType::FRONT_IMAGE: return imageBase + "/front";
        case DataType::RIGHT_IMAGE: return imageBase + "/right";
        case DataType::BACK_IMAGE: return imageBase + "/back";
        default: return "";
    }
}