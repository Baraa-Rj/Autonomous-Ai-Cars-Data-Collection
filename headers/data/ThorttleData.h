#pragma once

#include "Data.h"

class ThorttleData : public Data{
    protected:
        float position;

        public:
            ThorttleData(std::DateTime timestamp, float position);
            ~ThorttleData();
            float getPosition() const;

            void setPosition(float position);

            
}