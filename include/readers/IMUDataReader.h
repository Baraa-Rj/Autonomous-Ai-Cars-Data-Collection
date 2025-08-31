#pragma once
#include "readers/DataReader.h"
#include "data/IMUData.h"

class IMUDataReader : public DataReader {
public:
    std::unique_ptr<Data> readNext() override;
};