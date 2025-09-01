#pragma once
#include "readers/DataReader.h"
#include "data/GPSData.h"

class GPSDataReader : public DataReader {
protected:
    std::unique_ptr<Data> parseLine(const std::string& line) override;
};