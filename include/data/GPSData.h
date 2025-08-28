#pragma once
#include "core/Data.h"

class GPSData : public Data {
public:
    double latitude;
    double longitude;
    double height;
    
    GPSData(double ts, double lat, double lon, double h) 
        : Data(ts), latitude(lat), longitude(lon), height(h) {}
    
    std::string toString() const override;
};