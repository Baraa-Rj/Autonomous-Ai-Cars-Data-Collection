#pragma once
#include "readers/DataReader.h"
#include "data/SpeedData.h"

class SpeedDataReader : public DataReader {
public:
    std::vector<std::unique_ptr<Data>> readCSV(const std::string& filepath) override;
};