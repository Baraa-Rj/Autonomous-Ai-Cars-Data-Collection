#pragma once
#include "readers/DataReader.h"
#include "data/ThrottleData.h"

class ThrottleDataReader : public DataReader {
public:
    std::vector<std::unique_ptr<Data>> readCSV(const std::string& filepath) override;
};