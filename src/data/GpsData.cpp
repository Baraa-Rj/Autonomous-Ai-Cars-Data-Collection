#include "data/GpsData.h"

GpsData::GpsData(std::chrono::system_clock::time_point timestamp, float latitude, float longitude, float altitude)
    : Data(timestamp), latitude(latitude), longitude(longitude), altitude(altitude) {}

GpsData::~GpsData() {}

float GpsData::getLatitude() const {
    return latitude;
}