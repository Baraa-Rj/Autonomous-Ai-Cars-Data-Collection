#pragma once
#include <opencv2/opencv.hpp>
#include <string>

class ImageHandler {
public:
    cv::Mat load(const std::string& path);
    cv::Mat resize(const cv::Mat& image, int width, int height);
};