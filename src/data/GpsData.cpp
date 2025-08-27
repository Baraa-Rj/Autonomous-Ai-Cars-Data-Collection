#include "headers/data/GpsData.h"

GpsData::GpsData(std::DateTime timestamp, float latitude, float longitude, float altitude) : Data(timestamp), latitude(latitude), longitude(longitude), altitude(altitude) {}

GpsData::~GpsData() {}

float GpsData::getLatitude() const {
    return latitude;
}