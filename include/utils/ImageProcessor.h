#pragma once
#include <QtGui/QPixmap>
#include <QtWidgets/QLabel>
#include <opencv2/opencv.hpp>
#include "data/ImageData.h"

class ImageProcessor {
public:
    static QPixmap matToQPixmap(const cv::Mat& mat);
    
    static void displayImage(ImageData* imageData, QLabel* label);
    
private:
    ImageProcessor() = default; 
};