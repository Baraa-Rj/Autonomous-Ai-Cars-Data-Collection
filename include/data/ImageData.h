#pragma once
#include "core/Data.h"
#include <opencv2/opencv.hpp>

class ImageData : public Data {
public:
    std::string filepath;
    cv::Mat image;
    bool loaded;
    
    ImageData(double ts, const std::string& path) 
        : Data(ts), filepath(path), loaded(false) {}
    
    std::string toString() const override;
    void loadImage();
    void releaseImage();
};