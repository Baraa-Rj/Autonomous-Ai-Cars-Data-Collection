    #include "data/ImageHandler.h"
#include <QLabel>
#include <QPixmap>

cv::Mat ImageHandler::load(const std::string& path) {
    return cv::imread(path);
}

cv::Mat ImageHandler::resize(const cv::Mat& image, int width, int height) {
    cv::Mat output;
    cv::resize(image, output, cv::Size(width, height));
    return output;
}

QImage ImageHandler::matToQImage(const cv::Mat& mat) {
    if (mat.empty()) return QImage();
    
    switch (mat.type()) {
        case CV_8UC3: {
            QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
            return img.rgbSwapped();
        }
        case CV_8UC1: {
            QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
            return img;
        }
        default:
            return QImage();
    }
}

void ImageHandler::setImageOnLabel(QLabel* label, const QString& imagePath) {
    if (!label) return;
    
    cv::Mat mat = cv::imread(imagePath.toStdString());
    if (mat.empty()) return;
    
    QImage qimg = matToQImage(mat);
    if (qimg.isNull()) return;
    
    QPixmap pixmap = QPixmap::fromImage(qimg);
    label->setPixmap(pixmap.scaled(label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}