#pragma once
#include "readers/DataReader.h"
#include "data/GPSData.h"

class GPSDataReader : public DataReader {
public:
    std::vector<std::unique_ptr<Data>> readCSV(const std::string& filepath) override;
};