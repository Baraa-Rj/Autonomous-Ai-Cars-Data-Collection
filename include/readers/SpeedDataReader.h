#pragma once
#include "readers/DataReader.h"
#include "data/SpeedData.h"

class SpeedDataReader : public DataReader {
public:
    std::unique_ptr<Data> readNext() override;
};