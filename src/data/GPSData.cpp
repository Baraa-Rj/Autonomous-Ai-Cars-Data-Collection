#include "data/GPSData.h"
#include <sstream>
#include <iomanip>

std::string GPSData::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6);
    oss << "GPS Location:\n"
        << "Lat: " << latitude << "°\n"
        << "Lon: " << longitude << "°\n" 
        << "Alt: " << height << "m";
    return oss.str();
}