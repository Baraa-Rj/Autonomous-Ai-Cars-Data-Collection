#pragma once
#include "readers/DataReader.h"
#include "data/BrakeData.h"

class BrakeDataReader : public DataReader {
public:
    std::vector<std::unique_ptr<Data>> readCSV(const std::string& filepath) override;
};