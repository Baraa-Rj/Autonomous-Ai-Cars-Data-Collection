#include "headers/data/ImageHandler.h"

cv::Mat ImageHandler::load(std::string path) {
    return cv::imread(path);
}

cv::Mat ImageHandler::resize(cv::Mat image, int width, int height) {
    return cv::resize(image, cv::Size(width, height));
}