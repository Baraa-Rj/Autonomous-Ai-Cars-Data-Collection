#include "data/ThrottleData.h"
#include <sstream>
#include <iomanip>

std::string ThrottleData::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Throttle: " << data_value << "%";
    return oss.str();
}