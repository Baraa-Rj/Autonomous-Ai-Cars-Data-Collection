#pragma once
#include "readers/DataReader.h"
#include "data/IMUData.h"

class IMUDataReader : public DataReader {
public:
    std::vector<std::unique_ptr<Data>> readCSV(const std::string& filepath) override;
};