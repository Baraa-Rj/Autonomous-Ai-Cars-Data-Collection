#include "utils/ImageProcessor.h"
#include <QtCore/Qt>

QPixmap ImageProcessor::matToQPixmap(const cv::Mat& mat) {
    QImage qimg;
    
    if (mat.channels() == 3) {
        cv::Mat rgbMat;
        cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB);
        qimg = QImage(rgbMat.data, rgbMat.cols, rgbMat.rows, rgbMat.step, QImage::Format_RGB888).copy();
    } else if (mat.channels() == 1) {
        qimg = QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8).copy();
    } else {
        return QPixmap(); 
    }
    
    return QPixmap::fromImage(qimg);
}

void ImageProcessor::displayImage(ImageData* imageData, QLabel* label) {
    if (!imageData || !label) return;
    
    // Load image on-demand if needed
    if (!imageData->isLoaded()) {
        imageData->loadImageAsync();
    }
    
    // Display if successfully loaded
    if (imageData->isLoaded() && !imageData->isEmpty()) {
        cv::Mat image = imageData->getImage();
        QPixmap pixmap = matToQPixmap(image);
        QPixmap scaled = pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        label->setPixmap(scaled);
    }
}