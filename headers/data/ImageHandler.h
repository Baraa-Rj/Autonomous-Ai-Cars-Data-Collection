#pragma once
#include <opencv2/opencv.hpp>

class ImageHandler{
    cv::Mat load(std::string path);
    cv::Mat resize(cv::Mat image, int width, int height);
}