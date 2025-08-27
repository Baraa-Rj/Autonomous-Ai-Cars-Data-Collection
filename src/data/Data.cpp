#include "headers/data/Data.h"

Data::Data(std::chrono::system_clock::time_point timestamp) : timestamp(timestamp) {}

Data::~Data() {}

std::chrono::system_clock::time_point Data::getTimestamp() const {
    return timestamp;
}