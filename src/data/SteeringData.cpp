#include "data/SteeringData.h"
#include <sstream>
#include <iomanip>

std::string SteeringData::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Steering: " << data_value << "°";
    return oss.str();
}