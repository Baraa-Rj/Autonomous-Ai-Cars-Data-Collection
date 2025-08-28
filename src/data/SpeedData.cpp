#include "data/SpeedData.h"
#include <sstream>
#include <iomanip>

std::string SpeedData::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Speed: " << data_value << " km/h";
    return oss.str();
}