#pragma once
#include "readers/DataReader.h"
#include "data/BrakeData.h"

class BrakeDataReader : public DataReader {
public:
    std::unique_ptr<Data> readNext() override;
};