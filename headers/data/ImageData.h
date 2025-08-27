#pragma once

#include "Data.h"
#include <opencv2/opencv.hpp>
#include <string>

class ImageData : public Data {
protected:
    std::string path;
public:
    enum class CameraPosition { FRONT, REAR, LEFT, RIGHT };

    ImageData(std::chrono::system_clock::time_point timestamp, std::string path, CameraPosition position);
    ~ImageData();

    std::string getPath() const;
    CameraPosition getPosition() const;

    void setPath(std::string path);
    void setPosition(CameraPosition position);
    cv::Mat loadImage();

private:
    CameraPosition position;
};