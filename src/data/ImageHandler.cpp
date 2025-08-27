#include "data/ImageHandler.h"

cv::Mat ImageHandler::load(const std::string& path) {
    return cv::imread(path);
}

cv::Mat ImageHandler::resize(const cv::Mat& image, int width, int height) {
    cv::Mat output;
    cv::resize(image, output, cv::Size(width, height));
    return output;
}