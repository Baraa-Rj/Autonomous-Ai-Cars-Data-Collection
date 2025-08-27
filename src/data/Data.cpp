#include "headers/data/Data.h"

Data::Data(std::DateTime timestamp) : timestamp(timestamp) {}

Data::~Data() {}

std::DateTime Data::getTimestamp() const {
    return timestamp;
}