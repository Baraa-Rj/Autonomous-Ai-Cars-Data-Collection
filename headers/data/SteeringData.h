#pragma once

#include "Data.h"

class SteeringData : public Data{
    protected:
        float angle;

        public:
            SteeringData(std::DateTime timestamp, float angle);
            ~SteeringData();

            float getAngle() const;

            void setAngle(float angle);

}