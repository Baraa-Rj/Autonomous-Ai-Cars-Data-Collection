#pragma once

#include "Data.h"

class ImageData : public Data{
    protected:
        std::string path;
        enum cameraPosition{
            FRONT,
            REAR,
            LEFT,
            RIGHT
        };

        public:
            ImageData(std::DateTime timestamp, std::string path, cameraPosition position);
            ~ImageData();

            std::string getPath() const;
            cameraPosition getPosition() const;

            void setPath(std::string path);
            void setPosition(cameraPosition position);
            cv::Mat loadImage();
}