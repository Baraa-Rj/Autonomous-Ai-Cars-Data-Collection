#pragma once

#include "Data.h"

class BrakeData : public Data{
    protected:
        float pressure;
        
        public: 
            BrakeData(std::DateTime timestamp, float pressure);
            ~BrakeData();

            float getPressure() const;

            void setPressure(float pressure);

            void print() const;
}