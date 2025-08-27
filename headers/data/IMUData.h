#pragma once

#include "Data.h"

class IMUData : public Data{
    protected:
        std::vector<float> acceleration;
        std::vector<float> gyroscope;

        public:
            IMUData(std::DateTime timestamp, std::vector<float> acceleration, std::vector<float> gyroscope);
            ~IMUData();

            std::vector<float> getAcceleration() const;
            std::vector<float> getGyroscope() const;

            void setAcceleration(std::vector<float> acceleration);
            void setGyroscope(std::vector<float> gyroscope);
        
}