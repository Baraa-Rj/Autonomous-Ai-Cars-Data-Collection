#include "data/GpsData.h"
#include "data/DataStore.h"

GpsData::GpsData(std::chrono::system_clock::time_point timestamp, float latitude, float longitude, float altitude)
    : Data(timestamp), latitude(latitude), longitude(longitude), altitude(altitude) {}

GpsData::~GpsData() {}

float GpsData::getLatitude() const {
    return latitude;
}

float GpsData::getLongitude() const {
    return longitude;
}

float GpsData::getAltitude() const {
    return altitude;
}

void GpsData::setLatitude(float latitude) {
    this->latitude = latitude;
}

void GpsData::setLongitude(float longitude) {
    this->longitude = longitude;
}

void GpsData::setAltitude(float altitude) {
    this->altitude = altitude;
}

DataType GpsData::getType() const {
    return DataType::GPS;
}