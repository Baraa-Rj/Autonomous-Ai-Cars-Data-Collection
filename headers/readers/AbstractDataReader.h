#pragma once
#include <string>
#include <list>
#include <chrono>
#include <memory>
#include "../data/Data.h"

class AbstractDataReader {
protected:
    std::string path;
    std::list<std::shared_ptr<Data>> items;

public:
    explicit AbstractDataReader(std::string path);
    virtual ~AbstractDataReader();

    virtual void loadData(const std::string& path) = 0;
    virtual std::list<std::shared_ptr<Data>> getDataAt(std::chrono::system_clock::time_point time) const;
    const std::list<std::shared_ptr<Data>>& getAllData() const { return items; }
};