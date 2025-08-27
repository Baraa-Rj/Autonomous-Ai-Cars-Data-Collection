#pragma once
#include <string>
#include <list>
#include <chrono>
#include "../data/Data.h"

class AbstractDataReader {
protected:
    std::string path;
    std::list<Data> items;

public:
    explicit AbstractDataReader(std::string path);
    virtual ~AbstractDataReader();

    virtual void loadData(const std::string& path) = 0;
    std::list<Data> getDataAt(std::chrono::system_clock::time_point time) const;
    const std::list<Data>& getAllData() const { return items; }
};