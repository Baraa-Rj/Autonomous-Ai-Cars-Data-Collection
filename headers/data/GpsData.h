#pragma once

#include "Data.h"

class GpsData : public Data{
    protected:
        float latitude;
        float longitude;
        float altitude;

        public:
            GpsData(std::DateTime timestamp, float latitude, float longitude, float altitude);
            ~GpsData();

            float getLatitude() const;
            float getLongitude() const;
            float getAltitude() const;
            
            void setLatitude(float latitude);
            void setLongitude(float longitude);
            void setAltitude(float altitude);

}