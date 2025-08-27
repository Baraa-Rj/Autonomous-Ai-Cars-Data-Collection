#include "readers/AbstractDataReader.h"
#include <fstream>
#include <sstream>

AbstractDataReader::AbstractDataReader(std::string path) : path(std::move(path)) {}

AbstractDataReader::~AbstractDataReader() {}

std::list<std::shared_ptr<Data>> AbstractDataReader::getDataAt(std::chrono::system_clock::time_point time) const {
    std::list<std::shared_ptr<Data>> result;
    for (const auto& d : items) {
        if (d->getTimestamp() == time) {
            result.push_back(d);
        }
    }
    return result;
}