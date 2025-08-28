#include "utils/ImageHandler.h"
#include <QtGui/QImage>
#include <opencv2/imgproc.hpp>

QPixmap ImageHandler::matToQPixmap(const cv::Mat& mat) {
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

QPixmap ImageHandler::resizePixmap(const QPixmap& pixmap, const QSize& targetSize, 
                                  Qt::AspectRatioMode aspectRatio,
                                  Qt::TransformationMode mode) {
    return pixmap.scaled(targetSize, aspectRatio, mode);
}

cv::Mat ImageHandler::resizeMat(const cv::Mat& mat, const cv::Size& targetSize, 
                               int interpolation) {
    cv::Mat resized;
    cv::resize(mat, resized, targetSize, 0, 0, interpolation);
    return resized;
}