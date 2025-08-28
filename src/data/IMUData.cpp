#include "data/IMUData.h"
#include <sstream>
#include <iomanip>

std::string IMUData::toString() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1);
    oss << "IMU Data:\n"
        << "Acc: " << x_acc << ", " << y_acc << ", " << z_acc << "\n"
        << "Gyro: " << x_gyro << ", " << y_gyro << ", " << z_gyro << "\n"
        << "Pitch: " << pitch << "° Roll: " << roll << "° Yaw: " << yaw << "°";
    return oss.str();
}