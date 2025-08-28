#pragma once
#include "readers/DataReader.h"
#include "data/SteeringData.h"

class SteeringDataReader : public DataReader {
public:
    std::vector<std::unique_ptr<Data>> readCSV(const std::string& filepath) override;
};