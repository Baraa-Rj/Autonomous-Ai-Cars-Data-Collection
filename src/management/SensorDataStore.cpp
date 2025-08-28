#include "management/SensorDataStore.h"

void SensorDataStore::clear() {
    gps_data.clear();
    imu_data.clear();
    speed_data.clear();
    brake_data.clear();
    throttle_data.clear();
    steering_data.clear();
    front_images.clear();
    back_images.clear();
    left_images.clear();
    right_images.clear();
}

size_t SensorDataStore::getTotalDataPoints() const {
    return gps_data.size() + imu_data.size() + speed_data.size() + 
           brake_data.size() + throttle_data.size() + steering_data.size() +
           front_images.size() + back_images.size() + left_images.size() + 
           right_images.size();
}