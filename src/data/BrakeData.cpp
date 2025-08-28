#include "data/BrakeData.h"
#include <sstream>
#include <iomanip>

std::string BrakeData::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "Brake: " << data_value << "%";
    return oss.str();
}