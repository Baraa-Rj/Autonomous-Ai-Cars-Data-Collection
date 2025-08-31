#pragma once
#include "readers/DataReader.h"
#include "data/GPSData.h"

class GPSDataReader : public DataReader {
public:
    std::unique_ptr<Data> readNext() override;
};