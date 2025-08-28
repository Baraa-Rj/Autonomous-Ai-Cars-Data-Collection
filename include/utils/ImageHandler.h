#pragma once

#include <QtGui/QPixmap>
#include <QtCore/QSize>
#include <opencv2/opencv.hpp>

class ImageHandler {
public:
    static QPixmap matToQPixmap(const cv::Mat& mat);
    
    static QPixmap resizePixmap(const QPixmap& pixmap, const QSize& targetSize, 
                               Qt::AspectRatioMode aspectRatio = Qt::KeepAspectRatio,
                               Qt::TransformationMode mode = Qt::SmoothTransformation);
    
    static cv::Mat resizeMat(const cv::Mat& mat, const cv::Size& targetSize, 
                            int interpolation = cv::INTER_LINEAR);

private:
    ImageHandler() = default;
};